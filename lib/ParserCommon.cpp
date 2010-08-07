/* ParserCommon.cc - commonalities between SPARQL/RDF-related parsers
 * $Id: ParserCommon.cpp,v 1.3 2008-10-10 11:52:37 eric Exp $
 */

#include "SWObjects.hpp"
#include "location.hpp"
#include "ParserCommon.hpp"
#include <fstream>
#include <iostream> // for cerr

namespace w3c_sw {

    /* Base class for Yacc parsers. */
    YaccDriver::YaccDriver (AtomFactory* atomFactory)
	: ParserDriver(), atomFactory(atomFactory),
	  ignorePrefixFlag(false),
	  trace_scanning(false), trace_parsing(false) {  }

    YaccDriver::YaccDriver (std::string baseURI, AtomFactory* atomFactory)
	: ParserDriver(baseURI), atomFactory(atomFactory),
	  ignorePrefixFlag(false),
	  trace_scanning(false), trace_parsing(false) {  }

    void YaccDriver::error (const class location& l,
			const std::string& m) {
	std::stringstream ss;
	ss << l << ": " << m;
	throw ParserException(ParserLocation(l.begin.filename, l.begin.line, l.begin.column),
			      ParserLocation(l.end.filename, l.end.line, l.end.column), ss.str());
    }

    void YaccDriver::error (const std::string& m) {
	throw m;
    }

    const URI* YaccDriver::getAbsoluteURI (std::string name) {
	std::string abs(libwww::HTParse(name, &baseURI, libwww::PARSE_all));
	return atomFactory->getURI(abs.c_str());
    }

} //namespace w3c_sw

void stop (size_t line) {
    std::cerr << "should have stopped with a " << line << std::endl;
}

