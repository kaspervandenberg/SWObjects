
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C++ LALR(1) parser skeleton written by Akim Demaille.  */

#ifndef PARSER_HEADER_H
#pragma once

/* "%code requires" blocks.  */

/* Line 35 of lalr1.cc  */
#line 51 "lib/SPARQLfedParser/SPARQLfedParser.ypp"
 // ##bison2
/* Bison seems to test inclusion with PARSER_HEADER_H, rather than something
 * which varies by parser_class_name . Overriding with define specific to
 * this parser.
 */
#endif /* !PARSER_HEADER_H */
#ifndef SPARQLFED_PARSER_HPP
#define SPARQLFED_PARSER_HPP

#include "SWObjects.hpp"
#include "ParserCommon.hpp"

namespace w3c_sw {

class SPARQLfedScanner;

/** The Driver class brings together all components. It creates an instance of
 * the SPARQLfedParser and SPARQLfedScanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. */

class SPARQLfedDriver : public YaccDriver {
    friend class SPARQLfedParser;
protected:
    const TTerm* curSubject;
    const TTerm* curPredicate;
    BasicGraphPattern* curBGP;
    ParserFilter* curFilter;
    TableOperation* curOp; // needed to make right-descending tree for e.g. TriplesBlock? ( ( GraphPatternNotTriples | Filter ) '.'? TriplesBlock? )*
    BindingClause* curBindingsClause;
    Binding* curBinding;
    ProductionVector<const Expression*>* curExprList;
    const TTerm* curGraphName;
    ProductionVector<const Expression*> filterExpressions;

    void ensureBasicGraphPattern ( ) {
	if (curBGP == NULL) {
	    curBGP = curGraphName == NULL ? 
		static_cast<BasicGraphPattern*>(new DefaultGraphPattern()) : 
		static_cast<BasicGraphPattern*>(new NamedGraphPattern(curGraphName));
	    curOp = makeConjunction(curOp, curBGP);
	}
    }

    ParserFilter* saveFilter () {
	ParserFilter* ret = curFilter;
	curFilter = NULL;
	return ret;
    }

    void restoreFilter (ParserFilter* was) {
	if (curFilter != NULL) {
	    if (curOp == NULL)
		ensureBasicGraphPattern();
	    curFilter->setOp(curOp);
	    curOp = curFilter;
	}
	curFilter = was;
    }

    TableOperation* makeConjunction (TableOperation* l, TableOperation* r) {
	if (l != NULL) {
	    if (r != NULL) {
		TableConjunction* ret = new TableConjunction();
		ret->addTableOperation(l, unnestTree);
		ret->addTableOperation(r, unnestTree);
		return ret;
	    } else
		return l;
	}
	if (r != NULL)
	    return r;
	return NULL;
    }

    TableOperation* makeDisjunction (TableOperation* l, TableOperation* r) {
	TableDisjunction* ret = new TableDisjunction();
	ret->addTableOperation(l, unnestTree);
	ret->addTableOperation(r, unnestTree);
	return ret;
    }

public:
    SPARQLfedDriver(std::string baseURI, AtomFactory* atomFactory);
    ~SPARQLfedDriver();

    virtual bool parse(IStreamContext& in);

    /** Pointer to the current lexer instance, this is used to connect the
     * parser to the scanner. It is used in the yylex macro. */
    class SPARQLfedScanner* lexer;

    /** Reference to the object created during parsing of the SPARQL stream. */
    Operation* root;

    /** Control attributes. */
    bool unnestTree;	/* conj(conj(A, B), C) => conj(A, B, C) */
};

} // namespace w3c_sw

// %} // ##bison1



/* Line 35 of lalr1.cc  */
#line 153 "lib/SPARQLfedParser/SPARQLfedParser.hpp"


#include <string>
#include <iostream>
#include "stack.hh"


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace w3c_sw {

/* Line 35 of lalr1.cc  */
#line 167 "lib/SPARQLfedParser/SPARQLfedParser.hpp"
  class position;
  class location;

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // w3c_sw

/* Line 35 of lalr1.cc  */
#line 177 "lib/SPARQLfedParser/SPARQLfedParser.hpp"

#include "location.hh"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)		\
do {							\
  if (N)						\
    {							\
      (Current).begin = (Rhs)[1].begin;			\
      (Current).end   = (Rhs)[N].end;			\
    }							\
  else							\
    {							\
      (Current).begin = (Current).end = (Rhs)[0].end;	\
    }							\
} while (false)
#endif


/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace w3c_sw {

/* Line 35 of lalr1.cc  */
#line 225 "lib/SPARQLfedParser/SPARQLfedParser.hpp"

  /// A Bison parser.
  class SPARQLfedParser
  {
  public:
    /// Symbol semantic values.
#ifndef YYSTYPE
    union semantic_type
    {

/* Line 35 of lalr1.cc  */
#line 159 "lib/SPARQLfedParser/SPARQLfedParser.ypp"

    struct {const TTerm* subject; const TTerm* predicate;} p_SubjectPredicatePair;
    struct {int limit; int offset;} p_LimitOffsetPair;
    struct {const URI* uri; LANGTAG* langtag;} p_uri_or_langtag;
    struct {Operation* operation; WhereClause* whereClause;} p_OperationWhereClausePair;
    struct {e_distinctness distinctness; VarSet* varSet;} p_Project;
    struct {e_distinctness distinctness; const Expression* p_Expression;} p_DistExprPair;
    s_OrderConditionPair p_OrderConditionPair;
    std::vector<s_OrderConditionPair>* p_OrderConditions;
    TableConjunction* p_TableConjunction;
    TableDisjunction* p_TableDisjunction;

    /* Terminals */
    LANGTAG* p_LANGTAG;

    std::string* p_string;

    const NumericRDFLiteral* p_NumericRDFLiteral;
    const BooleanRDFLiteral* p_BooleanRDFLiteral;

    /* Productions */
    Operation* p_Operation;
    OperationSet* p_OperationSet;
    const Variable* p_Variable;

    Select* p_Select;
    e_distinctness p_distinctness;
    e_listModifier p_listModifier;
    e_Silence p_Silence;
    TTermList* p_TTermList;
    ExpressionAliasList* p_ExpressionAliaseList;
    ExpressionAlias* p_ExpressionAlias;
    VarSet* p_VarSet;
    ProductionVector<const DatasetClause*>* p_DatasetClauses;
    Construct* p_Construct;
    Describe* p_Describe;
    Ask* p_Ask;
    DatasetClause* p_DatasetClause;
    DefaultGraphClause* p_DefaultGraphClause;
    NamedGraphClause* p_NamedGraphClause;
    WhereClause* p_WhereClause;
    SolutionModifier* p_SolutionModifier;
    e_ASCorDESC p_e_ASCorDESC;
    BindingClause* p_BindingsClause;
    ProductionVector<const Binding*>* p_BindingValues;
    Binding* p_BindingValue;
    TableOperation* p_TableOperation;
    ProductionVector<const TableOperation*>* p_TableOperations;
    OptionalGraphPattern* p_OptionalGraphPattern;
    BasicGraphPattern* p_BasicGraphPattern;
    ParserFilter* p_ParserFilter;
    FunctionCall* p_FunctionCall;
    ArgList* p_ArgList;
    const TTerm* p_TTerm;
    ProductionVector<const TTerm*>* p_TTerms;
    Expression* p_Expression;
    ProductionVector<const Expression*>* p_Expressions;
    GeneralComparator* p_GeneralComparator;
    const URI* p_URI;
    ProductionVector<const URI*>* p_URIs;
    const RDFLiteral* p_RDFLiteral;
    const BNode* p_BNode;




/* Line 35 of lalr1.cc  */
#line 305 "lib/SPARQLfedParser/SPARQLfedParser.hpp"
    };
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;
    /// Tokens.
    struct token
    {
      /* Tokens.  */
   enum yytokentype {
     __EOF__ = 0,
     IT_DELETE = 258,
     IT_INSERT = 259,
     IT_GRAPH = 260,
     IT_SERVICE = 261,
     IT_LOAD = 262,
     IT_INTO = 263,
     IT_CLEAR = 264,
     IT_CREATE = 265,
     IT_SILENT = 266,
     IT_DROP = 267,
     IT_BASE = 268,
     IT_PREFIX = 269,
     IT_SELECT = 270,
     IT_DISTINCT = 271,
     IT_REDUCED = 272,
     GT_TIMES = 273,
     IT_CONSTRUCT = 274,
     IT_DESCRIBE = 275,
     IT_FROM = 276,
     IT_NAMED = 277,
     IT_ORDER = 278,
     IT_BY = 279,
     IT_ASC = 280,
     IT_DESC = 281,
     IT_LIMIT = 282,
     IT_OFFSET = 283,
     IT_BINDINGS = 284,
     IT_MEMBERS = 285,
     IT_STARTS = 286,
     IT_ENDS = 287,
     IT_ANY = 288,
     IT_UNORDERED = 289,
     GT_LCURLEY = 290,
     GT_RCURLEY = 291,
     IT_ASK = 292,
     IT_WHERE = 293,
     GT_LPAREN = 294,
     GT_RPAREN = 295,
     IT_UNDEF = 296,
     GT_DOT = 297,
     IT_OPTIONAL = 298,
     IT_MINUS = 299,
     IT_UNION = 300,
     IT_FILTER = 301,
     GT_COMMA = 302,
     GT_SEMI = 303,
     IT_a = 304,
     GT_LBRACKET = 305,
     GT_RBRACKET = 306,
     GT_OR = 307,
     GT_AND = 308,
     GT_EQUAL = 309,
     GT_NEQUAL = 310,
     GT_LT = 311,
     GT_GT = 312,
     GT_LE = 313,
     GT_GE = 314,
     GT_PLUS = 315,
     GT_MINUS = 316,
     GT_DIVIDE = 317,
     GT_NOT = 318,
     IT_IN = 319,
     GT_NOT_SPACECHAR_IN = 320,
     IT_IRI = 321,
     IT_URI = 322,
     IT_BNODE = 323,
     IT_COALESCE = 324,
     IT_IF = 325,
     IT_STRLANG = 326,
     IT_STRDT = 327,
     IT_EXISTS = 328,
     GT_NOT_SPACECHAR_EXISTS = 329,
     IT_SEPARATOR = 330,
     IT_STR = 331,
     IT_LANG = 332,
     IT_LANGMATCHES = 333,
     IT_DATATYPE = 334,
     IT_BOUND = 335,
     IT_sameTerm = 336,
     IT_isIRI = 337,
     IT_isURI = 338,
     IT_isBLANK = 339,
     IT_isLITERAL = 340,
     IT_REGEX = 341,
     GT_DTYPE = 342,
     IT_AS = 343,
     IT_GROUP = 344,
     IT_HAVING = 345,
     IT_COUNT = 346,
     IT_SUM = 347,
     IT_MIN = 348,
     IT_MAX = 349,
     IT_AVG = 350,
     IT_GROUP_CONCAT = 351,
     IT_SAMPLE = 352,
     IT_true = 353,
     IT_false = 354,
     INTEGER = 355,
     DECIMAL = 356,
     DOUBLE = 357,
     INTEGER_POSITIVE = 358,
     DECIMAL_POSITIVE = 359,
     DOUBLE_POSITIVE = 360,
     INTEGER_NEGATIVE = 361,
     DECIMAL_NEGATIVE = 362,
     DOUBLE_NEGATIVE = 363,
     STRING_LITERAL1 = 364,
     STRING_LITERAL_LONG1 = 365,
     STRING_LITERAL2 = 366,
     STRING_LITERAL_LONG2 = 367,
     IRI_REF = 368,
     PNAME_NS = 369,
     PNAME_LN = 370,
     BLANK_NODE_LABEL = 371,
     ANON = 372,
     VAR1 = 373,
     VAR2 = 374,
     LANGTAG = 375,
     NIL = 376
   };

    };
    /// Token type.
    typedef token::yytokentype token_type;

    /// Build a parser object.
    SPARQLfedParser (class SPARQLfedDriver& driver_yyarg);
    virtual ~SPARQLfedParser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

  private:
    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Generate an error message.
    /// \param state   the state where the error occurred.
    /// \param tok     the lookahead token.
    virtual std::string yysyntax_error_ (int yystate, int tok);

#if YYDEBUG
    /// \brief Report a symbol value on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_value_print_ (int yytype,
					 const semantic_type* yyvaluep,
					 const location_type* yylocationp);
    /// \brief Report a symbol on the debug stream.
    /// \param yytype       The token type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    virtual void yy_symbol_print_ (int yytype,
				   const semantic_type* yyvaluep,
				   const location_type* yylocationp);
#endif


    /// State numbers.
    typedef int state_type;
    /// State stack type.
    typedef stack<state_type>    state_stack_type;
    /// Semantic value stack type.
    typedef stack<semantic_type> semantic_stack_type;
    /// location stack type.
    typedef stack<location_type> location_stack_type;

    /// The state stack.
    state_stack_type yystate_stack_;
    /// The semantic value stack.
    semantic_stack_type yysemantic_stack_;
    /// The location stack.
    location_stack_type yylocation_stack_;

    /// Internal symbol numbers.
    typedef unsigned char token_number_type;
    /* Tables.  */
    /// For a state, the index in \a yytable_ of its portion.
    static const short int yypact_[];
    static const short int yypact_ninf_;

    /// For a state, default rule to reduce.
    /// Unless\a  yytable_ specifies something else to do.
    /// Zero means the default is an error.
    static const unsigned short int yydefact_[];

    static const short int yypgoto_[];
    static const short int yydefgoto_[];

    /// What to do in a state.
    /// \a yytable_[yypact_[s]]: what to do in state \a s.
    /// - if positive, shift that token.
    /// - if negative, reduce the rule which number is the opposite.
    /// - if zero, do what YYDEFACT says.
    static const short int yytable_[];
    static const short int yytable_ninf_;

    static const short int yycheck_[];

    /// For a state, its accessing symbol.
    static const unsigned short int yystos_[];

    /// For a rule, its LHS.
    static const unsigned short int yyr1_[];
    /// For a rule, its RHS length.
    static const unsigned char yyr2_[];

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#endif

#if YYERROR_VERBOSE
    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    virtual std::string yytnamerr_ (const char *n);
#endif

#if YYDEBUG
    /// A type to store symbol numbers and -1.
    typedef short int rhs_number_type;
    /// A `-1'-separated list of the rules' RHS.
    static const rhs_number_type yyrhs_[];
    /// For each rule, the index of the first RHS symbol in \a yyrhs_.
    static const unsigned short int yyprhs_[];
    /// For each rule, its source line number.
    static const unsigned short int yyrline_[];
    /// For each scanner token number, its symbol number.
    static const unsigned short int yytoken_number_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /* Debugging.  */
    int yydebug_;
    std::ostream* yycdebug_;
#endif

    /// Convert a scanner token number \a t to a symbol number.
    token_number_type yytranslate_ (int t);

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg        Why this token is reclaimed.
    /// \param yytype       The symbol type.
    /// \param yyvaluep     Its semantic value.
    /// \param yylocationp  Its location.
    inline void yydestruct_ (const char* yymsg,
			     int yytype,
			     semantic_type* yyvaluep,
			     location_type* yylocationp);

    /// Pop \a n symbols the three stacks.
    inline void yypop_ (unsigned int n = 1);

    /* Constants.  */
    static const int yyeof_;
    /* LAST_ -- Last index in TABLE_.  */
    static const int yylast_;
    static const int yynnts_;
    static const int yyempty_;
    static const int yyfinal_;
    static const int yyterror_;
    static const int yyerrcode_;
    static const int yyntokens_;
    static const unsigned int yyuser_token_number_max_;
    static const token_number_type yyundef_token_;

    /* User arguments.  */
    class SPARQLfedDriver& driver;
  };

/* Line 35 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // w3c_sw

/* Line 35 of lalr1.cc  */
#line 613 "lib/SPARQLfedParser/SPARQLfedParser.hpp"



#endif /* ! defined PARSER_HEADER_H */
