/* SWObjects: components for capturing and manipulating compile trees of RDF
   languages. This should capture all of SPARQL and most of N3 (no graphs as
   parts of an RDF triple).

 * $Id: SWObjects.hpp,v 1.26 2008-12-04 23:00:15 eric Exp $
 */

#ifndef SWOBJECTS_HH
# define SWOBJECTS_HH


#ifdef _MSC_VER
#define FUNCTION_STRING __FUNCSIG__ // __FUNCDNAME__ || __FUNCTION__ -- http://msdn.microsoft.com/en-us/library/b0084kay(VS.80).aspx

#else /* !_MSC_VER */
#ifdef __GNUC__
#define FUNCTION_STRING __PRETTY_FUNCTION__

#else /* !__GNUC__ */
#define FUNCTION_STRING "define a function name macro"

#endif /* !__GNUC__ */
#endif /* !_MSC_VER */


#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <exception>

#include <cstdarg>
#include <cassert>
#include <typeinfo>
#include <boost/regex.hpp>

#define TAB "  "
#define ns "\n xmlns=\"http://www.w3.org/2005/01/yacker/uploads/SPARQLfed/\"\n xmlns:yacker=\"http://www.w3.org/2005/01/yacker/\""

namespace w3c_sw {

class StringException : public std::exception {
public:
    char const* str;
    static std::map<StringException*, std::string> strs;

    StringException (std::string m) : str(m.c_str()) {
	strs[this] = m;
    }
    // !!! needs copy constructor in MS compilations, but
    //     haven't got UnknownPrefixException working in g++
#ifdef WIN32
    StringException (StringException& orig) {
	strs[this] = strs[&orig];
	str = strs[this].c_str();
    }
#endif
    virtual ~StringException () throw() { strs.erase(this); }
    char const* what() const throw() { 	return str; }
};

class Expressor;
class RecursiveExpressor;

class Base {
public:
    Base () { }
    virtual ~Base() { }
    virtual void express(Expressor* p_expressor) const;
};

template <typename T> class ProductionVector : public Base {
protected:
    std::vector<T> data;
public:
    ProductionVector () {  }
    ProductionVector (T v) { push_back(v); }
    virtual ~ProductionVector () {
	for (typename std::vector<T>::iterator it = ProductionVector<T>::begin(); 
	     it != ProductionVector<T>::end(); ++it)
	    delete *it;
    }

    void push_back(T v) {
	assert(v != NULL); // @DEBUG
	data.push_back(v);
    }
    size_t size () const { return data.size(); }
    virtual T operator [] (size_t i) { return data[i]; }
    virtual T at (size_t i) { return data.at(i); }
    void clear () { data.clear(); }
    void pop_back () { data.pop_back(); }
    virtual void express(Expressor* p_expressor) const {
	for (size_t i = 0; i < data.size(); i++)
	    data[i]->express(p_expressor);
    }
    typename std::vector<T>::iterator begin () { return data.begin(); }
    typename std::vector<T>::const_iterator begin () const { return data.begin(); }
    typename std::vector<T>::iterator end () { return data.end(); }
    typename std::vector<T>::const_iterator end () const { return data.end(); }
    void erase (typename std::vector<T>::iterator it) { data.erase(it); }
    void sort (bool (*comp)(T, T)) {
	std::list<T> l;
	for (typename std::vector<T>::iterator it = begin(); it != end(); ++it)
	    l.push_back(*it);
	l.sort(comp);
	data.clear();
	for (typename std::list<T>::iterator it = l.begin(); it != l.end(); ++it)
	    data.push_back(*it);
    }
#if 0
    class iterator;
    iterator begin() { return iterator(data.begin(), this); }
    iterator end() { return iterator(data.end(), this); }
#endif
};
template <typename T> class NoDelProductionVector : public ProductionVector<T> {
public:
    NoDelProductionVector () {  }
    NoDelProductionVector (T v) : ProductionVector<T>(v) {  }
    virtual ~NoDelProductionVector () { ProductionVector<T>::clear(); }
};
#if 0
template <class T> class ProductionVector<T>::iterator:
public std::iterator<std::forward_iterator_tag, void, void, void, void> {
private:
    ProductionVector<T> * whence;
protected:
    typename vector<T>::iterator i;
public:
    iterator () {}
    iterator (typename vector<T>::iterator i_, ProductionVector<T> * w): i(i_), whence(w) {  }
    bool operator==(const iterator & z) { return i == z.i; }
    bool operator!=(const iterator & z) { return i != z.i; }
    void operator++() { ++i; }
    T operator*() { return *i; }
};
#endif

class Terminal : public Base {
protected:
    std::string terminal;
    Terminal (std::string p) : Base(), terminal(p) {  }
    Terminal (std::string p, bool gensym) : Base() {
	std::stringstream name;
	name << p;
	if (gensym)
	    name << this;
	terminal = name.str();
    }
    ~Terminal () {  }
public:
    std::string getTerminal () const { return terminal; }
};

} // namespace w3c_sw

namespace w3c_sw {

class ResultSet;
class Result;
class RdfDB;

class LANGTAG : public Terminal {
public:
    LANGTAG(std::string p_LANGTAG) : Terminal(p_LANGTAG) {  }
};

class Operation : public Base {
protected:
    Operation () : Base() {  }
public:
    virtual ResultSet* execute(RdfDB*, ResultSet* = NULL) { throw(std::runtime_error(typeid(*this).name())); }
    virtual void express(Expressor* p_expressor) const = 0;
};

class POS;
class POSFactory;

/* START Parts Of Speach */
class POS : public Terminal {
    friend class POSsorter;
protected:
    POS (std::string matched) : Terminal(matched) {  }
    POS (std::string matched, bool gensym) : Terminal(matched, gensym) { }
    //    virtual int compareType (POS* to) = 0;
public:
    virtual bool isConstant () { return true; } // Override for variable types.
    static bool orderByType (const POS*, const POS*) { throw(std::runtime_error(FUNCTION_STRING)); }
    virtual int compare (POS* to, Result*) const {
	bool same = typeid(*to) == typeid(*this);
	return same ? getTerminal() != to->getTerminal() : orderByType(this, to);
    }
    virtual const POS* evalPOS (const Result*, bool) const { return this; }
    virtual void express(Expressor* p_expressor) const = 0;
    virtual std::string getBindingAttributeName() = 0;
    virtual std::string toString() const = 0;
    std::string substitutedString (Result* row, bool bNodesGenSymbols) const {
	const POS* subd = evalPOS(row, bNodesGenSymbols); /* re-uses atoms -- doesn't create them */
	if (subd != NULL)
	    return subd->toString();
	std::stringstream s;
	s << '[' << toString() << ']';
	return s.str();
    }
};

class URI : public POS {
    friend class POSFactory;
private:
    URI (std::string str) : POS(str) {  }
public:
    ~URI () { }
    virtual const char * getToken () { return "-POS-"; }
    virtual void express(Expressor* p_expressor) const;
    virtual std::string toString () const { std::stringstream s; s << "<" << terminal << ">"; return s.str(); }
    virtual std::string getBindingAttributeName () { return "uri"; }
    bool matches (std::string toMatch) { return terminal == toMatch; } // !!! added for SPARQLSerializer::functionCall
};

class Bindable : public POS {
protected:
    Bindable (std::string str) : POS(str) {  }
    Bindable (std::string str, bool gensym) : POS(str, gensym) {  }
public:
    virtual bool isConstant () { return false; }
};

struct URImap {
    boost::regex ifacePattern;
    std::string localPattern;
};

class Variable : public Bindable {
    friend class POSFactory;
protected:
    std::vector<URImap> uriMaps;
    POSFactory* posFactory;
private:
    Variable (std::string str) : Bindable(str) {  }
public:
    virtual std::string toString () const { std::stringstream s; s << "?" << terminal; return s.str(); }
    virtual const char * getToken () { return "-Variable-"; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* evalPOS(const Result* r, bool bNodesGenSymbols) const;
    virtual std::string getBindingAttributeName () { return "name"; }
    void setMaps (std::vector<URImap> maps, POSFactory* factory) { uriMaps = maps; posFactory = factory; }
};

class BNode : public Bindable {
    friend class POSFactory;
private:
    BNode (std::string str) : Bindable(str) {  }
    BNode () : Bindable("b", true) {  }
public:
    virtual std::string toString () const { std::stringstream s; s << "_:" << terminal; return s.str(); }
    virtual const char * getToken () { return "-BNode-"; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* evalPOS(const Result* r, bool bNodesGenSymbols) const;
    virtual std::string getBindingAttributeName () { return "bnode"; }
};

class RDFLiteral : public POS {
    friend class POSFactory;
private:
    std::string m_String;
    URI* datatype;
    LANGTAG* m_LANGTAG;

protected:
    RDFLiteral (std::string p_String, URI* p_URI, LANGTAG* p_LANGTAG, std::string matched) : POS(matched), m_String(p_String) {
	datatype = p_URI;
	m_LANGTAG = p_LANGTAG;
    }

    ~RDFLiteral () {
	delete m_LANGTAG;
    }
public:
    virtual std::string toString () const {
	std::stringstream s;
	/* Could just print terminal here. */
	s << '"' << m_String << '"';
	if (datatype) s << datatype->toString();
	if (m_LANGTAG) s << m_LANGTAG->getTerminal();
	return s.str();
    }
    virtual void express(Expressor* p_expressor) const;
    virtual std::string getBindingAttributeName () { return "literal"; }
    std::string getString () { return m_String; }
};
class NumericRDFLiteral : public RDFLiteral {
    friend class POSFactory;
protected:
    NumericRDFLiteral (std::string p_String, URI* p_URI, std::string matched) : RDFLiteral(p_String, p_URI, NULL, matched) {  }
    ~NumericRDFLiteral () {  }
public:
    virtual void express(Expressor* p_expressor) const = 0;
};
class IntegerRDFLiteral : public NumericRDFLiteral {
    friend class POSFactory;
protected:
    int m_value;
    IntegerRDFLiteral (std::string p_String, URI* p_URI, std::string matched, int p_value) : NumericRDFLiteral(p_String, p_URI, matched), m_value(p_value) {  }
    ~IntegerRDFLiteral () {  }
public:
    int getValue () { return m_value; }
    virtual std::string toString () const { std::stringstream s; s << m_value; return s.str(); }
    virtual void express(Expressor* p_expressor) const;
};
class DecimalRDFLiteral : public NumericRDFLiteral {
    friend class POSFactory;
protected:
    float m_value;
    DecimalRDFLiteral (std::string p_String, URI* p_URI, std::string matched, float p_value) : NumericRDFLiteral(p_String, p_URI, matched), m_value(p_value) {  }
    ~DecimalRDFLiteral () {  }
    virtual void express(Expressor* p_expressor) const;
public:
    virtual std::string toString () const { std::stringstream s; s << m_value; return s.str(); }
};
class DoubleRDFLiteral : public NumericRDFLiteral {
    friend class POSFactory;
protected:
    double m_value;
    DoubleRDFLiteral (std::string p_String, URI* p_URI, std::string matched, double p_value) : NumericRDFLiteral(p_String, p_URI, matched), m_value(p_value) {  }
    ~DoubleRDFLiteral () {  }
    virtual void express(Expressor* p_expressor) const;
};
class BooleanRDFLiteral : public RDFLiteral {
    friend class POSFactory;
protected:
    bool m_value;
    BooleanRDFLiteral (std::string p_String, std::string matched, bool p_value) : RDFLiteral(p_String, NULL, NULL, matched), m_value(p_value) {  }
public:
    virtual std::string toString () const { std::stringstream s; s << (m_value ? "true" : "false"); return s.str(); }
    virtual void express(Expressor* p_expressor) const;
};
class NULLpos : public POS {
    friend class POSFactory;
private:
    NULLpos () : POS("NULL", "") {  }
    ~NULLpos () {  }
public:
    virtual const char * getToken () { return "-NULL-"; }
    virtual std::string toString () const { std::stringstream s; s << "NULL"; return s.str(); }
    virtual void express(Expressor* p_expressor) const;
    virtual std::string getBindingAttributeName () { throw(std::runtime_error(FUNCTION_STRING)); }
};

class BasicGraphPattern;

class TriplePattern : public Base {
    friend class POSFactory;
private:
    POS* m_s; POS* m_p; POS* m_o;
    bool weaklyBound;
    static bool _bindVariable(POS* it, const POS* p, ResultSet* rs, Result* provisional, bool weaklyBound);
    TriplePattern (POS* p_s, POS* p_p, POS* p_o) : Base(), m_s(p_s), m_p(p_p), m_o(p_o), weaklyBound(false) {  }
    TriplePattern (TriplePattern const& copy, bool weaklyBound) : Base(), m_s(copy.m_s), m_p(copy.m_p), m_o(copy.m_o), weaklyBound(weaklyBound) {  }
public:
    ~TriplePattern () {  }
    POS* getS () { return m_s; }
    POS* getP () { return m_p; }
    POS* getO () { return m_o; }
    static bool lt (TriplePattern* l, TriplePattern* r) {
	if (l->m_s != r->m_s) return l->m_s < r->m_s;
	if (l->m_p != r->m_p) return l->m_p < r->m_p;
	if (l->m_o != r->m_o) return l->m_o < r->m_o;
	return 0;
    }    
    static bool gt (TriplePattern* l, TriplePattern* r) {
	if (l->m_s != r->m_s) return l->m_s > r->m_s;
	if (l->m_p != r->m_p) return l->m_p > r->m_p;
	if (l->m_o != r->m_o) return l->m_o > r->m_o;
	return 0;
    }    
    std::string toString () const {
	std::stringstream s;
	s << "{" << m_s->toString() << " " << m_p->toString() << " " << m_o->toString() << "}";
	return s.str();
    }
    std::string toString (Result* row) const {
	std::stringstream s;
	s << 
	    "{" << m_s->substitutedString(row, false) << 
	    " " << m_p->substitutedString(row, false) << 
	    " " << m_o->substitutedString(row, false) << "}";
	return s.str();
    }
    virtual void express(Expressor* p_expressor) const;
    bool bindVariables (TriplePattern* tp, bool, ResultSet* rs, POS* graphVar, Result* provisional, POS* graphName) {
	return
	    _bindVariable(graphVar, graphName, rs, provisional, weaklyBound) &&
	    _bindVariable(tp->m_s, m_s, rs, provisional, weaklyBound) && 
	    _bindVariable(tp->m_p, m_p, rs, provisional, weaklyBound) && 
	    _bindVariable(tp->m_o, m_o, rs, provisional, weaklyBound);
    }
    bool construct(BasicGraphPattern* target, Result* r, POSFactory* posFactory, bool bNodesGenSymbols = true);
};

class DefaultGraphPattern;
class POSFactory {
    typedef std::map<std::string, Variable*> VariableMap;
    typedef std::map<std::string, BNode*> BNodeMap;
    typedef std::map<std::string, URI*> URIMap;
    typedef std::map<std::string, RDFLiteral*> RDFLiteralMap;
    typedef std::map<std::string, TriplePattern*> TriplePatternMap; // i don't know what the key should be. string for now...
    class MakeNumericRDFLiteral {
    public:
	virtual ~MakeNumericRDFLiteral () {  }
	virtual NumericRDFLiteral* makeIt(std::string p_String, URI* p_URI, std::string matched) = 0;
    };

protected:
    VariableMap		variables;
    BNodeMap		bnodes;
    URIMap		uris;
    RDFLiteralMap	rdfLiterals;
    TriplePatternMap	triples;
    NULLpos		nullPOS;
    BooleanRDFLiteral*	litFalse;
    BooleanRDFLiteral*	litTrue;
    NumericRDFLiteral* getNumericRDFLiteral(std::string p_String, const char* type, MakeNumericRDFLiteral* maker);
    std::map<const std::string, int> typeOrder;

public:
    POSFactory () {
	litFalse  = getBooleanRDFLiteral("false", false);
	litTrue  = getBooleanRDFLiteral("true", true);

	typeOrder[typeid(BNode).name()] = 2;
	typeOrder[typeid(URI).name()] = 3;
	typeOrder[typeid(RDFLiteral).name()] = 4;
    }
    ~POSFactory();
    Variable* getVariable(std::string name);
    BNode* createBNode();
    BNode* getBNode(std::string name);
    URI* getURI(std::string name);
    POS* getPOS(std::string posStr);
    RDFLiteral* getRDFLiteral(std::string p_String, URI* p_URI, LANGTAG* p_LANGTAG);

    IntegerRDFLiteral* getNumericRDFLiteral(std::string p_String, int p_value);
    DecimalRDFLiteral* getNumericRDFLiteral(std::string p_String, float p_value);
    DoubleRDFLiteral* getNumericRDFLiteral(std::string p_String, double p_value);

    BooleanRDFLiteral* getBooleanRDFLiteral(std::string p_String, bool p_value);
    BooleanRDFLiteral* getFalse () { return litFalse; }
    BooleanRDFLiteral* getTrue () { return litTrue; }
    NULLpos* getNULL () { return &nullPOS; }

    /* getTriple(s) interface: */
    TriplePattern* getTriple (TriplePattern* p, bool weaklyBound) {
	return getTriple(p->getS(), p->getP(), p->getO(), weaklyBound);
    }
    TriplePattern* getTriple(POS* s, POS* p, POS* o, bool weaklyBound = false);
    TriplePattern* getTriple (std::string s, std::string p, std::string o) {
	return getTriple(getPOS(s), getPOS(p), getPOS(o), false);
    }
    TriplePattern* getTriple (std::string spo) {
	const boost::regex expression("[[:space:]]*((?:<[^>]*>)|(?:_:[^[:space:]]+)|(?:[?$][^[:space:]]+)|(?:\\\"[^\\\"]+\\\"))"
				      "[[:space:]]*((?:<[^>]*>)|(?:_:[^[:space:]]+)|(?:[?$][^[:space:]]+)|(?:\\\"[^\\\"]+\\\"))"
				      "[[:space:]]*((?:<[^>]*>)|(?:_:[^[:space:]]+)|(?:[?$][^[:space:]]+)|(?:\\\"[^\\\"]+\\\"))[[:space:]]*\\.");
	boost::match_results<std::string::const_iterator> what;
	boost::match_flag_type flags = boost::match_default;
	if (!regex_search(spo, what, expression, flags))
	    return NULL;
	std::string s(what[1].first, what[1].second);
	return getTriple(getPOS(std::string(what[1].first, what[1].second)), 
			 getPOS(std::string(what[2].first, what[2].second)), 
			 getPOS(std::string(what[3].first, what[3].second)), false);
    }
    void parseTriples (BasicGraphPattern* g, std::string spo);

    /* EBV (Better place for this?) */
    const POS* ebv(const POS* pos);
    bool lessThan (const POS* lhs, const POS* rhs) {
	const std::string lt = typeid(*lhs).name();
	const std::string rt = typeid(*rhs).name();
	if (lt != rt) {
	    const int li = typeOrder[lt];
	    const int ri = typeOrder[rt];
	    if (li < ri)
		return true;
	}
	return lhs->getTerminal().compare(rhs->getTerminal()) < 0;
    }
};

    /* Sorter for the POSs. */
class POSsorter;
extern POSsorter* ThePOSsorter;

class POSsorter {
public:
    std::map<const std::string, int> typeOrder;
    POSsorter () {
	//typeOrder.insert(make_pair(typeid(BNode).name(), 2));
	typeOrder[typeid(BNode).name()] = 2;
	typeOrder[typeid(URI).name()] = 3;
	typeOrder[typeid(RDFLiteral).name()] = 4;
	ThePOSsorter = this;
    }
    bool sort (const POS* lhs, const POS* rhs) {
	const std::string lt = typeid(*lhs).name();
	const std::string rt = typeid(*rhs).name();
	const int li = typeOrder[lt];
	const int ri = typeOrder[rt];
	if (li < ri)
	    return true;
	return lhs->getTerminal().compare(rhs->getTerminal()) < 0;
    }
};

/* END Parts Of Speach */

class Expression : public Base {
private:
public:
    Expression () : Base() { }
    ~Expression () {  }
    virtual void express(Expressor* p_expressor) const = 0;
    virtual const POS* eval(const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const = 0;
};

typedef enum {DIST_all, DIST_distinct, DIST_reduced} e_distinctness;
typedef enum { ORDER_Asc, ORDER_Desc } e_ASCorDESC;
#define LIMIT_None -1
#define OFFSET_None -1
typedef struct {e_ASCorDESC ascOrDesc; Expression* expression;} s_OrderConditionPair;
typedef enum { SILENT_Yes, SILENT_No } e_Silence;

class Filter : public Base {
private:
    Expression* m_Constraint;
public:
    Filter (Expression* p_Constraint) : Base(), m_Constraint(p_Constraint) {  }
    ~Filter () { delete m_Constraint; }
    virtual void express(Expressor* p_expressor) const;
};

/*
TableOperation class hierarchy:               Base
                                               |
                             ___________TableOperation_____________
            ________________/                  |                   \__________________
     TableJunction                    BasicGraphPattern                     TableOperationOnOperation
        /        \                       /          \                           /               \
T*Conjunction T*Disjunction   NamedGraphPattern  DefaultGraphPattern  GraphGraphPattern  OptionalGraphPattern


related Expressor operations:   base(Base* self, std::string productionName)
                                               |
                             __________[TableOperation]____________
            ________________/                  |                   \__________________
    [TableJunction]                  [BasicGraphPattern]                   [TableOperationOnOperation]
        /        \                       /          \                           /               \
t*Conjunction t*Disjunction   namedGraphPattern  defaultGraphPattern  graphGraphPattern  optionalGraphPattern

*/

class TableOperation : public Base {
protected:
    ProductionVector<Filter*> m_Filters;
    TableOperation () : Base(), m_Filters() { }
public:
    //size_t filters () { return m_Filters.size(); }
    void addFilter (Filter* filter) {
	m_Filters.push_back(filter);
    }
    virtual void bindVariables(RdfDB*, ResultSet*) = 0; //{ throw(std::runtime_error(FUNCTION_STRING)); }
    virtual void express(Expressor* p_expressor) const = 0;
    virtual TableOperation* getDNF() = 0;
};
class TableJunction : public TableOperation {
protected:
    ProductionVector<TableOperation*> m_TableOperations;
public:
    TableJunction () : TableOperation(), m_TableOperations() {  }

    virtual void addTableOperation(TableOperation* tableOp);
    std::vector<TableOperation*>::iterator begin () { return m_TableOperations.begin(); }
    std::vector<TableOperation*>::iterator end () { return m_TableOperations.end(); }
    void clear () { m_TableOperations.clear(); }
    void erase (std::vector<TableOperation*>::iterator it) { m_TableOperations.erase(it); }
    size_t size () const { return m_TableOperations.size(); }
    TableOperation* simplify () {
	TableOperation* ret;
	if (size() == 0 || size() == 1) {
	    ret = size() == 0 ? NULL : *begin();
	    clear();
	    delete this;
	} else
	    ret = this;
	return ret;
    }
};
class TableConjunction : public TableJunction { // ⊍
public:
    TableConjunction () : TableJunction() {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables(RdfDB*, ResultSet* rs);
    virtual TableOperation* getDNF();
};
class TableDisjunction : public TableJunction { // ⊎
public:
    TableDisjunction () : TableJunction() {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables(RdfDB*, ResultSet* rs);
    virtual TableOperation* getDNF();
};
class DontDeleteThisBGP;
class BasicGraphPattern : public TableOperation { // ⊌⊍
protected:
    // make sure we don't delete the TriplePatterns
    NoDelProductionVector<TriplePattern*> m_TriplePatterns;
    bool allOpts;
    BasicGraphPattern (bool allOpts) : TableOperation(), m_TriplePatterns(), allOpts(allOpts) {  }

public:
    void addTriplePattern (TriplePattern* p) {
	for (std::vector<TriplePattern*>::iterator it = m_TriplePatterns.begin();
	     it != m_TriplePatterns.end(); ++it)
	    if (*it == p)
		return;
	m_TriplePatterns.push_back(p);
    }
    virtual void bindVariables(RdfDB* db, ResultSet* rs) = 0;
    void bindVariables(ResultSet* rs, POS* graphVar, BasicGraphPattern* toMatch, POS* graphName);
    void construct(BasicGraphPattern* target, ResultSet* rs);
    size_t size () const { return m_TriplePatterns.size(); }
    std::vector<TriplePattern*>::iterator begin () { return m_TriplePatterns.begin(); }
    std::vector<TriplePattern*>::const_iterator begin () const { return m_TriplePatterns.begin(); }
    std::vector<TriplePattern*>::iterator end () { return m_TriplePatterns.end(); }
    std::vector<TriplePattern*>::const_iterator end () const { return m_TriplePatterns.end(); }
    void erase (std::vector<TriplePattern*>::iterator it) { m_TriplePatterns.erase(it); }
    void sort (bool (*comp)(TriplePattern*, TriplePattern*)) { m_TriplePatterns.sort(comp); }
    void clearTriples () { m_TriplePatterns.clear(); }
    virtual TableOperation* getDNF ();
    virtual void express(Expressor* p_expressor) const = 0;
};
class DontDeleteThisBGP : public TableOperation {
protected: BasicGraphPattern* bgp;
public:
    DontDeleteThisBGP (BasicGraphPattern* bgp) : bgp(bgp) {  }
    ~DontDeleteThisBGP () { /* Leave bgp alone. */ }
    virtual void bindVariables(RdfDB* db, ResultSet* rs) { bgp->bindVariables(db, rs); }
    virtual TableOperation* getDNF () { return new DontDeleteThisBGP(bgp); }
    virtual void express (Expressor* p_expressor) const { bgp->express(p_expressor); }
};

class NamedGraphPattern : public BasicGraphPattern {
private:
    POS* m_name;

public:
    NamedGraphPattern (POS* p_name, bool allOpts = false) : BasicGraphPattern(allOpts), m_name(p_name) {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables(RdfDB* db, ResultSet* rs);
};
class DefaultGraphPattern : public BasicGraphPattern {
public:
    DefaultGraphPattern (bool allOpts = false) : BasicGraphPattern(allOpts) {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables(RdfDB* db, ResultSet* rs);
    bool operator== (const DefaultGraphPattern & ref) const {
	if (m_TriplePatterns.size() != ref.m_TriplePatterns.size())
	    return false;

	std::set<TriplePattern*> refs;
	for (std::vector<TriplePattern*>::const_iterator it = ref.m_TriplePatterns.begin();
	     it != ref.m_TriplePatterns.end(); ++it)
	    refs.insert(*it);

	for (std::vector<TriplePattern*>::const_iterator it = m_TriplePatterns.begin();
	     it != m_TriplePatterns.end(); ++it)
	    if (refs.erase(*it) == 0)
		return false;
	return true;
    }
};
class TableOperationOnOperation : public TableOperation {
protected:
    TableOperation* m_TableOperation;
    TableOperationOnOperation (TableOperation* p_TableOperation) : TableOperation(), m_TableOperation(p_TableOperation) {  }
    ~TableOperationOnOperation() { delete m_TableOperation; }
    virtual TableOperationOnOperation* makeANewThis(TableOperation* p_TableOperation) = 0;
public:
    virtual TableOperation* getDNF();
};
/* GraphGraphPattern: pass-through class that's just used to reproduce verbatim SPARQL queries
 */
class GraphGraphPattern : public TableOperationOnOperation {
private:
    POS* m_VarOrIRIref;
public:
    GraphGraphPattern (POS* p_POS, TableOperation* p_GroupGraphPattern) : TableOperationOnOperation(p_GroupGraphPattern), m_VarOrIRIref(p_POS) {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables (RdfDB* db, ResultSet* rs) {
	m_TableOperation->bindVariables(db, rs);
    }
    virtual TableOperationOnOperation* makeANewThis (TableOperation* p_TableOperation) { return new GraphGraphPattern(m_VarOrIRIref, p_TableOperation); }
};
class OptionalGraphPattern : public TableOperationOnOperation {
public:
    OptionalGraphPattern (TableOperation* p_GroupGraphPattern) : TableOperationOnOperation(p_GroupGraphPattern) {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void bindVariables(RdfDB*, ResultSet* rs);
    virtual TableOperationOnOperation* makeANewThis (TableOperation* p_TableOperation) { return new OptionalGraphPattern(p_TableOperation); }
};

class VarSet : public Base {
protected:
    VarSet () : Base() { }
public:
    virtual void express(Expressor* p_expressor) const = 0;
};

class POSList : public VarSet {
private:
    ProductionVector<POS*> m_POSs;
public:
    POSList () : VarSet(), m_POSs() {  }
    ~POSList () { m_POSs.clear(); }
    void push_back(POS* v) { m_POSs.push_back(v); }
    virtual void express(Expressor* p_expressor) const;
    std::vector<POS*>::iterator begin () { return m_POSs.begin(); }
    std::vector<POS*>::iterator end () { return m_POSs.end(); }
};
class StarVarSet : public VarSet {
private:
public:
    StarVarSet () : VarSet() {  }
    size_t size() { return 0; }
    POS* operator [] (size_t) { return NULL; }
    POS* getElement (size_t) { return NULL; }
    virtual void express(Expressor* p_expressor) const;
};

class DatasetClause : public Base {
protected:
    POS* m_IRIref;
    POSFactory* m_posFactory;
public:
    DatasetClause (POS* p_IRIref, POSFactory* p_posFactory) : Base(), m_IRIref(p_IRIref), m_posFactory(p_posFactory) {  }
    ~DatasetClause () { /* m_IRIref is centrally managed */ }
    virtual void loadData(RdfDB*) = 0;
    void _loadData(BasicGraphPattern*);
    virtual void express(Expressor* p_expressor) const = 0;
};
class DefaultGraphClause : public DatasetClause {
private:
public:
    DefaultGraphClause (POS* p_IRIref, POSFactory* p_posFactory) : DatasetClause(p_IRIref, p_posFactory) { }
    ~DefaultGraphClause () {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void loadData(RdfDB*);
};
class NamedGraphClause : public DatasetClause {
private:
public:
    NamedGraphClause (POS* p_IRIref, POSFactory* p_posFactory) : DatasetClause(p_IRIref, p_posFactory) { }
    ~NamedGraphClause () {  }
    virtual void express(Expressor* p_expressor) const;
    virtual void loadData(RdfDB*);
};

    /* SolutionModifiers */
class SolutionModifier : public Base {
private:
    std::vector<s_OrderConditionPair>* m_OrderConditions;
    int m_limit;
    int m_offset;
public:
    SolutionModifier (std::vector<s_OrderConditionPair>* p_OrderConditions, int p_limit, int p_offset) : Base(), m_OrderConditions(p_OrderConditions), m_limit(p_limit), m_offset(p_offset) {  }
    ~SolutionModifier () {
	if (m_OrderConditions != NULL)
	    for (size_t i = 0; i < m_OrderConditions->size(); i++)
		delete m_OrderConditions->at(i).expression;
	delete m_OrderConditions;
    }
    void modifyResult(ResultSet* rs);
    virtual void express(Expressor* p_expressor) const;
};
class Binding : public ProductionVector<POS*> {
private:
public:
    Binding () : ProductionVector<POS*>() {  }
    ~Binding () { clear(); /* atoms in vector are centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
    void bindVariables(RdfDB* db, ResultSet* rs, Result* r, POSList* p_Vars);
};
class BindingClause : public ProductionVector<Binding*> {
private:
    POSList* m_Vars;
public:
    BindingClause (POSList* p_Vars) : ProductionVector<Binding*>(), m_Vars(p_Vars) {  }
    ~BindingClause () { delete m_Vars; }
    virtual void express(Expressor* p_expressor) const;
    void bindVariables(RdfDB* db, ResultSet* rs);
};
class WhereClause : public Base {
private:
    TableOperation* m_GroupGraphPattern;
    BindingClause* m_BindingClause;
public:
    WhereClause (TableOperation* p_GroupGraphPattern, BindingClause* p_BindingClause) : Base(), m_GroupGraphPattern(p_GroupGraphPattern), m_BindingClause(p_BindingClause) {  }
    ~WhereClause () {
	delete m_GroupGraphPattern;
	delete m_BindingClause;
    }
    virtual void express(Expressor* p_expressor) const;
    void bindVariables(RdfDB* db, ResultSet* rs);
};

class Select : public Operation {
private:
    e_distinctness m_distinctness;
    VarSet* m_VarSet;
    ProductionVector<DatasetClause*>* m_DatasetClauses;
    WhereClause* m_WhereClause;
    SolutionModifier* m_SolutionModifier;
public:
    Select (e_distinctness p_distinctness, VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) : Operation(), m_distinctness(p_distinctness), m_VarSet(p_VarSet), m_DatasetClauses(p_DatasetClauses), m_WhereClause(p_WhereClause), m_SolutionModifier(p_SolutionModifier) {  }
    ~Select () {
	delete m_VarSet;
	delete m_DatasetClauses;
	delete m_WhereClause;
	delete m_SolutionModifier;
    }
    virtual void express(Expressor* p_expressor) const;
    virtual ResultSet* execute(RdfDB* db, ResultSet* rs = NULL);
};
class Construct : public Operation {
protected:
    DefaultGraphPattern* m_ConstructTemplate;
    ProductionVector<DatasetClause*>* m_DatasetClauses;
    WhereClause* m_WhereClause;
    SolutionModifier* m_SolutionModifier;
public:
    Construct (DefaultGraphPattern* p_ConstructTemplate, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) : Operation(), m_ConstructTemplate(p_ConstructTemplate), m_DatasetClauses(p_DatasetClauses), m_WhereClause(p_WhereClause), m_SolutionModifier(p_SolutionModifier) {  }
    ~Construct () {
	delete m_ConstructTemplate;
	delete m_DatasetClauses;
	delete m_WhereClause;
	delete m_SolutionModifier;
    }
    virtual void express(Expressor* p_expressor) const;
    virtual ResultSet* execute(RdfDB* db, ResultSet* rs = NULL);
    WhereClause* getWhereClause () { return m_WhereClause; }
};
class Describe : public Operation {
private:
    VarSet* m_VarSet;
    ProductionVector<DatasetClause*>* m_DatasetClauses;
    WhereClause* m_WhereClause;
    SolutionModifier* m_SolutionModifier;
public:
    Describe (VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) : Operation(), m_VarSet(p_VarSet), m_DatasetClauses(p_DatasetClauses), m_WhereClause(p_WhereClause), m_SolutionModifier(p_SolutionModifier) {  }
    ~Describe () {
	delete m_VarSet;
	delete m_DatasetClauses;
	delete m_WhereClause;
	delete m_SolutionModifier;
    }
    virtual void express(Expressor* p_expressor) const;
};
class Ask : public Operation {
private:
    ProductionVector<DatasetClause*>* m_DatasetClauses;
    WhereClause* m_WhereClause;
public:
    Ask (ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause) : Operation(), m_DatasetClauses(p_DatasetClauses), m_WhereClause(p_WhereClause) {  }
    ~Ask () {
	delete m_DatasetClauses;
	delete m_WhereClause;
    }
    virtual void express(Expressor* p_expressor) const;
};
class Replace : public Operation {
private:
    WhereClause* m_WhereClause;
    TableOperation* m_GraphTemplate;
public:
    Replace (WhereClause* p_WhereClause, TableOperation* p_GraphTemplate) : Operation(), m_WhereClause(p_WhereClause), m_GraphTemplate(p_GraphTemplate) {  }
    ~Replace () { delete m_WhereClause; delete m_GraphTemplate; }
    virtual void express(Expressor* p_expressor) const;
};
class Insert : public Operation {
private:
    TableOperation* m_GraphTemplate;
    WhereClause* m_WhereClause;
public:
    Insert (TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) : Operation(), m_GraphTemplate(p_GraphTemplate), m_WhereClause(p_WhereClause) {  }
    ~Insert () { delete m_GraphTemplate; delete m_WhereClause; }
    virtual void express(Expressor* p_expressor) const;
};
class Delete : public Operation {
private:
    TableOperation* m_GraphTemplate;
    WhereClause* m_WhereClause;
public:
    Delete (TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) : Operation(), m_GraphTemplate(p_GraphTemplate), m_WhereClause(p_WhereClause) {  }
    ~Delete () { delete m_GraphTemplate; delete m_WhereClause; }
    virtual void express(Expressor* p_expressor) const;
};
class Load : public Operation {
private:
    ProductionVector<URI*>* m_IRIrefs;
    URI* m_into;
public:
    Load (ProductionVector<URI*>* p_IRIrefs, URI* p_into) : Operation(), m_IRIrefs(p_IRIrefs), m_into(p_into) {  }
    ~Load () { delete m_IRIrefs; delete m_into; }
    virtual void express(Expressor* p_expressor) const;
};
class Clear : public Operation {
private:
    URI* m__QGraphIRI_E_Opt;
public:
    Clear (URI* p__QGraphIRI_E_Opt) : Operation(), m__QGraphIRI_E_Opt(p__QGraphIRI_E_Opt) { }
    ~Clear () { delete m__QGraphIRI_E_Opt; }
    virtual void express(Expressor* p_expressor) const;
};
class Create : public Operation {
private:
    e_Silence m_Silence;
    URI* m_GraphIRI;
public:
    Create (e_Silence p_Silence, URI* p_GraphIRI) : Operation(), m_Silence(p_Silence), m_GraphIRI(p_GraphIRI) {  }
    ~Create () { /* m_GraphIRI is centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
};
class Drop : public Operation {
private:
    e_Silence m_Silence;
    URI* m_GraphIRI;
public:
    Drop (e_Silence p_Silence, URI* p_GraphIRI) : Operation(), m_Silence(p_Silence), m_GraphIRI(p_GraphIRI) {  }
    ~Drop () { /* m_GraphIRI is centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
};

/* kinds of Expressions */
class VarExpression : public Expression {
private:
    const Variable* m_Variable;
public:
    VarExpression (const Variable* p_Variable) : Expression(), m_Variable(p_Variable) {  }
    ~VarExpression () { /* m_Variable is centrally managed */ }
    const Variable* getVariable () { return m_Variable; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* /* posFactory */, bool bNodesGenSymbols) const {
	return m_Variable->evalPOS(r, bNodesGenSymbols);
    }
};
class LiteralExpression : public Expression {
private:
    RDFLiteral* m_RDFLiteral;
public:
    LiteralExpression (RDFLiteral* p_RDFLiteral) : Expression(), m_RDFLiteral(p_RDFLiteral) {  }
    ~LiteralExpression () { /* m_RDFLiteral is centrally managed */ }
    RDFLiteral* getLiteral () { return m_RDFLiteral; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* /* posFactory */, bool bNodesGenSymbols) const {
	return m_RDFLiteral->evalPOS(r, bNodesGenSymbols);
    }
};
class BooleanExpression : public Expression {
private:
    BooleanRDFLiteral* m_BooleanRDFLiteral;
public:
    BooleanExpression (BooleanRDFLiteral* p_BooleanRDFLiteral) : Expression(), m_BooleanRDFLiteral(p_BooleanRDFLiteral) {  }
    ~BooleanExpression () { /* m_BooleanRDFLiteral is centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* /* posFactory */, bool bNodesGenSymbols) const {
	return m_BooleanRDFLiteral->evalPOS(r, bNodesGenSymbols);
    }
};
class URIExpression : public Expression {
private:
    URI* m_URI;
public:
    URIExpression (URI* p_URI) : Expression(), m_URI(p_URI) {  }
    ~URIExpression () { /* m_URI is centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* /* posFactory */, bool bNodesGenSymbols) const {
	return m_URI->evalPOS(r, bNodesGenSymbols);
    }
};

class ArgList : public Base {
private:
    ProductionVector<Expression*>* expressions;
public:
    typedef std::vector<Expression*> Args;
    typedef Args::iterator ArgIterator;
    ArgList (ProductionVector<Expression*>* expressions) : Base(), expressions(expressions) {  }
    ~ArgList () { delete expressions; }
    ArgIterator begin () { return expressions->begin(); }
    ArgIterator end () { return expressions->end(); }
    size_t size () { return expressions->size(); }
    virtual void express(Expressor* p_expressor) const;
};
class FunctionCall : public Base {
private:
    URI* m_IRIref;
    ArgList* m_ArgList;
public:
    FunctionCall (URI* p_IRIref, ArgList* p_ArgList) : Base(), m_IRIref(p_IRIref), m_ArgList(p_ArgList) {  }
    FunctionCall (URI* p_IRIref, Expression* arg1, Expression* arg2, Expression* arg3) : Base() {
	m_IRIref = p_IRIref;
	ProductionVector<Expression*>* args = new ProductionVector<Expression*>();
	if (arg1) args->push_back(arg1);
	if (arg2) args->push_back(arg2);
	if (arg3) args->push_back(arg3);
	m_ArgList = new ArgList(args);
    }
    ~FunctionCall () { delete m_ArgList; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const {
	std::vector<const POS*> subd;
	for (ArgList::ArgIterator it = m_ArgList->begin(); it != m_ArgList->end(); ++it)
	    subd.push_back((*it)->eval(r, posFactory, bNodesGenSymbols));
	std::stringstream s;
	s << m_IRIref->toString() << '(';
	for (std::vector<const POS*>::iterator it = subd.begin(); it != subd.end(); ++it) {
	    if (it != subd.begin())
		s << ", ";
	    s << (*it)->toString();
	}
	s << ')';
	s << " not implemented";
	throw s.str();
    }
};
class FunctionCallExpression : public Expression {
private:
    FunctionCall* m_FunctionCall;
public:
    FunctionCallExpression (FunctionCall* p_FunctionCall) : Expression(), m_FunctionCall(p_FunctionCall) {  }
    ~FunctionCallExpression () { delete m_FunctionCall; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const {
	return m_FunctionCall->eval(r, posFactory, bNodesGenSymbols);
    }
};

/* Expressions */
/* Logical connectives: or and */
class UnaryExpression : public Expression {
protected:
    Expression* m_Expression;
public:
    UnaryExpression (Expression* p_Expression) : Expression(), m_Expression(p_Expression) {  }
    ~UnaryExpression () { delete m_Expression; }
    virtual const char* getUnaryOperator() = 0;
};
class NaryExpression : public Expression {
protected:
    ProductionVector<const Expression*> m_Expressions;
public:
    NaryExpression (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : Expression(), m_Expressions() {
	m_Expressions.push_back(p_Expression);
	for (std::vector<Expression*>::iterator it = p_Expressions->begin();
	     it != p_Expressions->end(); ++it)
	    m_Expressions.push_back(*it);
    }

    virtual const char* getInfixNotation() = 0;
};
class BooleanJunction : public NaryExpression {
public:
    BooleanJunction (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : NaryExpression(p_Expression, p_Expressions) { }
};
class BooleanConjunction : public BooleanJunction { // ⋀
public:
    BooleanConjunction (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : BooleanJunction(p_Expression, p_Expressions) {  }
    virtual const char* getInfixNotation () { return "&&"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const {
	for (std::vector<const Expression*>::const_iterator it = m_Expressions.begin(); it != m_Expressions.end(); ++it) {
	    const POS* ret = posFactory->ebv((*it)->eval(r, posFactory, bNodesGenSymbols));
	    if (ret != posFactory->getTrue())
		return ret;
	}
	return posFactory->getTrue();
    }
};
class BooleanDisjunction : public BooleanJunction { // ⋁
public:
    BooleanDisjunction (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : BooleanJunction(p_Expression, p_Expressions) {  }
    virtual const char* getInfixNotation () { return "||"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const {
	for (std::vector<const Expression*>::const_iterator it = m_Expressions.begin(); it != m_Expressions.end(); ++it) {
	    const POS* ret = posFactory->ebv((*it)->eval(r, posFactory, bNodesGenSymbols));
	    if (ret != posFactory->getFalse())
		return ret;
	}
	return posFactory->getFalse();
    }
};

class BooleanComparator : public Expression {
protected:
    Expression* left;
    Expression* right;
public:
    BooleanComparator (Expression* p_Expression) : Expression(), right(p_Expression) {  }
    ~BooleanComparator () { delete left; delete right; }
    virtual void setLeftParm (Expression* p_left) { left = p_left; }

    virtual const char* getComparisonNotation() = 0;
    virtual void express(Expressor* p_expressor) const = 0;
};
class BooleanEQ : public BooleanComparator {
public:
    BooleanEQ (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return "="; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	return left->eval(res, posFactory, bNodesGenSymbols) == 
	    right->eval(res, posFactory, bNodesGenSymbols) ? 
	    posFactory->getTrue() : posFactory->getFalse();
    }
};
class BooleanNE : public BooleanComparator {
public:
    BooleanNE (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return "!="; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* l = left->eval(res, posFactory, bNodesGenSymbols);
	const POS* r = right->eval(res, posFactory, bNodesGenSymbols);
	return l == r ? posFactory->getFalse() : posFactory->getTrue();
    }
};
class BooleanLT : public BooleanComparator {
public:
    BooleanLT (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return "<"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* l = left->eval(res, posFactory, bNodesGenSymbols);
	const POS* r = right->eval(res, posFactory, bNodesGenSymbols);
	return l < r ? posFactory->getFalse() : posFactory->getTrue();
    }
};
class BooleanGT : public BooleanComparator {
public:
    BooleanGT (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return ">"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* l = left->eval(res, posFactory, bNodesGenSymbols);
	const POS* r = right->eval(res, posFactory, bNodesGenSymbols);
	return l == r ? posFactory->getFalse() : 
	    r < l ? posFactory->getTrue() : posFactory->getFalse();
    }
};
class BooleanLE : public BooleanComparator {
public:
    BooleanLE (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return "<="; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* l = left->eval(res, posFactory, bNodesGenSymbols);
	const POS* r = right->eval(res, posFactory, bNodesGenSymbols);
	return l == r ? posFactory->getTrue() : 
	    l < r ? posFactory->getTrue() : posFactory->getFalse();
    }
};
class BooleanGE : public BooleanComparator {
public:
    BooleanGE (Expression* p_Expression) : BooleanComparator(p_Expression) {  }
    virtual const char* getComparisonNotation () { return ">="; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* l = left->eval(res, posFactory, bNodesGenSymbols);
	const POS* r = right->eval(res, posFactory, bNodesGenSymbols);
	return l == r ? posFactory->getTrue() : 
	    r < l ? posFactory->getTrue() : posFactory->getFalse();
    }
};
class ComparatorExpression : public Expression {
private:
    BooleanComparator* m_BooleanComparator;
public:
    ComparatorExpression (BooleanComparator* p_BooleanComparator) : Expression(), m_BooleanComparator(p_BooleanComparator) {  }
    ~ComparatorExpression () { delete m_BooleanComparator; }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* r, POSFactory* posFactory, bool bNodesGenSymbols) const {
	return m_BooleanComparator->eval(r, posFactory, bNodesGenSymbols);
    }
};
class BooleanNegation : public UnaryExpression {
public:
    BooleanNegation (Expression* p_PrimaryExpression) : UnaryExpression(p_PrimaryExpression) {  }
    ~BooleanNegation () {  }
    virtual const char* getUnaryOperator () { return "!"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	const POS* v = posFactory->ebv(m_Expression->eval(res, posFactory, bNodesGenSymbols));
	return v == posFactory->getTrue() ? posFactory->getFalse() : posFactory->getTrue();
    }
};
class ArithmeticSum : public NaryExpression {
public:
    ArithmeticSum (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : NaryExpression(p_Expression, p_Expressions) {  }
    virtual const char* getInfixNotation () { return "+"; };    
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	std::vector<const POS*> subd;
	for (std::vector<const Expression*>::const_iterator it = m_Expressions.begin();
	     it != m_Expressions.end(); ++it)
	    subd.push_back((*it)->eval(res, posFactory, bNodesGenSymbols));
	std::stringstream s;
	s << "(+ ";
	for (std::vector<const POS*>::const_iterator it = subd.begin(); it != subd.end(); ++it) {
	    if (it != subd.begin())
		s << ", ";
	    s << (*it)->toString();
	}
	s << ')';
	s << " not implemented";
	throw s.str();
    }
};
class ArithmeticNegation : public UnaryExpression {
public:
    ArithmeticNegation (Expression* p_MultiplicativeExpression) : UnaryExpression(p_MultiplicativeExpression) {  }
    ~ArithmeticNegation () {  }
    virtual const char* getUnaryOperator () { return "-"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	std::stringstream s;
	s << "(- _0 " << m_Expression->eval(res, posFactory, bNodesGenSymbols) <<
	    ')' << " not implemented";
	throw s.str();
    }
};
class NumberExpression : public Expression {
private:
    NumericRDFLiteral* m_NumericRDFLiteral;
public:
    NumberExpression (NumericRDFLiteral* p_NumericRDFLiteral) : Expression(), m_NumericRDFLiteral(p_NumericRDFLiteral) {  }
    ~NumberExpression () { /* m_NumericRDFLiteral is centrally managed */ }
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* /* posFactory */, bool bNodesGenSymbols) const {
	return m_NumericRDFLiteral->evalPOS(res, bNodesGenSymbols);
    }
};
class ArithmeticProduct : public NaryExpression {
public:
    ArithmeticProduct (Expression* p_Expression, ProductionVector<Expression*>* p_Expressions) : NaryExpression(p_Expression, p_Expressions) {  }
    virtual const char* getInfixNotation () { return "+"; };    
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	std::vector<const POS*> subd;
	for (std::vector<const Expression*>::const_iterator it = m_Expressions.begin();
	     it != m_Expressions.end(); ++it)
	    subd.push_back((*it)->eval(res, posFactory, bNodesGenSymbols));
	std::stringstream s;
	s << "(- ";
	for (std::vector<const POS*>::const_iterator it = subd.begin(); it != subd.end(); ++it) {
	    if (it != subd.begin())
		s << ", ";
	    s << (*it)->toString();
	}
	s << ')';
	s << " not implemented";
	throw s.str();
    }
};
class ArithmeticInverse : public UnaryExpression {
public:
    ArithmeticInverse (Expression* p_UnaryExpression) : UnaryExpression(p_UnaryExpression) {  }
    ~ArithmeticInverse () {  }
    virtual const char* getUnaryOperator () { return "1/"; };
    virtual void express(Expressor* p_expressor) const;
    virtual const POS* eval (const Result* res, POSFactory* posFactory, bool bNodesGenSymbols) const {
	std::stringstream s;
	s << "(/ 1 " << m_Expression->eval(res, posFactory, bNodesGenSymbols) <<
	    ')' << " not implemented";
	throw s.str();
    }
};

} // namespace w3c_sw
/* END ClassBlock */
#include <iostream>
namespace libwww {

/* URI parsing stuff stolen from libwww
 *
 */
class DummyHTURI {
private:
public:
    DummyHTURI () {  }
    virtual ~DummyHTURI () {  }
    virtual bool hasScheme () { return false; }
    virtual std::string getScheme () { throw(std::runtime_error("illegal call to DummyHTURI::getScheme")); }
    virtual void clearScheme () { throw(std::runtime_error("illegal call to DummyHTURI::clearScheme")); }
    virtual bool hasHost () { return false; }
    virtual std::string getHost () { throw(std::runtime_error("illegal call to DummyHTURI::getHost")); }
    virtual void clearHost () { throw(std::runtime_error("illegal call to DummyHTURI::clearHost")); }
    virtual bool hasAbsolute () { return false; }
    virtual std::string getAbsolute () { throw(std::runtime_error("illegal call to DummyHTURI::getAbsolute")); }
    virtual void clearAbsolute () { throw(std::runtime_error("illegal call to DummyHTURI::clearAbsolute")); }
    virtual bool hasRelative () { return false; }
    virtual std::string getRelative () { throw(std::runtime_error("illegal call to DummyHTURI::getRelative")); }
    virtual void clearRelative () { throw(std::runtime_error("illegal call to DummyHTURI::clearRelative")); }
    virtual bool hasFragment () { return false; }
    virtual std::string getFragment () { throw(std::runtime_error("illegal call to DummyHTURI::getFragment")); }
    virtual void clearFragment () { throw(std::runtime_error("illegal call to DummyHTURI::clearFragment")); }
};
class HTURI : public DummyHTURI {
private:
    std::string scheme;   bool schemeP;
    std::string host;	  bool hostP;
    std::string absolute; bool absoluteP;
    std::string relative; bool relativeP;
    std::string fragment; bool fragmentP;
public:
    HTURI(std::string);
    virtual bool hasScheme () { return schemeP; }
    virtual std::string getScheme () { return scheme; }
    virtual void clearScheme () { scheme.clear(); schemeP = false; }
    virtual bool hasHost () { return hostP; }
    virtual std::string getHost () { return host; }
    virtual void clearHost () { host.clear(); hostP = false; }
    virtual bool hasAbsolute () { return absoluteP; }
    virtual std::string getAbsolute () { return absolute; }
    virtual void clearAbsolute () { absolute.clear(); absoluteP = false; }
    virtual bool hasRelative () { return relativeP; }
    virtual std::string getRelative () { return relative; }
    virtual void clearRelative () { relative.clear(); relativeP = false; }
    virtual bool hasFragment () { return fragmentP; }
    virtual std::string getFragment () { return fragment; }
    virtual void clearFragment () { fragment.clear(); fragmentP = false; }
};
typedef enum {
    PARSE_scheme =		16,	/* Access scheme, e.g. "HTTP" */
    PARSE_host =		 8,	/* Host name, e.g. "www.w3.org" */
    PARSE_path =		 4,	/* URL Path, e.g. "pub/WWW/TheProject.html" */

    PARSE_view =                 2,      /* Fragment identifier, e.g. "news" */
    PARSE_fragment =             PARSE_view,
    PARSE_anchor =		 PARSE_view,

    PARSE_punctuation =	         1,	/* Include delimiters, e.g, "/" and ":" */
    PARSE_all =		        31
} e_PARSE_opts;

std::string HTParse(std::string name, const std::string* rel, e_PARSE_opts wanted);

} // namespace libwww

namespace w3c_sw {

class Expressor {
public:
    virtual ~Expressor () {  }

    virtual void base(const Base* const self, std::string productionName) = 0;

    virtual void uri(const URI* const self, std::string terminal) = 0;
    virtual void variable(const Variable* const self, std::string terminal) = 0;
    virtual void bnode(const BNode* const self, std::string terminal) = 0;
    virtual void rdfLiteral(const RDFLiteral* const self, std::string terminal, URI* datatype, LANGTAG* p_LANGTAG) = 0;
    virtual void rdfLiteral(const NumericRDFLiteral* const self, int p_value) = 0;
    virtual void rdfLiteral(const NumericRDFLiteral* const self, float p_value) = 0;
    virtual void rdfLiteral(const NumericRDFLiteral* const self, double p_value) = 0;
    virtual void rdfLiteral(const BooleanRDFLiteral* const self, bool p_value) = 0;
    virtual void nullpos(const NULLpos* const self) = 0;
    virtual void triplePattern(const TriplePattern* const self, POS* p_s, POS* p_p, POS* p_o) = 0;
    virtual void filter(const Filter* const self, Expression* p_Constraint) = 0;
    virtual void namedGraphPattern(const NamedGraphPattern* const self, POS* p_name, bool p_allOpts, const ProductionVector<TriplePattern*>* p_TriplePatterns, const ProductionVector<Filter*>* p_Filters) = 0;
    virtual void defaultGraphPattern(const DefaultGraphPattern* const self, bool p_allOpts, const ProductionVector<TriplePattern*>* p_TriplePatterns, const ProductionVector<Filter*>* p_Filters) = 0;
    virtual void tableConjunction(const TableConjunction* const self, const ProductionVector<TableOperation*>* p_TableOperations, const ProductionVector<Filter*>* p_Filters) = 0;
    virtual void tableDisjunction(const TableDisjunction* const self, const ProductionVector<TableOperation*>* p_TableOperations, const ProductionVector<Filter*>* p_Filters) = 0;
    virtual void optionalGraphPattern(const OptionalGraphPattern* const self, TableOperation* p_GroupGraphPattern) = 0;
    virtual void graphGraphPattern(const GraphGraphPattern* const self, POS* p_POS, TableOperation* p_GroupGraphPattern) = 0;
    virtual void posList(const POSList* const self, const ProductionVector<POS*>* p_POSs) = 0;
    virtual void starVarSet(const StarVarSet* const self) = 0;
    virtual void defaultGraphClause(const DefaultGraphClause* const self, POS* p_IRIref) = 0;
    virtual void namedGraphClause(const NamedGraphClause* const self, POS* p_IRIref) = 0;
    virtual void solutionModifier(const SolutionModifier* const self, std::vector<s_OrderConditionPair>* p_OrderConditions, int p_limit, int p_offset) = 0;
    virtual void binding(const Binding* const self, const ProductionVector<POS*>* values) = 0;
    virtual void bindingClause(const BindingClause* const self, POSList* p_Vars, const ProductionVector<Binding*>* p_Bindings) = 0;
    virtual void whereClause(const WhereClause* const self, TableOperation* p_GroupGraphPattern, BindingClause* p_BindingClause) = 0;
    virtual void select(const Select* const self, e_distinctness p_distinctness, VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) = 0;
    virtual void construct(const Construct* const self, DefaultGraphPattern* p_ConstructTemplate, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) = 0;
    virtual void describe(const Describe* const self, VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) = 0;
    virtual void ask(const Ask* const self, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause) = 0;
    virtual void replace(const Replace* const self, WhereClause* p_WhereClause, TableOperation* p_GraphTemplate) = 0;
    virtual void insert(const Insert* const self, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) = 0;
    virtual void del(const Delete* const self, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) = 0;
    virtual void load(const Load* const self, ProductionVector<URI*>* p_IRIrefs, URI* p_into) = 0;
    virtual void clear(const Clear* const self, URI* p__QGraphIRI_E_Opt) = 0;
    virtual void create(const Create* const self, e_Silence p_Silence, URI* p_GraphIRI) = 0;
    virtual void drop(const Drop* const self, e_Silence p_Silence, URI* p_GraphIRI) = 0;
    virtual void varExpression(const VarExpression* const self, const Variable* p_Variable) = 0;
    virtual void literalExpression(const LiteralExpression* const self, RDFLiteral* p_RDFLiteral) = 0;
    virtual void booleanExpression(const BooleanExpression* const self, BooleanRDFLiteral* p_BooleanRDFLiteral) = 0;
    virtual void uriExpression(const URIExpression* const self, URI* p_URI) = 0;
    virtual void argList(const ArgList* const self, ProductionVector<Expression*>* expressions) = 0;
    virtual void functionCall(const FunctionCall* const self, URI* p_IRIref, ArgList* p_ArgList) = 0;
    virtual void functionCallExpression(const FunctionCallExpression* const self, FunctionCall* p_FunctionCall) = 0;
/* Expressions */
    virtual void booleanNegation(const BooleanNegation* const self, Expression* p_Expression) = 0;
    virtual void arithmeticNegation(const ArithmeticNegation* const self, Expression* p_Expression) = 0;
    virtual void arithmeticInverse(const ArithmeticInverse* const self, Expression* p_Expression) = 0;
    virtual void booleanConjunction(const BooleanConjunction* const self, const ProductionVector<Expression*>* p_Expressions) = 0;
    virtual void booleanDisjunction(const BooleanDisjunction* const self, const ProductionVector<Expression*>* p_Expressions) = 0;
    virtual void arithmeticSum(const ArithmeticSum* const self, const ProductionVector<Expression*>* p_Expressions) = 0;
    virtual void arithmeticProduct(const ArithmeticProduct* const self, const ProductionVector<Expression*>* p_Expressions) = 0;
    virtual void booleanEQ(const BooleanEQ* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void booleanNE(const BooleanNE* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void booleanLT(const BooleanLT* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void booleanGT(const BooleanGT* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void booleanLE(const BooleanLE* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void booleanGE(const BooleanGE* const self, Expression* p_left, Expression* p_right) = 0;
    virtual void comparatorExpression(const ComparatorExpression* const self, BooleanComparator* p_BooleanComparator) = 0;
    virtual void numberExpression(const NumberExpression* const self, NumericRDFLiteral* p_NumericRDFLiteral) = 0;
};

/* RecursiveExpressor - default actions for expressor.
 * Use this Expressor when you don't feel like supplying all of the methods.
 */
class RecursiveExpressor : public Expressor {
public:
    virtual void uri (URI*, std::string) {  }
    virtual void variable (Variable*, std::string) {  }
    virtual void bnode (BNode*, std::string) {  }
    virtual void rdfLiteral (RDFLiteral*, std::string, URI* datatype, LANGTAG* p_LANGTAG) {
	if (datatype) datatype->express(this);
	if (p_LANGTAG) p_LANGTAG->express(this);
    }
    virtual void rdfLiteral (const NumericRDFLiteral*, int) {  }
    virtual void rdfLiteral (const NumericRDFLiteral*, float) {  }
    virtual void rdfLiteral (const NumericRDFLiteral*, double) {  }
    virtual void rdfLiteral (const BooleanRDFLiteral*, bool) {  }
    virtual void nullpos (const NULLpos* const) {  }
    virtual void triplePattern (const TriplePattern* const, POS* p_s, POS* p_p, POS* p_o) {
	p_s->express(this);
	p_p->express(this);
	p_o->express(this);
    }
    virtual void filter (const Filter* const, Expression* p_Constraint) {
	p_Constraint->express(this);
    }
    virtual void namedGraphPattern (NamedGraphPattern*, POS* p_name, bool /*p_allOpts*/, ProductionVector<TriplePattern*>* p_TriplePatterns, ProductionVector<Filter*>* p_Filters) {
	p_name->express(this);
	p_TriplePatterns->express(this);
	p_Filters->express(this);
    }
    virtual void defaultGraphPattern (DefaultGraphPattern*, bool /*p_allOpts*/, ProductionVector<TriplePattern*>* p_TriplePatterns, ProductionVector<Filter*>* p_Filters) {
	p_TriplePatterns->express(this);
	p_Filters->express(this);
    }
    virtual void tableConjunction (const TableConjunction* const, const ProductionVector<TableOperation*>* p_TableOperations, const ProductionVector<Filter*>* p_Filters) {
	p_TableOperations->express(this);
	p_Filters->express(this);
    }
    virtual void tableDisjunction (const TableDisjunction* const, const ProductionVector<TableOperation*>* p_TableOperations, const ProductionVector<Filter*>* p_Filters) {
	p_TableOperations->express(this);
	p_Filters->express(this);
    }
    virtual void optionalGraphPattern (const OptionalGraphPattern* const, TableOperation* p_GroupGraphPattern) {
	p_GroupGraphPattern->express(this);
    }
    virtual void graphGraphPattern (const GraphGraphPattern* const, POS* p_POS, TableOperation* p_GroupGraphPattern) {
	p_POS->express(this);
	p_GroupGraphPattern->express(this);
    }
    virtual void posList (const POSList* const, const ProductionVector<POS*>* p_POSs) {
	p_POSs->express(this);
    }
    virtual void starVarSet (const StarVarSet* const) {  }
    virtual void defaultGraphClause (const DefaultGraphClause* const, POS* p_IRIref) {
	p_IRIref->express(this);
    }
    virtual void namedGraphClause (const NamedGraphClause* const, POS* p_IRIref) {
	p_IRIref->express(this);
    }
    virtual void solutionModifier (SolutionModifier*, std::vector<s_OrderConditionPair>* p_OrderConditions, int, int) {
	if (p_OrderConditions)
	    for (size_t i = 0; i < p_OrderConditions->size(); i++)
		p_OrderConditions->at(i).expression->express(this);
    }
    virtual void binding (const Binding* const, const ProductionVector<POS*>* values) {//!!!
	for (std::vector<POS*>::const_iterator it = values->begin();
	     it != values->end(); ++it)
	    (*it)->express(this);
    }
    virtual void bindingClause (const BindingClause* const, POSList* p_Vars, const ProductionVector<Binding*>* p_Bindings) {
	p_Vars->express(this);
	p_Bindings->ProductionVector<Binding*>::express(this);
    }
    virtual void whereClause (const WhereClause* const, TableOperation* p_GroupGraphPattern, BindingClause* p_BindingClause) {
	p_GroupGraphPattern->express(this);
	if (p_BindingClause) p_BindingClause->express(this);
    }
    virtual void select (Select*, e_distinctness, VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	p_VarSet->express(this);
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void construct (const Construct* const, DefaultGraphPattern* p_ConstructTemplate, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	p_ConstructTemplate->express(this);
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void describe (const Describe* const, VarSet* p_VarSet, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	p_VarSet->express(this);
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void ask (const Ask* const, ProductionVector<DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause) {
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
    }
    virtual void replace (const Replace* const, WhereClause* p_WhereClause, TableOperation* p_GraphTemplate) {
	p_WhereClause->express(this);
	p_GraphTemplate->express(this);
    }
    virtual void insert (const Insert* const, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) {
	p_GraphTemplate->express(this);
	if (p_WhereClause) p_WhereClause->express(this);
    }
    virtual void del (const Delete* const, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) {
	p_GraphTemplate->express(this);
	p_WhereClause->express(this);
    }
    virtual void load (const Load* const, ProductionVector<URI*>* p_IRIrefs, URI* p_into) {
	p_IRIrefs->express(this);
	p_into->express(this);
    }
    virtual void clear (const Clear* const, URI* p__QGraphIRI_E_Opt) {
	p__QGraphIRI_E_Opt->express(this);
    }
    virtual void create (Create*, e_Silence, URI* p_GraphIRI) {
	p_GraphIRI->express(this);
    }
    virtual void drop (Drop*, e_Silence, URI* p_GraphIRI) {
	p_GraphIRI->express(this);
    }
    virtual void varExpression (const VarExpression* const, const Variable* p_Variable) {
	p_Variable->express(this);
    }
    virtual void literalExpression (const LiteralExpression* const, RDFLiteral* p_RDFLiteral) {
	p_RDFLiteral->express(this);
    }
    virtual void booleanExpression (const BooleanExpression* const, BooleanRDFLiteral* p_BooleanRDFLiteral) {
	p_BooleanRDFLiteral->express(this);
    }
    virtual void uriExpression (const URIExpression* const, URI* p_URI) {
	p_URI->express(this);
    }
    virtual void argList (const ArgList* const, ProductionVector<Expression*>* expressions) {
	expressions->express(this);
    }
    virtual void functionCall (const FunctionCall* const, URI* p_IRIref, ArgList* p_ArgList) {
	p_IRIref->express(this);
	p_ArgList->express(this);
    }
    virtual void functionCallExpression (const FunctionCallExpression* const, FunctionCall* p_FunctionCall) {
	p_FunctionCall->express(this);
    }
/* Expressions */
    virtual void booleanNegation (const BooleanNegation* const, Expression* p_Expression) {
	p_Expression->express(this);
    }
    virtual void arithmeticNegation (const ArithmeticNegation* const, Expression* p_Expression) {
	p_Expression->express(this);
    }
    virtual void arithmeticInverse (const ArithmeticInverse* const, Expression* p_Expression) {
	p_Expression->express(this);
    }
    virtual void booleanConjunction (const BooleanConjunction* const, const ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void booleanDisjunction (const BooleanDisjunction* const, const ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void booleanNegation (BooleanNegation*, ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void arithmeticSum (const ArithmeticSum* const, const ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void arithmeticProduct (const ArithmeticProduct* const, const ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void arithmeticInverse (ArithmeticInverse*, ProductionVector<Expression*>* p_Expressions) {
	p_Expressions->express(this);
    }
    virtual void booleanEQ (const BooleanEQ* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void booleanNE (const BooleanNE* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void booleanLT (const BooleanLT* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void booleanGT (const BooleanGT* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void booleanLE (const BooleanLE* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void booleanGE (const BooleanGE* const, Expression* p_left, Expression* p_right) {
	p_left->express(this);
	p_right->express(this);
    }
    virtual void comparatorExpression (const ComparatorExpression* const, BooleanComparator* p_BooleanComparator) {
	p_BooleanComparator->express(this);
    }
    virtual void numberExpression (const NumberExpression* const, NumericRDFLiteral* p_NumericRDFLiteral) {
	p_NumericRDFLiteral->express(this);
    }
};
class TestExpressor : public RecursiveExpressor {
    virtual void base (Base*, std::string) { throw(std::runtime_error("hit base in TestExpressor")); }
};

#ifdef _MSC_VER
    /* @@@ Temporary work-around for a build bug in MSVC++ where TurltSDriver
     *     isn't defined by including TurtleSParser/TurtleSParser.hpp .
     */
    void loadGraph(BasicGraphPattern* bgp, POSFactory* f, std::string mediaType, std::string baseURI, std::string fileName);
#endif /* _MSC_VER */

} //namespace w3c_sw



#endif /* ! defined SWOBJECTS_HH */
