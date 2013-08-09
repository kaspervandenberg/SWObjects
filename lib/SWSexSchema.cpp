/* RdfDB - sets of variable bindings and their proofs.
 * $Id: RdfDB.cpp,v 1.2 2008-08-27 02:21:41 eric Exp $
 */

#include "SWSexSchema.hpp"

namespace w3c_sw {

    std::ostream& SWSexSchema::CodeMap::print (std::ostream& os) const {
	for (const_iterator it = begin(); it != end(); ++it) {
	    os << " %" << it->first << "{" << it->second << "%}";
	}
	return os;
    }
    std::string SWSexSchema::CodeMap::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::OrRule::print (std::ostream& os, bool braces) const {
	if (braces)
	    os << '{';
	os << '(' << "\n    ";
	for (std::vector<const RulePattern*>::const_iterator it = rules.begin();
	     it != rules.end(); ++it) {
	    if (it != rules.begin())
		os << "\n  | ";
	    os << **it;
	}
	os << ')';
	if (braces)
	    os << '}';
	return os;
    }
    std::string SWSexSchema::OrRule::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AndRule::print (std::ostream& os, bool braces) const {
	if (braces)
	    os << '{';
	os << '(' << "\n    ";
	for (std::vector<const RulePattern*>::const_iterator it = rules.begin();
	     it != rules.end(); ++it) {
	    if (it != rules.begin())
		os << ",\n    ";
	    os << **it;
	}
	os << ')';
	if (braces)
	    os << '}';
	return os;
    }
    std::string SWSexSchema::AndRule::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::NegatedRule::print (std::ostream& os, bool braces) const {
	if (braces)
	    os << '{';
	os << '!' << *pat;
	if (braces)
	    os << '}';
	return os;
    }
    std::string SWSexSchema::NegatedRule::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::print (std::ostream& os, bool braces) const {
	if (braces)
	    os << '{';
	os << *nameClass << ' ' << *value << "{" << min << ',';
	if (max != Unlimited)
	    os << max;
	os << '}';
	if (braces)
	    os << '}';
	return os;
    }
    std::string SWSexSchema::AtomicRule::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::NameTerm::print (std::ostream& os) const {
	if (reverse)
	    os << '^';
	return os << term->str();
    }
    std::string SWSexSchema::AtomicRule::NameTerm::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::NameAll::print (std::ostream& os) const {
	if (reverse)
	    os << '^';
	return os << '*';
    }
    std::string SWSexSchema::AtomicRule::NameAll::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::NamePattern::print (std::ostream& os) const {
	if (reverse)
	    os << '^';
	return os << base->str() << '*';
    }
    std::string SWSexSchema::AtomicRule::NamePattern::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::ValueReference::print (std::ostream& os) const {
	return os << '@' << label->str();
    }
    std::string SWSexSchema::AtomicRule::ValueReference::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::ValueType::print (std::ostream& os) const {
	return os << tterm->str();
    }
    std::string SWSexSchema::AtomicRule::ValueType::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::ValueSet::print (std::ostream& os) const {
	os << '(';
	for (std::vector<const TTerm*>::const_iterator it = tterms.begin();
	     it != tterms.end(); ++it) {
	    if (it != tterms.begin())
		os << ' ';
	    os << (*it)->str();
	}
	return os << ')';
    }
    std::string SWSexSchema::AtomicRule::ValueSet::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::AtomicRule::ValueAny::print (std::ostream& os) const {
	return os << '-';
    }
    std::string SWSexSchema::AtomicRule::ValueAny::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::RuleActions::print (std::ostream& os, bool braces) const {
	if (braces)
	    os << '{';
	os << *pattern;
	if (braces)
	    os << '}';
	return os << codeMap;
    }
    std::string SWSexSchema::RuleActions::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::RuleMap::print (std::ostream& os, const TTerm* start) const {
	bool first = true;
	if (start != NULL) {
	    const_iterator it = find(start);
	    if (it != end()) {
		first = false;
		os << it->first->str() << ' ';
		it->second->print(os, true);	    
	    }
	}
	typedef std::vector<const TTerm*> Sorted;
	Sorted v;
	for (const_iterator it = begin(); it != end(); ++it)
	    v.push_back(it->first);
	std::sort(v.begin(), v.end(), TTermSorter());
	for (Sorted::const_iterator it = v.begin(); it != v.end(); ++it) {
	    if (*it == start)
		continue;
	    if (!first)
		os << "\n\n";
	    first = false;
	    os << (*it)->str() << ' ';
	    find(*it)->second->print(os, true);
	}
	return os;
    }
    std::string SWSexSchema::RuleMap::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }
    std::ostream& SWSexSchema::print (std::ostream& os) const {
	if (start != NULL)
	    os << "start=" << start->str() << "\n";
	ruleMap.print(os, start);
	return os;
    }
    std::string SWSexSchema::str () const {
	std::stringstream ss; print(ss); return ss.str();
    }

    const TTerm* SWSexSchema::AtomicRule::NameTerm::getP () const {
	return term;
    }
    bool SWSexSchema::AtomicRule::NameTerm::matchP (const TTerm* test) const {
	return term == test;
    }
    const TTerm* SWSexSchema::AtomicRule::NameAll::getP () const {
	return NULL;
    }
    bool SWSexSchema::AtomicRule::NameAll::matchP (const TTerm* test) const {
	return true;
    }
    const TTerm* SWSexSchema::AtomicRule::NamePattern::getP () const {
	return NULL;
    }
    bool SWSexSchema::AtomicRule::NamePattern::matchP (const TTerm* test) const {
	throw NotImplemented("matchP on * !rdf:* !dc:*");
    }

    SWSexSchema::AtomicRule::Value::TermSet SWSexSchema::AtomicRule::ValueReference::getOs () const {
	SWSexSchema::AtomicRule::Value::TermSet ret;
	ret.push_back(NULL);
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::AtomicRule::ValueReference::validateTriple (const w3c_sw::SWSexSchema::AtomicRule* rule, const TriplePattern* tp, DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	// !!! also record the current matched from tp
	RuleMap::const_iterator referencedRule = rm.find(label);
	if (referencedRule == rm.end())
	    throw std::runtime_error("referenced production not found"); // not found
	SWSexSchema::Solution* s = referencedRule->second->validate(source, matched, rm, tp->getO());
	if (s->passes) {
	    source.erase(source.find(tp));
	    matched.addTriplePattern(tp);
	}
	return s; // for now. change to some inclusive datastructure later.
    }
    SWSexSchema::AtomicRule::Value::TermSet SWSexSchema::AtomicRule::ValueType::getOs () const {
	SWSexSchema::AtomicRule::Value::TermSet ret;
	ret.push_back(NULL);
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::AtomicRule::ValueType::validateTriple (const w3c_sw::SWSexSchema::AtomicRule* rule, const TriplePattern* tp, DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::AtomicSolution* ret = new SWSexSchema::AtomicSolution(rule);
	const RDFLiteral* asLiteral = dynamic_cast<const RDFLiteral*>(tp->getO());
	const URI* type = asLiteral == NULL ? NULL : asLiteral->getDatatype();
	if (type == tterm) {
	    ret->passes = true;
	    source.erase(source.find(tp));
	    matched.addTriplePattern(tp);
	} else
	    ret->passes = false;
	return ret;
    }
    SWSexSchema::AtomicRule::Value::TermSet SWSexSchema::AtomicRule::ValueSet::getOs () const {
	return tterms;
    }
    SWSexSchema::Solution* SWSexSchema::AtomicRule::ValueSet::validateTriple (const w3c_sw::SWSexSchema::AtomicRule* rule, const TriplePattern* tp, DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::AtomicSolution* ret = new SWSexSchema::AtomicSolution(rule);
	for (TermSet::const_iterator it = tterms.begin(); it != tterms.end(); ++it)
	    if (tp->getO() == *it) {
		ret->passes = true;
		source.erase(source.find(tp));
		matched.addTriplePattern(tp);
		return ret;
	    }
	ret->passes = false;
	return ret;
    }
    SWSexSchema::AtomicRule::Value::TermSet SWSexSchema::AtomicRule::ValueAny::getOs () const {
	SWSexSchema::AtomicRule::Value::TermSet ret;
	ret.push_back(NULL);
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::AtomicRule::ValueAny::validateTriple (const w3c_sw::SWSexSchema::AtomicRule* rule, const TriplePattern* tp, DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::AtomicSolution* ret = new SWSexSchema::AtomicSolution(rule);
	ret->passes = true;
	source.erase(source.find(tp));
	matched.addTriplePattern(tp);
	return ret;
    }

    SWSexSchema::Solution* SWSexSchema::OrRule::validate (DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::SetSolution* ret = new SWSexSchema::SetSolution(this);
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::AndRule::validate (DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::SetSolution* ret = new SWSexSchema::SetSolution(this);
	DefaultGraphPattern matchedSoFar;
	ret->passes = true;
	for (std::vector<const RulePattern*>::const_iterator rule = rules.begin();
	     rule != rules.end(); ++rule) {
	    DefaultGraphPattern subMatched;
	    const SWSexSchema::Solution* s = (*rule)->validate(source, subMatched, rm, point);
	    ret->m.insert(std::make_pair(*rule, s)); // subMatched
	    if (s->passes) {
		std::copy(subMatched.begin(), subMatched.end(), std::back_inserter(matchedSoFar));
	    } else {
		ret->passes = false;
		// Put back the stuff we removed from the source.
		std::copy(matchedSoFar.begin(), matchedSoFar.end(), std::back_inserter(source));
		return ret;;
	    }
	}
	std::copy(matchedSoFar.begin(), matchedSoFar.end(), std::back_inserter(matched));
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::NegatedRule::validate (DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	throw NotImplemented("SWSexSchema::NegatedRule::validate");
    }
    SWSexSchema::Solution* SWSexSchema::AtomicRule::validate (DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	SWSexSchema::AtomicSolution* ret = new SWSexSchema::AtomicSolution(this);
	ret->passes = true;
	const TTerm* p = nameClass->getP();
	SWSexSchema::AtomicRule::Value::TermSet vals = value->getOs();
	PropertyPath::TriplesTemplates templates;
	for (SWSexSchema::AtomicRule::Value::TermSet::const_iterator v = vals.begin();
	     v != vals.end(); ++v)
	    templates.insert(PropertyPath::TriplesTemplate(nameClass->reverse ? *v : point, p, nameClass->reverse ? point : *v));

	DefaultGraphPattern matchedSoFar;
	size_t matchCount = 0;
	for (PropertyPath::TriplesTemplates::const_iterator tmplate = templates.begin();
	     tmplate != templates.end(); ++tmplate) {
	    BasicGraphPattern::triple_iterator mi = source.getTripleIterator(tmplate->s, tmplate->p, tmplate->o);
	    BasicGraphPattern::triple_iterator end;

	    DefaultGraphPattern copy;
	    for ( ; mi != end; ++mi)
		copy.addTriplePattern(*mi);
	    for (std::vector<const TriplePattern*>::const_iterator it = copy.begin(); it != copy.end(); ++it) {
		const TriplePattern* tp = *it;
		if (nameClass->matchP(tp->getP())) {
		    DefaultGraphPattern subMatched;
		    const SWSexSchema::Solution* s = value->validateTriple(this, tp, source, subMatched, rm, point);
		    // ret->m.insert(std::make_pair(this, subMatched));
		    if (s->passes && ++matchCount <= max) {
			/* TODO: Could process constraint Code here. */
			std::copy(subMatched.begin(), subMatched.end(), std::back_inserter(matchedSoFar));
			ret->refs.push_back(SWSexSchema::AtomicSolution::Ref(tp, s));
		    } else {
			ret->passes = false;
			// Put back the stuff we removed from the source.
			std::copy(matchedSoFar.begin(), matchedSoFar.end(), std::back_inserter(source));
			matchedSoFar.clearTriples();
			delete s;
			break;
		    }
		}
	    }
	}

	if (matchCount < min) {
	    ret->passes = false;
	    // Put back the stuff we removed from the source.
	    std::copy(matchedSoFar.begin(), matchedSoFar.end(), std::back_inserter(source));
	} else {
	    std::copy(matchedSoFar.begin(), matchedSoFar.end(), std::back_inserter(matched));
	}
	return ret;
    }
    SWSexSchema::Solution* SWSexSchema::RuleActions::validate (DefaultGraphPattern& source, DefaultGraphPattern& matched, const RuleMap& rm, const TTerm* point) const {
	return  pattern->validate(source, matched, rm, point);
    }

    bool SWSexSchema::validate (DefaultGraphPattern& bgp, const TTerm* point, const TTerm* as) const {
	const TTerm* from = as ? as : start;
	if (from == NULL)
	    throw std::runtime_error("no starting production"); // @@ XML Schema semantics here?
	RuleMap::const_iterator rule = ruleMap.find(from);
	if (rule == ruleMap.end())
	    throw std::runtime_error("starting production not found"); // not found

	DefaultGraphPattern source(bgp);
	DefaultGraphPattern matchedSoFar;
	SWSexSchema::Solution* sol = rule->second->validate(source, matchedSoFar, ruleMap, point);
	if (matchedSoFar.size() > 0)
	    w3c_sw_LINEN << "matched triples: " << matchedSoFar << "\n";
	if (source.size() > 0) {
	    w3c_sw_LINEN << "unused triples: " << source << "\n";
	    delete sol;
	    return false;
	}
	// sol->dispatch(handlers);
	delete sol;
	return true;
    }
} // namespace w3c_sw

