/* SPARQLAlgebraSerializer.hpp - simple SPARQL algebra for SPARQL compile trees.
 * Creates s-expressions similar to http://www.sparql.org/validator.html .
 *
 * $Id: SPARQLAlgebraSerializer.hpp,v 1.10 2008-11-13 20:53:54 eric Exp $
 */

#ifndef SPARQLAlgebraSerializer_H
#define SPARQLAlgebraSerializer_H

#include <stack>

namespace w3c_sw {

class SPARQLAlgebraSerializer : public ExpressorSerializer {
public:
    typedef enum { DEBUG_none, DEBUG_graphs } e_DEBUG;
    typedef enum {
	ALGEBRA_simple = 0, 
	/* options */
	ALGEBRA_binaryOpts = 1, 
	ALGEBRA_not_exists = 2, 
	/* compatibility modes */
	ALGEBRA_1_0 = 1, 
	ALGEBRA_arq = 3
    } e_ALGEBRA;
protected:
    std::stringstream ret;
    const ProductionVector<const Expression*>* injectFilter;
    bool normalizing; // no constructor switch for this yet.
    typedef enum {PREC_Low, PREC_Or = PREC_Low, 
		  PREC_And, 
		  PREC_EQ, PREC_NE, PREC_LT, PREC_GT, PREC_LE, PREC_GE, 
		  PREC_Plus, PREC_Minus, 
		  PREC_Times, PREC_Divide, 
		  PREC_Not, PREC_Pos, PREC_Neg, PREC_High = PREC_Neg} e_PREC;
    const char* tab;
    e_DEBUG debug;
    size_t depth;
    std::stack<e_PREC> precStack;
    const char* leadStr;
    e_ALGEBRA algebra;

    /* Simulate SPARQL semantics serialization:
     *   Joins test members and express as leftjoins where 2nd is an optional.
     *   Optionals outside of a join express as (leftjoin (table unit) ...).
     *
     */
    bool optionalIsChildOfJoin;


    void start (e_PREC prec) {
	if (prec < precStack.top())
	    ret << "(";
	precStack.push(prec);
    }
    void end () {
	e_PREC prec = precStack.top();
	precStack.pop();
	if (prec < precStack.top())
	    ret << ")";
    }
    void lead () {
	ret << leadStr;
	for (size_t i = 0; i < depth; i++)
	    ret << tab;
    }
    void lead (size_t p_depth) {
	ret << leadStr;
	for (size_t i = 0; i < p_depth; i++)
	    ret << tab;
    }
public:
    SPARQLAlgebraSerializer (e_ALGEBRA algebra = ALGEBRA_simple, const char* p_tab = "  ", e_DEBUG debug = DEBUG_none, const char* leadStr = "") : 
	injectFilter(NULL), normalizing(false), tab(p_tab), debug(debug), depth(0), precStack(), leadStr(leadStr), algebra(algebra), optionalIsChildOfJoin(false)
    { precStack.push(PREC_High); }
    virtual std::string str () { return ret.str(); }
    virtual void str (std::string seed) { ret.str(seed); }
    //!!!
    virtual void base (const Base* const, std::string productionName) { throw(std::runtime_error(productionName)); };

    virtual void uri (const URI* const, std::string lexicalValue) {
	ret << '<' << lexicalValue << '>';
    }
    virtual void variable (const Variable* const, std::string lexicalValue) {
	ret << '?' << lexicalValue;
    }
    virtual void bnode (const BNode* const, std::string lexicalValue) {
	ret << "_:" << lexicalValue; // rewrite when combining named BNodes from multiple docs?
    }
    virtual void rdfLiteral (const RDFLiteral* const, std::string lexicalValue, const URI* datatype, LANGTAG* p_LANGTAG) {
	ret << '"' << lexicalValue << '"';
	if (datatype != NULL) { ret << "^^<" << datatype->getLexicalValue() << '>'; }
	if (p_LANGTAG != NULL) { ret << '@' << p_LANGTAG->getLexicalValue(); }
    }
    virtual void rdfLiteral (const NumericRDFLiteral* const self, int p_value) {
	if (normalizing)
	    ret << p_value;
	else
	    ret << self->toString();
    }
    virtual void rdfLiteral (const NumericRDFLiteral* const self, float p_value) {
	if (normalizing)
	    ret << p_value;
	else
	    ret << self->toString();
    }
    virtual void rdfLiteral (const NumericRDFLiteral* const self, double p_value) {
	if (normalizing)
	    ret << p_value;
	else
	    ret << self->toString();
    }
    virtual void rdfLiteral (const BooleanRDFLiteral* const self, bool p_value) {
	if (normalizing)
	    ret << (p_value ? "true" : "false");
	else
	    ret << self->toString();
    }
    virtual void nullpos (const NULLpos* const) {
	ret << "NULL ";
    }
    virtual void triplePattern (const TriplePattern* const, const POS* p_s, const POS* p_p, const POS* p_o) {
	ret << "(triple ";
	p_s->express(this);
	ret << ' ';
	p_p->express(this);
	ret << ' ';
	p_o->express(this);
	ret << ")";
    }
    virtual void _exprlist (const ProductionVector<const Expression*>* p_Constraints) {
	if (p_Constraints->size() > 1)
	    ret << "(exprlist ";
	for (std::vector<const Expression*>::const_iterator it = p_Constraints->begin();
	     it != p_Constraints->end(); ++it) {
	    if (it != p_Constraints->begin())
		ret << " ";
	    (*it)->express(this);
	}
	if (p_Constraints->size() > 1)
	    ret << ")";
	ret << std::endl;
    }
    virtual void filter (const Filter* const, const TableOperation* p_op, const ProductionVector<const Expression*>* p_Constraints) {
#if 0
	injectFilter = p_Constraints;
#endif
	lead();
	ret << "(filter ";
	_exprlist(p_Constraints);
	++depth;
	if (p_op) p_op->express(this); else ret << "No nested operation!"; // !!!
	--depth;
	lead();
	ret << ")";
	ret << std::endl;
    }
    void injectFilters () {
	if (injectFilter != NULL) {
	    lead();
	    ret << "FILTER ";
	    injectFilter->express(this);
	    ret << std::endl;
	    injectFilter = NULL;
	}
    }
    void _BasicGraphPattern (const BasicGraphPattern* self, const ProductionVector<const TriplePattern*>* p_TriplePatterns, bool p_allOpts) {
	/* Note early return for "(table unit)". */
	if (p_TriplePatterns->size() == 0) {
	    ret << "(table unit)" << std::endl;
	    return;
	}
	ret << "(bgp";
	if (debug & DEBUG_graphs) ret << ' ' << self;
	if (p_TriplePatterns->size() > 1)
	    ret << std::endl;
	else
	    ret << " ";
	depth++;
	for (std::vector<const TriplePattern*>::const_iterator triple = p_TriplePatterns->begin();
	     triple != p_TriplePatterns->end(); triple++) {
	    if (p_TriplePatterns->size() > 1)
		lead();
	    if (p_allOpts)
		ret << "(optional " << std::endl << "  ";
	    depth++;
	    (*triple)->express(this);
	    if (p_TriplePatterns->size() > 1)
		ret << std::endl;
	    depth--;
	    if (p_allOpts)
		ret << ")" << std::endl;;
	    }
	injectFilters();
	depth--;
	if (p_TriplePatterns->size() > 1)
	    lead();
	ret << ")" << std::endl;
    }
    virtual void namedGraphPattern (const NamedGraphPattern* const self, const POS* p_name, bool p_allOpts, const ProductionVector<const TriplePattern*>* p_TriplePatterns) {
	lead();
	p_name->express(this);
	ret << ' ';
	_BasicGraphPattern(self, p_TriplePatterns, p_allOpts);
    }
    virtual void defaultGraphPattern (const DefaultGraphPattern* const self, bool p_allOpts, const ProductionVector<const TriplePattern*>* p_TriplePatterns) {
	lead();
	_BasicGraphPattern(self, p_TriplePatterns, p_allOpts);
    }
    virtual void tableDisjunction (const TableDisjunction* const self, const ProductionVector<const TableOperation*>* p_TableOperations) {
	lead();
	ret << "(union";
	if (debug & DEBUG_graphs) ret << ' ' << self;
	ret << std::endl;
	depth++;
	for (std::vector<const TableOperation*>::const_iterator it = p_TableOperations->begin();
	     it != p_TableOperations->end(); ++it) {
	    (*it)->express(this);
	}
	injectFilters();
	depth--;
	lead();
	ret << ")" << std::endl;
    }

    void recursiveJoiner (const ProductionVector<const TableOperation*>* p_TableOperations, std::vector<const TableOperation*>::const_reverse_iterator it) {

	const TableOperation* r = *it;
	const OptionalGraphPattern* opt = dynamic_cast<const OptionalGraphPattern*>(r);
	++it;
	if (it == std::vector<const TableOperation*>::const_reverse_iterator(p_TableOperations->begin())) {
	    if (opt != NULL) {
		lead(); ret << "(leftjoin" << std::endl;
		depth++;
		lead(); ret << "(table unit)" << std::endl;
		optionalIsChildOfJoin = true;
		opt->express(this);
		optionalIsChildOfJoin = false;
		depth--;
		lead(); ret << ")" << std::endl;
	    } else {
		r->express(this);
	    }
	} else {
	    lead(); ret << (opt != NULL ? "(leftjoin" : "(join") << std::endl;
	    depth++;
	    recursiveJoiner(p_TableOperations, it);
	    if (opt != NULL) {
		optionalIsChildOfJoin = true;
		opt->express(this);
		optionalIsChildOfJoin = false;
	    } else {
		r->express(this);
	    }
	    depth--;
	    lead(); ret << ")" << std::endl;
	}
    }

    virtual void tableConjunction (const TableConjunction* const, const ProductionVector<const TableOperation*>* p_TableOperations) {
	if (algebra & ALGEBRA_binaryOpts)
	    recursiveJoiner(p_TableOperations, std::vector<const TableOperation*>::const_reverse_iterator(p_TableOperations->end()));
	else {
	    lead(); ret << "(join" << std::endl;
	    depth++;
	    for (std::vector<const TableOperation*>::const_iterator it = p_TableOperations->begin();
		 it != p_TableOperations->end(); ++it)
		(*it)->express(this);
	    depth--;
	    lead(); ret << ")" << std::endl;
	}
    }
    virtual void optionalGraphPattern (const OptionalGraphPattern* const, const TableOperation* p_GroupGraphPattern, const ProductionVector<const Expression*>* p_Expressions) {
	if (algebra & ALGEBRA_binaryOpts) {
	    if (optionalIsChildOfJoin == false) {
		lead(); ret << "(leftjoin" << std::endl;
		depth++;
		lead(); ret << "(table unit)" << std::endl;
	    }
	    p_GroupGraphPattern->express(this);
	    if (p_Expressions->size() > 0) {
		lead();
		_exprlist(p_Expressions);
	    }
	    if (optionalIsChildOfJoin == false) {
		depth--;
		lead(); ret << ")" << std::endl;
	    }
	} else {
	    lead(); ret << "(optional" << std::endl;
	    depth++;
	    p_GroupGraphPattern->express(this);
	    if (p_Expressions->size() > 0)
		lead(), _exprlist(p_Expressions);
	    depth--;
	    lead(); ret << ")" << std::endl;
	}
    }
    virtual void minusGraphPattern (const MinusGraphPattern* const, const TableOperation* p_GroupGraphPattern) {
	lead();
	ret << "MINUS ";
	depth++;
	p_GroupGraphPattern->express(this);
	depth--;
    }
    void _nestedGraphPattern (const POS* p_POS, const TableOperation* p_GroupGraphPattern) {
	p_POS->express(this);
	ret << std::endl;
	depth++;
	p_GroupGraphPattern->express(this);
	depth--;
    }
    virtual void graphGraphPattern (const GraphGraphPattern* const self, const POS* p_POS, const TableOperation* p_GroupGraphPattern) {
	lead();
	ret << "GRAPH ";
	if (debug & DEBUG_graphs) ret << ' ' << self;
	_nestedGraphPattern(p_POS, p_GroupGraphPattern);
    }
    virtual void serviceGraphPattern (const ServiceGraphPattern* const self, const POS* p_POS, const TableOperation* p_GroupGraphPattern, POSFactory* /* posFactory */, bool /* lexicalCompare */) {
	lead();
	ret << "SERVICE ";
	if (debug & DEBUG_graphs) ret << ' ' << self;
	_nestedGraphPattern(p_POS, p_GroupGraphPattern);
    }
    virtual void posList (const POSList* const, const ProductionVector<const POS*>* p_POSs) {
	for (std::vector<const POS*>::const_iterator it = p_POSs->begin();
	     it != p_POSs->end(); ++it) {
	    (*it)->express(this);
	    ret << ' ';
	}
    }
    virtual void starVarSet (const StarVarSet* const) {
	ret << "* ";
    }
    virtual void defaultGraphClause (const DefaultGraphClause* const, const POS* p_IRIref) {
	ret << "FROM ";
	p_IRIref->express(this);
    }
    virtual void namedGraphClause (const NamedGraphClause* const, const POS* p_IRIref) {
	ret << "FROM NAMED ";
	p_IRIref->express(this);
    }
    virtual void solutionModifier (const SolutionModifier* const, std::vector<s_OrderConditionPair>* p_OrderConditions, int p_limit, int p_offset) {
	lead();
	if (p_limit != LIMIT_None) ret << "LIMIT " << p_limit << std::endl;
	if (p_offset != OFFSET_None) ret << "OFFSET " << p_offset << std::endl;
	if (p_OrderConditions) {
	    ret << "ORDER BY ";
	    for (size_t i = 0; i < p_OrderConditions->size(); i++) {
		if (p_OrderConditions->at(i).ascOrDesc == ORDER_Desc) ret << "DESC ";
		p_OrderConditions->at(i).expression->express(this);
	    }
	    ret << std::endl;
	}
    }
    virtual void binding (const Binding* const, const ProductionVector<const POS*>* values) {//!!!
	ret << "  { ";
	for (std::vector<const POS*>::const_iterator it = values->begin();
	     it != values->end(); ++it)
	    (*it)->express(this);
	ret << ')' << std::endl;
    }
    virtual void bindingClause (const BindingClause* const, POSList* p_Vars, const ProductionVector<const Binding*>* p_Bindings) {
	ret << "BINDINGS ";
	p_Vars->express(this);
	ret << '{' << std::endl; //!!!
	p_Bindings->ProductionVector<const Binding*>::express(this);
	ret << '}' << std::endl;
    }
    virtual void whereClause (const WhereClause* const, const TableOperation* p_GroupGraphPattern, const BindingClause* p_BindingClause) {
	p_GroupGraphPattern->express(this);
	if (p_BindingClause) p_BindingClause->express(this);
    }
    virtual void select (const Select* const, e_distinctness p_distinctness, VarSet* p_VarSet, ProductionVector<const DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	lead();
	ret << "SELECT ";
	if (p_distinctness == DIST_distinct) ret << "DISTINCT ";
	if (p_distinctness == DIST_reduced) ret << "REDUCED ";
	p_VarSet->express(this);
	p_DatasetClauses->express(this);
	ret << std::endl;
	lead();
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void construct (const Construct* const, DefaultGraphPattern* p_ConstructTemplate, ProductionVector<const DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	lead();
	ret << "CONSTRUCT ";
	p_ConstructTemplate->express(this);
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void describe (const Describe* const, VarSet* p_VarSet, ProductionVector<const DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause, SolutionModifier* p_SolutionModifier) {
	lead();
	ret << "DESCRIBE ";
	p_VarSet->express(this);
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	p_SolutionModifier->express(this);
    }
    virtual void ask (const Ask* const, ProductionVector<const DatasetClause*>* p_DatasetClauses, WhereClause* p_WhereClause) {
	lead();
	ret << "(ask" << std::endl;
	++depth;
	p_DatasetClauses->express(this);
	p_WhereClause->express(this);
	--depth;
	ret << ")" << std::endl;
    }
    virtual void replace (const Replace* const, WhereClause* p_WhereClause, TableOperation* p_GraphTemplate) {
	lead();
	ret << "REPLACE ";
	p_WhereClause->express(this);
	p_GraphTemplate->express(this);
    }
    virtual void insert (const Insert* const self, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) {
	lead();
	ret << "INSERT { ";
	if (debug & DEBUG_graphs) ret << self << ' ';
	p_GraphTemplate->express(this);
	if (p_WhereClause) p_WhereClause->express(this);
	ret << "}" << std::endl;
    }
    virtual void del (const Delete* const, TableOperation* p_GraphTemplate, WhereClause* p_WhereClause) {
	lead();
	ret << "DELETE { ";
	p_GraphTemplate->express(this);
	if (p_WhereClause) p_WhereClause->express(this);
	ret << "}" << std::endl;
    }
    virtual void load (const Load* const, ProductionVector<const URI*>* p_IRIrefs, const URI* p_into) {
	lead();
	ret << "LOAD ";
	p_IRIrefs->express(this);
	p_into->express(this);
    }
    virtual void clear (const Clear* const, const URI* p__QGraphIRI_E_Opt) {
	lead();
	ret << "CLEAR ";
	p__QGraphIRI_E_Opt->express(this);
    }
    virtual void create (const Create* const, e_Silence p_Silence, const URI* p_GraphIRI) {
	lead();
	ret << "CREATE ";
	if (p_Silence != SILENT_Yes) ret << "SILENT";
	p_GraphIRI->express(this);
    }
    virtual void drop (const Drop* const, e_Silence p_Silence, const URI* p_GraphIRI) {
	lead();
	ret << "DROP ";
	if (p_Silence != SILENT_Yes) ret << "SILENT";
	p_GraphIRI->express(this);
    }
    virtual void posExpression (const POSExpression* const, const POS* p_POS) {
	p_POS->express(this);
    }
    virtual void argList (const ArgList* const, ProductionVector<const Expression*>* expressions) {
	expressions->express(this);
    }
    virtual void functionCall (const FunctionCall* const, const URI* p_IRIref, const ArgList* p_ArgList) {

	if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-str"))
	    ret << "str";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-lang"))
	    ret << "lang";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-langMatches"))
	    ret << "langMatches";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-datatype"))
	    ret << "datatype";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-bound"))
	    ret << "bound";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-sameTerm"))
	    ret << "sameTerm";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-isIRI"))
	    ret << "isIRI";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-isIRI"))
	    ret << "isIRI";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-isBlank"))
	    ret << "isBlank";
	else if (p_IRIref->matches("http://www.w3.org/TR/rdf-sparql-query/#func-isLiteral"))
	    ret << "isLiteral";
	else
	    p_IRIref->express(this);
	ret << "(";
	p_ArgList->express(this);
	ret << ")";
    }
    virtual void functionCallExpression (const FunctionCallExpression* const, FunctionCall* p_FunctionCall) {
	p_FunctionCall->express(this);
    }
/* Expressions */
    virtual void booleanNegation (const BooleanNegation* const, const Expression* p_Expression) {
	start(PREC_Not);
	ret << '!';
	p_Expression->express(this);
	end();
    }
    virtual void arithmeticNegation (const ArithmeticNegation* const, const Expression* p_Expression) {
	start(PREC_Neg);
	ret << "- ";
	p_Expression->express(this);
	end();
    }
    virtual void arithmeticInverse (const ArithmeticInverse* const, const Expression* p_Expression) {
	start(PREC_Divide);
	ret << "1/";
	p_Expression->express(this);
	end();
    }
    virtual void booleanConjunction (const BooleanConjunction* const, const ProductionVector<const Expression*>* p_Expressions) {
	start(PREC_And);
	for (std::vector<const Expression*>::const_iterator it = p_Expressions->begin();
	     it != p_Expressions->end(); ++it) {
	    if (it != p_Expressions->begin())
		ret << " && ";
	    (*it)->express(this);
	}
	end();
    }
    virtual void booleanDisjunction (const BooleanDisjunction* const, const ProductionVector<const Expression*>* p_Expressions) {
	start(PREC_Or);
	for (std::vector<const Expression*>::const_iterator it = p_Expressions->begin();
	     it != p_Expressions->end(); ++it) {
	    if (it != p_Expressions->begin())
		ret << " || ";
	    (*it)->express(this);
	}
	end();
    }
    virtual void arithmeticSum (const ArithmeticSum* const, const ProductionVector<const Expression*>* p_Expressions) {
	start(PREC_Plus);
	for (std::vector<const Expression*>::const_iterator it = p_Expressions->begin();
	     it != p_Expressions->end(); ++it) {
	    if (it != p_Expressions->begin())
		ret << " + ";
	    (*it)->express(this);
	}
	end();
    }
    virtual void arithmeticProduct (const ArithmeticProduct* const, const ProductionVector<const Expression*>* p_Expressions) {
	start(PREC_Times);
	for (std::vector<const Expression*>::const_iterator it = p_Expressions->begin();
	     it != p_Expressions->end(); ++it) {
	    if (it != p_Expressions->begin())
		ret << " * ";
	    (*it)->express(this);
	}
	end();
    }
    virtual void booleanEQ (const BooleanEQ* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_EQ);
	ret << "= ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void booleanNE (const BooleanNE* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_NE);
	ret << "!= ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void booleanLT (const BooleanLT* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_LT);
	ret << "< ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void booleanGT (const BooleanGT* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_GT);
	ret << "> ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void booleanLE (const BooleanLE* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_LE);
	ret << "<= ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void booleanGE (const BooleanGE* const, const Expression* p_left, const Expression* p_right) {
	start(PREC_GE);
	ret << ">= ";
	p_left->express(this);
	ret << " ";
	p_right->express(this);
	end();
    }
    virtual void comparatorExpression (const ComparatorExpression* const, const BooleanComparator* p_BooleanComparator) {
	p_BooleanComparator->express(this);
    }
    virtual void numberExpression (const NumberExpression* const, const NumericRDFLiteral* p_NumericRDFLiteral) {
	p_NumericRDFLiteral->express(this);
    }
};

#ifdef STREAM_ALGEBRA
    inline std::ostream& operator<< (std::ostream& os, DefaultGraphPattern const& my) {
	SPARQLAlgebraSerializer s;
	((DefaultGraphPattern&)my).express(&s);
	return os << s.str();
    }

    inline std::ostream& operator<< (std::ostream& os, Operation const& my) {
	SPARQLAlgebraSerializer s;
	((Operation&)my).express(&s);
	return os << s.str();
    }
#endif /* STREAM_ALGEBRA */

}

#endif // SPARQLAlgebraSerializer_H

