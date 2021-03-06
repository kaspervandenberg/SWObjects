/* test_SPARQL.cpp - test the ../bin/SPARQL executable
 *
 * $Id: test_SPARQL.cpp,v 1.5 2008-12-04 22:37:09 eric Exp $
 */

#include <iostream>
#include <fstream>
#include "SWObjects.hpp"
#include "ResultSet.hpp"

#define BOOST_TEST_MODULE SPARQL
#include <boost/test/unit_test.hpp>

w3c_sw::POSFactory F;

const char* Doutput =
    "+----+---------------------------------------------------+----------------------------------------+\n"
    "| ?S | ?P                                                | ?O                                     |\n"
    "| <> |           <http://usefulinc.com/ns/doap#homepage> |           <http://swobj.org/SPARQL/v1> |\n"
    "| <> | <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> | <http://usefulinc.com/ns/doap#Project> |\n"
    "| <> |          <http://usefulinc.com/ns/doap#shortdesc> |         \"a semantic web query toolbox\" |\n"
    "+----+---------------------------------------------------+----------------------------------------+\n";

struct ExecResults {
    std::string s;
    ExecResults (const char* cmd) {
	s  = "execution failure";
	FILE *p = ::popen(cmd, "r");
	BOOST_REQUIRE(p != NULL);
	char buf[100];
	s = "";

	/* Gave up on [[ ferror(p) ]] because it sometimes returns EPERM on OSX.
	 */
	for (size_t count; (count = fread(buf, 1, sizeof(buf), p)) || !feof(p);)
	    s += std::string(buf, buf + count);
	pclose(p);
    }
};

bool operator== (ExecResults& tested, std::string& ref) {
    return tested.s == ref;
}

std::ostream& operator== (std::ostream& o, ExecResults& tested) {
    return o << tested.s;
}

BOOST_AUTO_TEST_SUITE( tutorial )
BOOST_AUTO_TEST_CASE( D ) {
    ExecResults tested("../bin/SPARQL -D");
    BOOST_CHECK_EQUAL(tested.s, 
		      "{\n"
		      "  <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://usefulinc.com/ns/doap#Project> .\n"
		      "  <> <http://usefulinc.com/ns/doap#homepage> <http://swobj.org/SPARQL/v1> .\n"
		      "  <> <http://usefulinc.com/ns/doap#shortdesc> \"a semantic web query toolbox\"  .\n"
		      "}\n");
}
BOOST_AUTO_TEST_CASE( D_trig ) {
    ExecResults tested("../bin/SPARQL -D -L text/trig");
    BOOST_CHECK_EQUAL(tested.s, 
		      "{\n"
		      "  <> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://usefulinc.com/ns/doap#Project> .\n"
		      "  <> <http://usefulinc.com/ns/doap#homepage> <http://swobj.org/SPARQL/v1> .\n"
		      "  <> <http://usefulinc.com/ns/doap#shortdesc> \"a semantic web query toolbox\"  .\n"
		      "}\n");
}
BOOST_AUTO_TEST_CASE( D_turtle ) {
    ExecResults tested("../bin/SPARQL -D -L text/turtle");
    BOOST_CHECK_EQUAL(tested.s, 
		      "<> <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> <http://usefulinc.com/ns/doap#Project> .\n"
		      "<> <http://usefulinc.com/ns/doap#homepage> <http://swobj.org/SPARQL/v1> .\n"
		      "<> <http://usefulinc.com/ns/doap#shortdesc> \"a semantic web query toolbox\" .\n");
}
BOOST_AUTO_TEST_CASE( D_spo ) {
    ExecResults invocation("../bin/SPARQL -D -e \"SELECT ?s ?p ?o WHERE {?s ?p ?o}\"");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+----+---------------------------------------------------+----------------------------------------+\n"
		 "| ?s | ?p                                                | ?o                                     |\n"
		 "| <> | <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> | <http://usefulinc.com/ns/doap#Project> |\n"
		 "| <> |           <http://usefulinc.com/ns/doap#homepage> |           <http://swobj.org/SPARQL/v1> |\n"
		 "| <> |          <http://usefulinc.com/ns/doap#shortdesc> |         \"a semantic web query toolbox\" |\n"
		 "+----+---------------------------------------------------+----------------------------------------+\n",
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
}
BOOST_AUTO_TEST_CASE( D_spo_utf8 ) {
    ExecResults invocation("../bin/SPARQL -D -8 -e \"SELECT ?s ?p ?o WHERE {?s ?p ?o}\"");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+----+---------------------------------------------------+----------------------------------------+\n"
		 "| ?s | ?p                                                | ?o                                     |\n"
		 "| <> | <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> | <http://usefulinc.com/ns/doap#Project> |\n"
		 "| <> |           <http://usefulinc.com/ns/doap#homepage> |           <http://swobj.org/SPARQL/v1> |\n"
		 "| <> |          <http://usefulinc.com/ns/doap#shortdesc> |         \"a semantic web query toolbox\" |\n"
		 "+----+---------------------------------------------------+----------------------------------------+\n",
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
}
BOOST_AUTO_TEST_CASE( G_spo ) {
    ExecResults invocation("../bin/SPARQL -G foo -e \"SELECT ?s ?p ?o WHERE { GRAPH <foo> { ?s ?p ?o } }\"");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+----+---------------------------------------------------+----------------------------------------+\n"
		 "| ?s | ?p                                                | ?o                                     |\n"
		 "| <> | <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> | <http://usefulinc.com/ns/doap#Project> |\n"
		 "| <> |           <http://usefulinc.com/ns/doap#homepage> |           <http://swobj.org/SPARQL/v1> |\n"
		 "| <> |          <http://usefulinc.com/ns/doap#shortdesc> |         \"a semantic web query toolbox\" |\n"
		 "+----+---------------------------------------------------+----------------------------------------+\n", 
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
}
BOOST_AUTO_TEST_CASE( DG_sp ) {
    ExecResults invocation("../bin/SPARQL -a -DG foo -G foo2 -e \"SELECT ?g {\n"
		       "    GRAPH ?g {?s ?p <http://usefulinc.com/ns/doap#Project>}}\"");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+--------+\n"
		 "| ?g     |\n"
		 "|  <foo> |\n"
		 "| <foo2> |\n"
		 "+--------+\n", 
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
}
BOOST_AUTO_TEST_CASE( DG_sp_U_sp ) {
    ExecResults invocation("../bin/SPARQL -a -DG foo -G foo2 -e \"SELECT ?g {\n"
			   "        {?s ?p <http://usefulinc.com/ns/doap#Project>}\n"
			   "    UNION\n"
			   "        {GRAPH ?g{?s ?p <http://usefulinc.com/ns/doap#Project>}}}\"\n");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+--------+\n"
		 "| ?g     |\n"
		 "|     -- |\n"
		 "|  <foo> |\n"
		 "| <foo2> |\n"
		 "+--------+\n", 
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
}
BOOST_AUTO_TEST_SUITE_END(/* tutorial */)

#ifdef FIXED_SPARQL_ARGS_ORDER // !!!
/* sensitivity to position of -b directive */
BOOST_AUTO_TEST_CASE( Dbe ) {
    ExecResults tested("../bin/SPARQL -D -b http://foo.example/ -e \"SELECT * WHERE { <> a ?t}\"");
    BOOST_CHECK_EQUAL(tested.s, 
		      "+\n"
		      "|\n"
		      "+\n");
}
#endif

/* make sure we fail mis-matches */
BOOST_AUTO_TEST_CASE( triple_match__dawg_triple_pattern_001_002 ) {
    ExecResults tested("../bin/SPARQL -d data-r2/triple-match/data-01.ttl data-r2/triple-match/dawg-tp-01.rq --compare data-r2/triple-match/result-tp-02.ttl");
    BOOST_CHECK_EQUAL(tested.s, 
		      "+-----------------------------+------------------------------+\n"
		      "| ?p                          | ?q                           |\n"
		      "| <http://example.org/data/p> | <http://example.org/data/v1> |\n"
		      "| <http://example.org/data/p> | <http://example.org/data/v2> |\n"
		      "+-----------------------------+------------------------------+\n"
		      "!=\n"
		      "+------------------------------+-----------------------------+\n"
		      "| ?q                           | ?x                          |\n"
		      "| <http://example.org/data/v2> | <http://example.org/data/x> |\n"
		      "| <http://example.org/data/v1> | <http://example.org/data/x> |\n"
		      "+------------------------------+-----------------------------+\n"
		      "\n");
}

BOOST_AUTO_TEST_CASE( triple_match__dawg_triple_pattern_001 ) {
    ExecResults tested("../bin/SPARQL -d data-r2/triple-match/data-01.ttl data-r2/triple-match/dawg-tp-01.rq --compare data-r2/triple-match/result-tp-01.ttl");
    BOOST_CHECK_EQUAL(tested.s, 
		      "matched\n");
}

BOOST_AUTO_TEST_CASE( bool_no_base ) {
    ExecResults tested("../bin/SPARQL -b '' -d SPARQL/rel.ttl SPARQL/rel.rq");
    BOOST_CHECK_EQUAL(tested.s, 
		      "true\n");
}

BOOST_AUTO_TEST_CASE( bool_base_0 ) {
    ExecResults tested("../bin/SPARQL -b http://foo.example/ -d SPARQL/rel.ttl SPARQL/rel.rq");
    BOOST_CHECK_EQUAL(tested.s, 
		      "true\n");
}

BOOST_AUTO_TEST_CASE( bool_base_1 ) {
    ExecResults tested("../bin/SPARQL -d SPARQL/rel.ttl -b http://foo.example/ SPARQL/rel.rq");
    BOOST_CHECK_EQUAL(tested.s, 
		      "false\n");
}

BOOST_AUTO_TEST_CASE( resultsFormat ) {
    w3c_sw::POS::String2BNode bnodeMap; // share, not used for these tests.
    {   /* Create an simple table dump. */
	ExecResults creation("../bin/SPARQL -D -e \"SELECT*{?S?P?O}\" -o SPARQL/Dt.srt\n");
	BOOST_CHECK_EQUAL(creation.s, "");

	/* Check that table dump. */
	ExecResults cat("../bin/SPARQL -d SPARQL/D.srt\n");
	w3c_sw::ResultSet cat_measured(&F, cat.s, false, bnodeMap);
	w3c_sw::ResultSet cat_expected(&F, Doutput, false, bnodeMap);
	BOOST_CHECK_EQUAL(cat_measured, cat_expected);
    }
 
    {   /* Create an SRX (SPARQL Xml Results format) */
	ExecResults creation("../bin/SPARQL -D -e \"SELECT*{?S?P?O}\" -o SPARQL/Dt.srx\n");
	BOOST_CHECK_EQUAL(creation.s, "");

	/* Check that SRX. */
	ExecResults cat("../bin/SPARQL -d SPARQL/D.srx\n");
	w3c_sw::ResultSet cat_measured(&F, cat.s, false, bnodeMap);
	w3c_sw::ResultSet
	    cat_expected(&F, Doutput, false, bnodeMap);
	BOOST_CHECK_EQUAL(cat_measured, cat_expected);
    }
 
    {
	ExecResults join("../bin/SPARQL -d SPARQL/D.srx -d SPARQL/E.srt\n");
	w3c_sw::ResultSet join_measured(&F, join.s, false, bnodeMap);
	w3c_sw::ResultSet
	    join_expected(&F, 
			  "+----+---------------------------------------------------+----------------------------------------+-----------------------------------------+\n"
			  "| ?S | ?P                                                | ?O                                     | ?O2                                     |\n"
			  "| <> | <http://www.w3.org/1999/02/22-rdf-syntax-ns#type> | <http://usefulinc.com/ns/doap#Project> | <http://usefulinc.com/ns/doap#Project2> |\n"
			  "| <> |          <http://usefulinc.com/ns/doap#shortdesc> |         \"a semantic web query toolbox\" |         \"a semantic web query toolbox2\" |\n"
			  "+----+---------------------------------------------------+----------------------------------------+-----------------------------------------+\n", 
			  false, bnodeMap);
	BOOST_CHECK_EQUAL(join_measured, join_expected);
    }
}

BOOST_AUTO_TEST_CASE( GRDDL0 ) {
    ::setenv("XSLT", "/usr/bin/xsltproc %STYLESHEET %DATA", 1);
    ExecResults invocation("../bin/SPARQL -d SPARQL/GRDDL0.html -e 'SELECT ?fam {?s <http://xmlns.com/foaf/0.1/family_name> ?fam}'");
    w3c_sw::POS::String2BNode bnodeMap;
    w3c_sw::ResultSet tested(&F, invocation.s, false, bnodeMap);
    w3c_sw::ResultSet
	expected(&F, 
		 "+-----------------+\n"
		 "| ?fam            |\n"
		 "| \"Prud'hommeaux\" |\n"
		 "+-----------------+\n", 
		 false, bnodeMap);
    BOOST_CHECK_EQUAL(tested, expected);
    ::unsetenv("XSLT");
}

