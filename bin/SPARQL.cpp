/* Copyright Eric Prud'hommeaux 2009.
 * Distributed under the Apache Software License.
 * thanks to Vladimir Prus's boost:program_options examples.

 * $Id$ */

#include <iostream>
#include <fstream>
#include <iterator>

#ifndef TEST_CLI
#include "SWObjects.hpp"
namespace sw = w3c_sw;
#include "SPARQLfedParser/SPARQLfedParser.hpp"
#include "TurtleSParser/TurtleSParser.hpp"
#include "TrigSParser/TrigSParser.hpp"
#include "RdfDB.hpp"
#include "ResultSet.hpp"
#include "RdfXmlParser.hpp"

#include "XMLQueryExpressor.hpp"
#include "QueryMapper.hpp"
#include "SPARQLSerializer.hpp"
#include "SQLizer.hpp"

#if XML_PARSER == SWOb_LIBXML2
  #include "../interface/SAXparser_libxml.hpp"
  sw::SAXparser_libxml P;
#elif XML_PARSER == SWOb_EXPAT1
  #include "../interface/SAXparser_expat.hpp"
  sw::SAXparser_expat P;
#elif XML_PARSER == SWOb_MSXML3
  #include "../interface/SAXparser_msxml3.hpp"
  sw::SAXparser_msxml3 P;
#else
  #warning DAWG tests require an XML parser
#endif

#if HTTP_CLIENT == SWOb_ASIO
  #include "../interface/WEBagent_boostASIO.hpp"
  sw::WEBagent_boostASIO Agent;
#endif /* HTTP_CLIENT == SWOb_ASIO */
#endif /* !TEST_CLI */

/* Keep all inclusions of boost *after* the inclusion of SWObjects.hpp
 * (or include config.h manually) */
#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#include <boost/regex.hpp>

#if TEST_CLI
/* Simulate HTParse interface (with bogus results, but good enough for testing CLI). */
namespace sw {
    namespace libwww {
	typedef enum {PARSE_all} e_PARSE_opts;
	std::string HTParse (std::string name, const std::string* rel, e_PARSE_opts /* wanted */)
	{
	    std::string ret;
	    if (rel)
		ret = *rel;
	    return ret.append(name);
	}
    };
};
#endif

const sw::POS* CwdURI;
const sw::POS* BaseURI;
std::string BaseUriMessage () {
    return (BaseURI == NULL)
	? std::string(" with no base URI")
	: std::string(" with base URI ") + BaseURI->getLexicalValue();
}

const sw::POS* ArgBaseURI;
bool NoExec = false;
int Debug = 0;
bool Quiet = false;
const sw::POS* NamedGraphName = NULL;
const sw::POS* Query; // URI is a guery ref; RDFLiteral is a query string.
typedef std::vector<const sw::POS*> mapList;
mapList Maps;
std::string DataMediaType;
std::string UserName;
std::string PassWord;
std::map<std::string, std::string> HTTPHeaders;

#ifndef TEST_CLI
std::ostream* DebugStream = NULL;
sw::POSFactory F;
sw::RdfDB Db(&Agent, &P, &DebugStream);
sw::SPARQLfedDriver SparqlParser("", &F);
sw::TurtleSDriver TurtleParser("", &F);
sw::TrigSDriver TrigParser("", &F);
sw::RdfXmlParser GRdfXmlParser(&F, &P);
sw::RDFaParser GRDFaParser(&F, &P);
sw::QueryMapper QueryMapper(&F, &DebugStream);
sw::ParserDriver* Parsers[] = {
    &SparqlParser, 
    &TurtleParser, 
    &TrigParser, 
    &GRdfXmlParser, 
    &GRDFaParser
};

struct loadEntry {
    const sw::POS* graphName;
    const sw::POS* resource;
    const sw::POS* baseURI;
    loadEntry (const sw::POS* graphName, const sw::POS* resource, const sw::POS* baseURI)
	: graphName(graphName), resource(resource), baseURI(baseURI) {  }
    void loadGraph () {
	const sw::POS* graph = graphName ? graphName : sw::DefaultGraph;
	Db.loadData(resource, Db.assureGraph(graph), &F); // !!! baseURI
    }
};
typedef std::vector<loadEntry> loadList;
loadList LoadList;
loadEntry Output(NULL, NULL, NULL);
bool InPlace = false;

#endif /* TEST_CLI */

/* Set Debug when parsed. */
struct debugLevel { };
void validate (boost::any& v, const std::vector<std::string>& values, debugLevel*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    std::stringstream stream(s);
    int i;
    stream >> i;
    Debug = i;
    v = boost::any(debugLevel());
    if (Debug > 0)
	std::cout << "debug level: " << Debug << "\n";
}

/* Set Query to an RDFLiteral when parsed. */
struct queryString {};
void validate (boost::any&, const std::vector<std::string>& values, queryString*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    if (Query != NULL)
	throw boost::program_options
	    ::validation_error(std::string("query string: \"").
			       append(s).append("\" is redundant against ").
			       append(Query->getLexicalValue()));
    Query = F.getRDFLiteral(s);
}

/* Set DataMediaType when parsed. */
struct langName { };
void validate (boost::any&, const std::vector<std::string>& values, langName*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    if (!s.compare("?")) {
	std::cout << "data language options: \"\", guess, turtle, trig, rdfa, rdfxml";
    } else {
	if (!s.compare(""))
	    DataMediaType = "";
	else if (!s.compare("guess"))
	    DataMediaType = "text/plain";
	else if (!s.compare("turtle"))
	    DataMediaType = "text/turtle";
	else if (!s.compare("trig"))
	    DataMediaType = "text/trig";
	else if (!s.compare("rdfa") || !s.compare("html"))
	    DataMediaType = "text/html";
	else if (!s.compare("rdfxml"))
	    DataMediaType = "application/rdf+xml";
	else {
	    throw boost::program_options::validation_error(std::string("invalid value: \"").append(s).append("\""));
	}
	if (Debug > 0) {
	    if (DataMediaType.size() == 0)
		std::cout << "using no data language mediatype.\n";
	    else
		std::cout << "using data language mediatype " << DataMediaType << ".\n";
	}
    }
}
struct langType { };
void validate (boost::any&, const std::vector<std::string>& values, langType*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    if (!s.compare("?")) {
	std::cout << "data mediatype options: \"\", text/plain, text/turtle, text/trig, text/html, application/rdf:xml";
    } else {
	if (!s.compare(""))
	    DataMediaType = "";
	else if (!s.compare("text/plain"))
	    DataMediaType = "text/plain";
	else if (!s.compare("text/turtle"))
	    DataMediaType = "text/turtle";
	else if (!s.compare("text/trig"))
	    DataMediaType = "text/trig";
	else if (!s.compare("text/html"))
	    DataMediaType = "text/html";
	else if (!s.compare("application/rdf+xml"))
	    DataMediaType = "application/rdf+xml";
	else {
	    throw boost::program_options::validation_error(std::string("invalid value: \"").append(s).append("\""));
	}
	if (Debug > 0) {
	    if (DataMediaType.size() == 0)
		std::cout << "using no data mediatype mediatype.\n";
	    else
		std::cout << "using data mediatype mediatype " << DataMediaType << ".\n";
	}
    }
}

/* Base class for all relative URI arguments. */
struct relURI {};

const sw::POS* htparseWrapper(std::string s, const sw::POS* base) {
    std::string baseURIstring = base ? base->getLexicalValue() : "";
    std::string t = libwww::HTParse(s, &baseURIstring, libwww::PARSE_all); // !! maybe with PARSE_less ?
    return F.getURI(t.c_str());
}
void validateBase(const std::vector<std::string>& values, const sw::POS** setMe, const sw::POS* copySource, const char* argName) {
    const std::string& s = po::validators::get_single_string(values);
    if (s == "?") {
	std::cout << argName << "URI: " << (*setMe ? (*setMe)->getLexicalValue() : "\"\"") << "\n";
    } else {
	*setMe = 
	    (s == ".") ? CwdURI : 
	    (s == ":") ? copySource : 
	    htparseWrapper(s, *setMe);
	if (Debug > 0)
	    std::cout << "setting " << argName << " URI to " << (*setMe)->getLexicalValue() << "\n";
    }
}

/* Overload of relURI to validate --base arguments. */
struct baseURI : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, baseURI*, int)
{
    validateBase(values, &BaseURI, ArgBaseURI, "base");
    for (size_t i = 0; i < sizeof(Parsers)/sizeof(Parsers[0]); ++i)
	Parsers[i]->setBase(BaseURI->getLexicalValue());
}

/* Overload of relURI to validate --arg-base arguments. */
struct argBaseURI : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, argBaseURI*, int)
{
    validateBase(values, &ArgBaseURI, BaseURI, "argument base");
}

/* Overload of relURI to validate --output arguments. */
struct outPut : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, outPut*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    const sw::POS* abs(htparseWrapper(s, ArgBaseURI));
    Output = loadEntry(NULL, abs, BaseURI);
    if (Debug > 0)
	std::cout << "Sending output to " << abs->getLexicalValue() << BaseUriMessage() << "\n";
}

/* Overload of relURI to validate --in-place arguments. */
struct inPlace : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, inPlace*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    if (s == ".") {
	InPlace = true;
	if (Debug > 0)
	    std::cout << "Manipulating other input data.\n";
    } else {
	const sw::POS* abs(htparseWrapper(s, ArgBaseURI));
	LoadList.push_back(loadEntry(NULL, abs, BaseURI));
	Output = loadEntry(NULL, abs, BaseURI);
	if (Debug > 0)
	    std::cout << "Replacing data from " << abs->getLexicalValue() << " with base URI " << BaseURI->getLexicalValue() << "\n";
    }
}

/* Overload of relURI to validate --data arguments. */
struct dataURI : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, dataURI*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    const sw::POS* abs(htparseWrapper(s, ArgBaseURI));
    LoadList.push_back(loadEntry(NULL, abs, BaseURI));
    if (Debug > 0)
	std::cout << "reading default graph from " << abs->getLexicalValue() << " with base URI " << BaseURI->getLexicalValue() << "\n";
}

/* Overload of relURI to validate --graph arguments. */
struct graphURI : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, graphURI*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    NamedGraphName = s == "." ? F.getURI(".") : htparseWrapper(s, ArgBaseURI);
}

/* Overload of relURI to validate --graph 2nd args, query and map arguments. */
struct orderedURI : public relURI {};
void validate (boost::any&, const std::vector<std::string>& values, orderedURI*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    const sw::POS* vald = htparseWrapper(s, ArgBaseURI);
    if (NamedGraphName != NULL) {
	if (NamedGraphName->getLexicalValue() == ".")
	    NamedGraphName = vald;
	LoadList.push_back(loadEntry(NamedGraphName, vald, BaseURI));
	if (Debug > 0)
	    std::cout << "reading named graph " << NamedGraphName->getLexicalValue()
		      << " from " << vald->getLexicalValue()
		      << BaseUriMessage() << "\n";
	NamedGraphName = NULL;
    } else if (Query == NULL) {
	if (Debug > 0)
	    std::cout << "query resource: " << vald->getLexicalValue() << "\n";
	Query = vald;
    } else {
	if (Debug > 0)
	    std::cout << "view: " << vald->getLexicalValue() << "\n";
	Maps.push_back(vald);
    }
}

/* Set UserName when parsed. */
struct userName {};
void validate (boost::any&, const std::vector<std::string>& values, userName*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    UserName = s;
}
/* Set Password when parsed. */
struct passWord {};
void validate (boost::any&, const std::vector<std::string>& values, passWord*, int)
{
    const std::string& s = po::validators::get_single_string(values);
    PassWord = s;
}
/* Assign an HTTP header when parsed. */
struct HeaderPair {
    std::string name;
    std::string value;
    HeaderPair(std::string name, std::string value) : name(name), value(value) {  }
};
HeaderPair parseHeaderPair (const std::vector<std::string>& values)
{
    const std::string& s = po::validators::get_single_string(values);
    size_t pos = s.find_first_of(':');
    if (pos == std::string::npos)
	throw boost::program_options::validation_error(std::string("no ':' found in HTTP header pair \"").append(s).append("\""));
    return HeaderPair(s.substr(0, pos), s.substr(pos+2));
}
struct headerAssign {};
void validate (boost::any&, const std::vector<std::string>& values, headerAssign*, int)
{
    HeaderPair pair = parseHeaderPair(values);
    HTTPHeaders[pair.name] = pair.value;
}
struct headerAppend {};
void validate (boost::any&, const std::vector<std::string>& values, headerAppend*, int)
{
    HeaderPair pair = parseHeaderPair(values);
    HTTPHeaders[pair.name].append(", ").append(pair.value);
}


std::string adjustPath (std::string nameStr) {
    if (nameStr.substr(0, 7) == "file://") {
	size_t slash = nameStr.find_first_of('/', 7);
	nameStr = nameStr.substr(slash);
    }
    return nameStr;
}

sw::Operation* parseQuery (const sw::POS* query) {
    std::string querySpec = query->getLexicalValue();
    sw::IStreamPtr::e_opts opts = 
	(dynamic_cast<const sw::RDFLiteral*>(query) != NULL) ? 
	sw::IStreamPtr::STRING : 
	sw::IStreamPtr::STDIN;
    sw::IStreamPtr iptr(querySpec, opts, &Agent, &DebugStream);
    if (SparqlParser.parse_stream(&iptr) != 0)
	throw std::string("error when parsing query ").append(querySpec);
    return SparqlParser.root;
}

int main(int ac, char* av[])
{
    int ret = 0; /* no errors */
    sw::BoxChars::GBoxChars = &sw::BoxChars::AsciiBoxChars;
    try {
	CwdURI = 
	    F.getURI(std::string("file://localhost")
		     .append(fs::current_path().string())
		     .append("/"));

        /* General options -- cli-only. */
        po::options_description generalOpts("General options");
        generalOpts.add_options()
            ("help,h", "brief help message")    
            ("Help,H", 
	     po::value< std::vector<std::string> >()->composing(), 
	     "general, results, uri, query, data, http, sql, tutorial, all")
            ("debug", po::value<debugLevel>(), 
	     "debugging level")
            ("no-exec,n", "don't execute")
            ("print,p", "print final query")
            ("quiet,q", "quiet")
            ("version,v", "print version string")
	    ;

	/* rest: cli appends cfg file. */

        po::options_description resultsOpts("");
        resultsOpts.add_options()
            ("ascii,a", "output ASCII-only")
            ("utf-8,u", "output utf-8")
            ("bold", "output bold-bordered utf-8 boxes")
            ("nullterm,0", "terminate lines with \0")
            ("compare", po::value<std::string>(), 
	     "compare to some expected results.")
            ;
    
        po::options_description uriOpts("URI resolution");
        uriOpts.add_options()
            ("base,b", po::value<baseURI>(), 
	     "base URI for data/query resoultion")
            ("arg-base,B", 
	     po::value<argBaseURI>()->composing(), 
	     "base URI for command line arguments")
            ;

        po::options_description dataOpts("Reading data");
        dataOpts.add_options()
            ("data,d", po::value<dataURI>(), 
	     "read default graph from arg or stdin")
            ("graph,g", po::value<graphURI>(), 
	     "URI  read named graph <arg> from <URI> or stdin")
            ("language-name,l", po::value<langName>(), 
	     "data language\n\"guess\" to guess by resource extension, or \"-\" for stdin")
            ("lang-media-type,L", po::value<langType>(), 
	     "data language\n\"guess\" to guess by resource extension, or \"-\" for stdin")
            ("output,o", po::value<outPut>(), 
	     "send results to relURI or \"-\" for stdout.")
            ("in-place,i", po::value<inPlace>(), 
	     "update arg with the results.\n\"-\" to read from stdin and write to stdout, \".\" manipulate input graphs.")
            ("description,D", 
	     "read application description graph (see section) into default graph.")
            ("desc-graph,G", po::value<std::vector <std::string> >(), 
	     "read application description graph into graph arg.")
            ;
    
        po::options_description httpOpts("HTTP options");
        httpOpts.add_options()
            ("username,u", po::value<userName>(), 
	     "username for HTTP transactions")
            ("password,u", po::value<passWord>(), 
	     "password for HTTP transactions")
            ("header", po::value<headerAssign>(), 
	     "assign a header value.\n"
	     "\"--header ’Accept: text/turtle,text/html;q=.8’\" prefers turtle over HTML.")
            ("header+", po::value<headerAppend>(), 
	     "append earlier value of header.")
            ;

        po::options_description sqlOpts("SQL options");
        sqlOpts.add_options()
            ("stem,s", po::value<std::string>(), 
	     "stem URL.")
            ("sql-service,S", po::value<std::string>(), 
	     "odbc-style SQL database\n\tdriver://[username[:password]@]host[:port]/database\nmysql://localhost/orders")
            ("mapset,m", po::value<std::string>(), 
	     "mapset resource, which supplies above parameters.")
            ;

        /* Ordered options -- not shown with --help.
	 * Hack: 2nd arg to --graph is a orderedURI 'cause boost::po doesn't
	 * handle "--foo arg1 arg2".
	 */
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("exec,e", po::value<queryString>(), "queries")
            ("ordered", po::value<orderedURI>(), "URIs")
            ;

        po::options_description cmdline_options;
        cmdline_options.add(generalOpts).add(resultsOpts).add(uriOpts).add(dataOpts).add(httpOpts).add(sqlOpts).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(resultsOpts).add(uriOpts).add(dataOpts).add(httpOpts).add(sqlOpts).add(hidden);

        po::options_description visible("");
        visible.add(generalOpts).add(resultsOpts).add(uriOpts).add(httpOpts).add(sqlOpts).add(dataOpts);
        
        po::options_description cursory("");
        cursory.add(generalOpts).add(resultsOpts).add(uriOpts).add(dataOpts);
        
        po::positional_options_description p;
        p.add("ordered", -1);
        
        po::variables_map vm;
	po::store(po::command_line_parser(ac, av).
              options(cmdline_options).positional(p).run(), vm);

	std::ifstream ifs(".SPARQL.cfg");
	po::store(parse_config_file(ifs, config_file_options), vm);
	po::notify(vm);
    
        if (vm.count("utf-8")) {
	    if (Debug > 0)
		std::cout << "Switching to utf-8.\n";
	    sw::BoxChars::GBoxChars = &sw::BoxChars::Utf8BoxChars;
        }

        if (vm.count("ascii")) {
	    if (Debug > 0)
		std::cout << "Switching to ASCII.\n";
	    sw::BoxChars::GBoxChars = &sw::BoxChars::AsciiBoxChars;
        }

        if (vm.count("bold")) {
	    if (Debug > 0)
		std::cout << "Switching to bold-bordered boxes.\n";
	    sw::BoxChars::GBoxChars = &sw::BoxChars::Utf8BldChars;
        }

        if (vm.count("no-exec")) {
	    if (Debug > 0)
		std::cout << "Execution suppressed.\n";
            NoExec = true;
        }

        if (vm.count("quiet")) {
	    if (Debug > 0)
		std::cout << "Non-debug messages supressed.\n";
            Quiet = true;
        }

	static const char* queryHelp = "Queries and maps:\n"
	    "  <queryURI>            read and execute a query from <queryURI>.\n"
	    "  -e <query>            execute <query>.\n"
	    "  <mapURI>              map query through <mapURI> before executing.\n";
	static const char* appDescGraph = 
	    "    @prefix doap: <http://usefulinc.com/ns/doap#> .\n"
	    "    <> a doap:Project ;\n"
            "        doap:homepage <http://swobj.org/SPARQL/v1> ;\n"
            "        doap:shortdesc \"a semantic web query toolbox\" .\n";
	static const char* tutorial = 
	    "Tutorial:\n"
	    "  The SPARQL executable contains a short description of itself, with the\n"
	    "  following assertions:\n"
	    "    SPARQL is a doap project.\n"
	    "    SPARQL has a homepage http://swobj.org/SPARQL/v1 .\n"
	    "    SPARQL is \"a Semantic Web query toolbox\".\n"
	    "  You can load this description (-D) and query it:\n"
	    "    SPARQL -D -e \"SELECT ?s ?p ?o WHERE {?s ?p ?o}\"\n"
	    "  Giving resuls like:\n"
	    "    +----+-------------------------------+----------------------------------------+\n"
	    "    | ?s | ?p                            | ?o                                     |\n"
	    "    | <> | <http://www.w3....ax-ns#type> | <http://usefulinc.com/ns/doap#Project> |\n"
	    "    | <> |           <http...p#homepage> |           <http://swobj.org/SPARQL/v1> |\n"
	    "    | <> |          <http:...#shortdesc> |         \"a semantic web query toolbox\" |\n"
	    "    +----+-------------------------------+----------------------------------------+\n"
	    "\n"
	    "-D loaded the three assertions into the \"default graph\", the graph to which\n"
	    "SPARQL queries are directed unless otherwise directed by GRAPH clause. To try a\n"
	    "GRAPH clause, load the description into the graph foo and query that graph:\n"
	    "    SPARQL -G foo -e \"SELECT ?s ?p ?o WHERE { GRAPH <foo> { ?s ?p ?o } }\"\n"
	    "\n"
	    "We can load the description into a couple graphs *and* the default graph and ask\n"
	    "all of the graphs which include an assertion about a doap project:\n"
	    "    SPARQL -a -DG foo -G foo2 -e \"SELECT ?g {GRAPH ?g{?s ?p <http://usefulinc.com/ns/doap#Project>}}\"\n"
	    "    +--------+\n"
	    "    | ?g     |\n"
	    "    |  <foo> |\n"
	    "    | <foo2> |\n"
	    "    +--------+\n"
	    "Note that the default graph did not appear as the query only matches loaded\n"
	    "*named* graphs. You can use a UNION to match both the default graph and loaded\n"
	    "named graphs:\n"
	    "    ./SPARQL -a -DG foo -G foo2 -e \"SELECT ?g {{?s ?p <http://usefulinc.com/ns/doap#Project>}\n"
	    "UNION {GRAPH ?g{?s ?p <http://usefulinc.com/ns/doap#Project>}}}\"\n"
	    "    +--------+\n"
	    "    | ?g     |\n"
	    "    |     -- |\n"
	    "    |  <foo> |\n"
	    "    | <foo2> |\n"
	    "    +--------+\n"
	    ;

        if (vm.count("help")) {
            std::cout << 
		"Usage: SPARQL [opts] queryURI mapURI*\n"
		"Usage: SPARQL [opts] -e query mapURI*\n\n"
		"get started with: SPARQL --Help tutorial\n" << 
		queryHelp << cursory;
            NoExec = true;
        }

        if (vm.count("Help")) {
	    std::vector<std::string> helps(vm["Help"].as< std::vector<std::string> >());
	    for (std::vector<std::string>::const_iterator it = helps.begin();
		 it != helps.end(); ++it) {
		std::string nl = "\n"; // !it->compare("all") ? "\n" : "";
		bool matched = false;
		if (!it->compare("general") || !it->compare("all"))
		    matched = true, std::cout << generalOpts << nl;
		if (!it->compare("results") || !it->compare("all"))
		    matched = true, std::cout << resultsOpts << nl;
 		if (!it->compare("uri") || !it->compare("all"))
		    matched = true, std::cout << uriOpts << nl;
 		if (!it->compare("query") || !it->compare("all"))
		    matched = true, std::cout
			<< queryHelp
			<<  nl;
		if (!it->compare("data") || !it->compare("all"))
		    matched = true, std::cout
			<< dataOpts
			<< "SPARQL will \"guess\" that data from ’-’ is trig and output is either a table or trig.\n\n"
			<< "Application description graph:\n" << appDescGraph
			<< nl;
 		if (!it->compare("http") || !it->compare("all"))
		    matched = true, std::cout << httpOpts << nl;
 		if (!it->compare("sql") || !it->compare("all"))
		    matched = true, std::cout << sqlOpts << nl;
 		if (!it->compare("tutorial") || !it->compare("all"))
		    matched = true, std::cout << tutorial << nl;
		if (matched == false)
		    std::cout << "Unknown help topic: " << *it << "\n";
	    }
            NoExec = true;
        }


        if (vm.count("version"))
            std::cout << "SPARQL version 1.0, revision: $Id$\n";
	else if (NoExec == false) {
	    if (vm.count("description")) {
		sw::IStreamPtr s(appDescGraph, sw::StreamPtr::STRING);
		s.mediaType = "text/turtle";
		Db.loadData(Db.assureGraph(sw::DefaultGraph), &s, "", &F); // !!! baseURI
	    }

	    if (vm.count("desc-graph")) {
		std::vector<std::string> descs(vm["desc-graph"].as< std::vector<std::string> >());
		for (std::vector<std::string>::const_iterator it = descs.begin();
		     it != descs.end(); ++it) {
		    sw::IStreamPtr s(appDescGraph, sw::StreamPtr::STRING);
		    s.mediaType = "text/turtle";
		    Db.loadData(Db.assureGraph(F.getURI(*it)), &s, *it, &F); // !!! baseURI
		}
	    }

	    for (loadList::iterator it = LoadList.begin();
		 it != LoadList.end(); ++it)
		it->loadGraph();

	    if (Debug > 0)
		std::cout << "<loadedData>\n" << Db << "</loadedData>\n";

	    if (Query == NULL) {
		if (Maps.begin() != Maps.end())
		    throw std::string("Mapping rules found with no query to map.");
	    } else {
		sw::Operation* query = parseQuery(Query);

		sw::QueryMapper queryMapper(&F, &DebugStream);
		for (mapList::const_iterator it = Maps.begin(); it != Maps.end(); ++it) {
		    sw::Operation* rule = parseQuery(*it);
		    sw::Construct* c;
		    if ((c = dynamic_cast<sw::Construct*>(rule)) == NULL)
			throw std::string("Rule file ").append(": ").
			    append((*it)->getLexicalValue()).
			    append(" was not a SPARQL CONSTRUCT");
		    queryMapper.addRule(c);
		    delete rule;
		}

		const sw::Operation* o;
		if (queryMapper.getRuleCount() > 0) {
		    if (DebugStream != NULL)
			*DebugStream << "Transforming user query by applying " << queryMapper.getRuleCount() << " rule maps." << std::endl;
		    query->express(&queryMapper);
		    o = queryMapper.last.operation;
		    delete query;
		} else
		    o = query;

		sw::RdfDB constructed;
		sw::ResultSet rs(&F); // !!! , &constructed overrides the query database
		rs.setRdfDB(InPlace ? &Db : &constructed);
		o->execute(&Db, &rs);
		if (vm.count("compare")) {
		    const sw::POS* cmp = htparseWrapper(vm["compare"].as<std::string>(), ArgBaseURI);
		    sw::IStreamPtr iptr(cmp->getLexicalValue(), 
					sw::IStreamPtr::NONE, 
					&Agent, &DebugStream);

		    sw::ResultSet* reference;
		    if (iptr.mediaType == "application/sparql-results+xml") {
			reference = new sw::ResultSet(&F, &P, *iptr);
		    } else {
			sw::RdfDB resGraph;
			if (iptr.mediaType == "text/turtle") {
			    TurtleParser.setGraph(resGraph.assureGraph(NULL));
			    TurtleParser.parse_stream(&iptr);
			    TurtleParser.clear("");
			} else {
			    throw std::string("media-type \"").append(iptr.mediaType).append("\" unknown.");
			}
			reference = 
			    rs.resultType == sw::ResultSet::RESULT_Graphs ?
			    new sw::ResultSet(&F, &resGraph) :
			    new sw::ResultSet(&F, &resGraph, "");
		    }
		    if (!(rs == *reference)) {
			if (!Quiet)
			    std::cout << rs << "!=\n" << *reference << "\n";
			ret = 1;
		    } else {
			if (!Quiet)
			    std::cout << "matched\n";
			ret = 0;
		    }
		    delete reference;
		} else {
		    if (rs.resultType == sw::ResultSet::RESULT_Graphs)
			std::cout << (InPlace ? Db.toString() : constructed.toString());
		    else
			std::cout << rs.toString();
		}
	    }
	    if (Output.resource != NULL) {
		std::string outres = Output.resource->getLexicalValue();
		sw::OStreamPtr optr(outres, sw::OStreamPtr::STDIN, 
				    &Agent, &DebugStream);
		if (optr.mediaType == "text/turtle") {
		    std::string str(Db.assureGraph(NULL)->toString());
		    *optr << str;
		} else if (optr.mediaType == "text/trig") {
		    std::string str(Db.toString());
		    *optr << str;
		}
	    }
	}
    } catch(std::exception& e) {
        std::cout << e.what() << "\n";
        ret = 2;
    } catch(std::string& e) {
        std::cout << e << "\n";
        ret = 2;
    }    

    return ret;
}