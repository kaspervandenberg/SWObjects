/* test_RdfXmlParser.cpp - test RdfXmlParser.hpp
 *
 * $Id: test_RdfXmlParser.cpp,v 1.5 2008-12-04 22:37:09 eric Exp $
 */

/* test graph-matching.
 *
 */

#define BOOST_TEST_MODULE RdfXmlParser

#include <map>
#include <vector>
#include "SWObjects.hpp"
#include "TurtleSParser/TurtleSParser.hpp"
#include "RdfXmlParser.hpp"

#if XML_PARSER == SWOb_LIBXML2
  #include "../interface/SAXparser_libxml.hpp"
  w3c_sw::SAXparser_libxml P;
#elif XML_PARSER == SWOb_EXPAT1
  #include "../interface/SAXparser_expat.hpp"
  w3c_sw::SAXparser_expat P;
#elif XML_PARSER == SWOb_MSXML3
  #include "../interface/SAXparser_msxml3.hpp"
  w3c_sw::SAXparser_msxml3 P;
#else
  #warning DAWG tests require an XML parser
#endif

#if HTTP_CLIENT == SWOb_ASIO
  #include "../interface/WEBagent_boostASIO.hpp"
  w3c_sw::WEBagent_boostASIO WebClient;
#else /* ! HTTP_CLIENT == SWOb_ASIO */
  #warning unable to test RdfXml over HTTP
#endif /* ! HTTP_CLIENT == SWOb_ASIO */

/* Keep all inclusions of boost *after* the inclusion of SWObjects.hpp
 * (or define BOOST_*_DYN_LINK manually).
 */
#include <boost/test/unit_test.hpp>

using namespace w3c_sw;

POSFactory F;
TurtleSDriver turtleParser("", &F);
RdfXmlParser GRdfXmlParser(&F, &P);

BOOST_AUTO_TEST_CASE( Ds_po ) {
    DefaultGraphPattern tested;
    IStreamPtr rdfxml("RdfXmlParser/Ds_po.rdf");
    GRdfXmlParser.parse(&tested, rdfxml);

    DefaultGraphPattern expected;
    turtleParser.setGraph(&expected);
    turtleParser.setBase("Ds_po.rdf");
    IStreamPtr reference("RdfXmlParser/Ds_po.ttl", StreamPtr::FILE);
    turtleParser.parse(reference);
    turtleParser.clear(""); // clear out namespaces and base URI.
    BOOST_CHECK_EQUAL(tested, expected);
}

BOOST_AUTO_TEST_CASE( Ds_Do ) {
    DefaultGraphPattern tested;
    IStreamPtr rdfxml("RdfXmlParser/Ds_Do.rdf");
    GRdfXmlParser.parse(&tested, rdfxml);

    DefaultGraphPattern expected;
    turtleParser.setGraph(&expected);
    turtleParser.setBase("Ds_Do.rdf");
    IStreamPtr reference("RdfXmlParser/Ds_Do.ttl", StreamPtr::FILE);
    turtleParser.parse(reference);
    turtleParser.clear(""); // clear out namespaces and base URI.
    BOOST_CHECK_EQUAL(tested, expected);
}

/* invoke with e.g. -DHTTP_RdfXml_test=http://mouni.local/RdfXml-0.rdf */
#if HTTP_CLIENT != SWOb_DISABLED && defined(HTTP_RdfXml_test)
BOOST_AUTO_TEST_CASE( by_http ) {
    DefaultGraphPattern tested;
    std::string s(WebClient.get(HTTP_RdfXml_test));
    BOOST_CHECK_EQUAL(WebClient.getMediaType().substr(0, 9), "application/rdf+xml");
    GRdfXmlParser.parse(&tested, s.begin(), s.end(), HTTP_RdfXml_test);

    DefaultGraphPattern expected;
    turtleParser.setGraph(&expected);
    turtleParser.setBase(F.getURI(HTTP_RdfXml_test));
    turtleParser.parse_file("RdfXml-0.ttl");
    turtleParser.clear(""); // clear out namespaces and base URI.
    BOOST_CHECK_EQUAL(tested, expected);
}
#endif /* HTTP_CLIENT != SWOb_DISABLED */

