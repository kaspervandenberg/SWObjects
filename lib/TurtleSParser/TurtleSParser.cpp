
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
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

// Take the name prefix into account.
#define yylex   w3c_swlex

/* First part of user declarations.  */


/* Line 311 of lalr1.cc  */
#line 43 "lib/TurtleSParser/TurtleSParser.cpp"


#include "TurtleSParser.hpp"

/* User implementation prologue.  */

/* Line 317 of lalr1.cc  */
#line 126 "lib/TurtleSParser/TurtleSParser.ypp"

#include "../TurtleSScanner.hpp"

/* Line 317 of lalr1.cc  */
#line 177 "lib/TurtleSParser/TurtleSParser.ypp"

#include "../TurtleSScanner.hpp"

/* this "connects" the bison parser in the driver to the flex scanner class
 * object. it defines the yylex() function call to pull the next token from the
 * current lexer object of the driver context. */
#undef yylex
#define yylex driver.lexer->lexWrapper


/* Line 317 of lalr1.cc  */
#line 68 "lib/TurtleSParser/TurtleSParser.cpp"

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


/* Line 380 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

namespace w3c_sw {

/* Line 380 of lalr1.cc  */
#line 137 "lib/TurtleSParser/TurtleSParser.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  TurtleSParser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  TurtleSParser::TurtleSParser (class TurtleSDriver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {
  }

  TurtleSParser::~TurtleSParser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  TurtleSParser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  TurtleSParser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  TurtleSParser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  TurtleSParser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  TurtleSParser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  TurtleSParser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  TurtleSParser::debug_level_type
  TurtleSParser::debug_level () const
  {
    return yydebug_;
  }

  void
  TurtleSParser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  TurtleSParser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 553 of lalr1.cc  */
#line 31 "lib/TurtleSParser/TurtleSParser.ypp"
{
    // initialize the initial location object
    yylloc.begin.filename = yylloc.end.filename = &driver.streamname;
}

/* Line 553 of lalr1.cc  */
#line 321 "lib/TurtleSParser/TurtleSParser.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 678 of lalr1.cc  */
#line 192 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	driver.root = (yysemantic_stack_[(2) - (2)].p_DefaultGraphPattern);
    }
    break;

  case 8:

/* Line 678 of lalr1.cc  */
#line 215 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	driver.setBase((yysemantic_stack_[(3) - (2)].p_URI)->getLexicalValue());
    }
    break;

  case 9:

/* Line 678 of lalr1.cc  */
#line 221 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	driver.ignorePrefix(true);
      }
    break;

  case 10:

/* Line 678 of lalr1.cc  */
#line 223 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	  driver.ignorePrefix(false);
      }
    break;

  case 11:

/* Line 678 of lalr1.cc  */
#line 225 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	  std::string prefix((yysemantic_stack_[(5) - (3)].p_URI)->getLexicalValue());
	  driver.addPrefix(prefix.substr(0, prefix.length()-1), (yysemantic_stack_[(5) - (5)].p_URI));
      }
    break;

  case 13:

/* Line 678 of lalr1.cc  */
#line 233 "lib/TurtleSParser/TurtleSParser.ypp"
    { (yyval.p_DefaultGraphPattern) = NULL; }
    break;

  case 14:

/* Line 678 of lalr1.cc  */
#line 234 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_DefaultGraphPattern) = (yysemantic_stack_[(1) - (1)].p_DefaultGraphPattern);
    }
    break;

  case 15:

/* Line 678 of lalr1.cc  */
#line 241 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_DefaultGraphPattern) = driver.neededBGP ? driver.neededBGP : (yysemantic_stack_[(2) - (2)].p_DefaultGraphPattern);
	driver.neededBGP = NULL;
    }
    break;

  case 16:

/* Line 678 of lalr1.cc  */
#line 249 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_DefaultGraphPattern) = (yysemantic_stack_[(2) - (2)].p_DefaultGraphPattern);
    }
    break;

  case 17:

/* Line 678 of lalr1.cc  */
#line 256 "lib/TurtleSParser/TurtleSParser.ypp"
    { (yyval.p_DefaultGraphPattern) = NULL; }
    break;

  case 19:

/* Line 678 of lalr1.cc  */
#line 261 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	driver.curSubject = (yysemantic_stack_[(1) - (1)].p_TTerm);
    }
    break;

  case 21:

/* Line 678 of lalr1.cc  */
#line 264 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	driver.curSubject = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;

  case 37:

/* Line 678 of lalr1.cc  */
#line 324 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	if (!driver.curBGP)
	    driver.neededBGP = driver.curBGP = new DefaultGraphPattern();
	driver.curBGP->addTriplePattern(driver.atomFactory->getTriple(driver.curSubject, driver.curPredicate, (yysemantic_stack_[(1) - (1)].p_TTerm)));
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_TTerm);
    }
    break;

  case 38:

/* Line 678 of lalr1.cc  */
#line 333 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_TTerm);
	driver.curPredicate = (yyval.p_TTerm);
    }
    break;

  case 39:

/* Line 678 of lalr1.cc  */
#line 337 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = driver.getURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#type");
	driver.curPredicate = (yyval.p_TTerm);
    }
    break;

  case 40:

/* Line 678 of lalr1.cc  */
#line 344 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_BNode) = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;

  case 41:

/* Line 678 of lalr1.cc  */
#line 347 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_BNode) = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;

  case 42:

/* Line 678 of lalr1.cc  */
#line 353 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_SubjectPredicatePair).subject = driver.curSubject;
	(yyval.p_SubjectPredicatePair).predicate = driver.curPredicate;
	driver.curSubject = driver.createBNode();
      }
    break;

  case 43:

/* Line 678 of lalr1.cc  */
#line 357 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	  (yyval.p_BNode) = (BNode*)driver.curSubject; // could store w/ type in ctx..
	  driver.curSubject = (yysemantic_stack_[(4) - (2)].p_SubjectPredicatePair).subject;
	  driver.curPredicate = (yysemantic_stack_[(4) - (2)].p_SubjectPredicatePair).predicate;
      }
    break;

  case 44:

/* Line 678 of lalr1.cc  */
#line 366 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_BNode) = driver.createBNode();
	const TTerm* tail = (yyval.p_BNode);
	ProductionVector<const TTerm*>* members = (yysemantic_stack_[(3) - (2)].p_TTerms);
	const URI* first = driver.getURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#first");
	const URI* rest = driver.getURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#rest");
	const TTerm* nil = driver.getURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil");
	if (!driver.curBGP)
	    driver.neededBGP = driver.curBGP = new DefaultGraphPattern();
	for (unsigned i = 0; i < members->size(); i++) {
	    driver.curBGP->addTriplePattern(driver.atomFactory->getTriple(tail, first, members->at(i)));
	    const TTerm* nextTail = i == members->size()-1 ? nil : driver.createBNode();
	    driver.curBGP->addTriplePattern(driver.atomFactory->getTriple(tail, rest, nextTail));
	    tail = nextTail;
	}
	members->clear();
	delete members;
    }
    break;

  case 45:

/* Line 678 of lalr1.cc  */
#line 388 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerms) = new ProductionVector<const TTerm*>((yysemantic_stack_[(1) - (1)].p_TTerm));
    }
    break;

  case 46:

/* Line 678 of lalr1.cc  */
#line 391 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yysemantic_stack_[(2) - (1)].p_TTerms)->push_back((yysemantic_stack_[(2) - (2)].p_TTerm));
	(yyval.p_TTerms) = (yysemantic_stack_[(2) - (1)].p_TTerms);
    }
    break;

  case 47:

/* Line 678 of lalr1.cc  */
#line 398 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_TTerm);
    }
    break;

  case 48:

/* Line 678 of lalr1.cc  */
#line 401 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;

  case 49:

/* Line 678 of lalr1.cc  */
#line 407 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_TTerm);
    }
    break;

  case 50:

/* Line 678 of lalr1.cc  */
#line 413 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 51:

/* Line 678 of lalr1.cc  */
#line 419 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 52:

/* Line 678 of lalr1.cc  */
#line 422 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_RDFLiteral);
    }
    break;

  case 53:

/* Line 678 of lalr1.cc  */
#line 425 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 54:

/* Line 678 of lalr1.cc  */
#line 428 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_BooleanRDFLiteral);
    }
    break;

  case 55:

/* Line 678 of lalr1.cc  */
#line 431 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_TTerm);
    }
    break;

  case 56:

/* Line 678 of lalr1.cc  */
#line 434 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = driver.getURI("http://www.w3.org/1999/02/22-rdf-syntax-ns#nil"); // !!! new GraphTerm_rule5($1);
    }
    break;

  case 57:

/* Line 678 of lalr1.cc  */
#line 441 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_RDFLiteral) = driver.getRDFLiteral(*(yysemantic_stack_[(2) - (1)].p_string), (yysemantic_stack_[(2) - (2)].p_uri_or_langtag).uri, (yysemantic_stack_[(2) - (2)].p_uri_or_langtag).langtag);
	delete (yysemantic_stack_[(2) - (1)].p_string);
    }
    break;

  case 58:

/* Line 678 of lalr1.cc  */
#line 449 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_URI) = (yysemantic_stack_[(2) - (2)].p_URI);
    }
    break;

  case 59:

/* Line 678 of lalr1.cc  */
#line 456 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_uri_or_langtag).uri = NULL;
	(yyval.p_uri_or_langtag).langtag = (yysemantic_stack_[(1) - (1)].p_LANGTAG);
    }
    break;

  case 60:

/* Line 678 of lalr1.cc  */
#line 460 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_uri_or_langtag).uri = (yysemantic_stack_[(1) - (1)].p_URI);
	(yyval.p_uri_or_langtag).langtag = NULL;
    }
    break;

  case 61:

/* Line 678 of lalr1.cc  */
#line 468 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_uri_or_langtag).uri = NULL;
	(yyval.p_uri_or_langtag).langtag = NULL;
    }
    break;

  case 62:

/* Line 678 of lalr1.cc  */
#line 472 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_uri_or_langtag) = (yysemantic_stack_[(1) - (1)].p_uri_or_langtag);
    }
    break;

  case 63:

/* Line 678 of lalr1.cc  */
#line 478 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 64:

/* Line 678 of lalr1.cc  */
#line 481 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 65:

/* Line 678 of lalr1.cc  */
#line 484 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 66:

/* Line 678 of lalr1.cc  */
#line 490 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 67:

/* Line 678 of lalr1.cc  */
#line 493 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 68:

/* Line 678 of lalr1.cc  */
#line 496 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 69:

/* Line 678 of lalr1.cc  */
#line 502 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 70:

/* Line 678 of lalr1.cc  */
#line 505 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 71:

/* Line 678 of lalr1.cc  */
#line 508 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 72:

/* Line 678 of lalr1.cc  */
#line 514 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 73:

/* Line 678 of lalr1.cc  */
#line 517 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 74:

/* Line 678 of lalr1.cc  */
#line 520 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_NumericRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_NumericRDFLiteral);
    }
    break;

  case 75:

/* Line 678 of lalr1.cc  */
#line 526 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_BooleanRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_BooleanRDFLiteral);
    }
    break;

  case 76:

/* Line 678 of lalr1.cc  */
#line 529 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_BooleanRDFLiteral) = (yysemantic_stack_[(1) - (1)].p_BooleanRDFLiteral);
    }
    break;

  case 77:

/* Line 678 of lalr1.cc  */
#line 535 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_string) = (yysemantic_stack_[(1) - (1)].p_string);
    }
    break;

  case 78:

/* Line 678 of lalr1.cc  */
#line 538 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_string) = (yysemantic_stack_[(1) - (1)].p_string);
    }
    break;

  case 79:

/* Line 678 of lalr1.cc  */
#line 541 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_string) = (yysemantic_stack_[(1) - (1)].p_string);
    }
    break;

  case 80:

/* Line 678 of lalr1.cc  */
#line 544 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_string) = (yysemantic_stack_[(1) - (1)].p_string);
    }
    break;

  case 81:

/* Line 678 of lalr1.cc  */
#line 550 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_URI) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 82:

/* Line 678 of lalr1.cc  */
#line 553 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_URI) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 83:

/* Line 678 of lalr1.cc  */
#line 559 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_URI) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 84:

/* Line 678 of lalr1.cc  */
#line 562 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_URI) = (yysemantic_stack_[(1) - (1)].p_URI);
    }
    break;

  case 85:

/* Line 678 of lalr1.cc  */
#line 568 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;

  case 86:

/* Line 678 of lalr1.cc  */
#line 571 "lib/TurtleSParser/TurtleSParser.ypp"
    {
	(yyval.p_TTerm) = (yysemantic_stack_[(1) - (1)].p_BNode);
    }
    break;



/* Line 678 of lalr1.cc  */
#line 1025 "lib/TurtleSParser/TurtleSParser.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  TurtleSParser::yysyntax_error_ (int yystate, int tok)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char TurtleSParser::yypact_ninf_ = -53;
  const signed char
  TurtleSParser::yypact_[] =
  {
         3,   -17,    35,    39,   -53,   -53,    30,   -53,    39,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,    32,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,    63,   -53,   -53,   -53,    38,
     -53,   -53,    -1,   -53,   -53,    65,    39,   -53,   -53,    65,
      65,   -20,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,    31,    39,   -53,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,    13,   -53,   -53,   -53,   -53,   -53,    40,    43,    16,
      65,   -53,    39,   -53,   -53,   -53,   -53,    39,   -53,    41,
     -53,   -53
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  TurtleSParser::yydefact_[] =
  {
         4,     0,     0,    13,     6,     5,     0,     1,     0,    42,
      75,    76,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    77,    78,    79,    80,    81,    84,    83,    85,    86,
      56,     2,    14,    17,    21,    41,    40,    19,    49,    52,
      53,    63,    64,    65,    54,    61,    51,    82,    55,     3,
       8,    48,     0,    45,    47,     0,    13,    18,    15,    31,
       0,     0,    59,    60,    62,    57,     9,     7,    44,    46,
      39,     0,     0,    38,    50,    16,    32,    22,    30,    20,
      58,     0,    43,    28,    35,    37,    10,    23,    33,     0,
      25,    29,     0,    36,    11,    26,    27,     0,    34,     0,
      24,    12
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  TurtleSParser::yypgoto_[] =
  {
       -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,
      -4,   -53,   -53,   -53,   -53,   -53,   -53,   -19,   -53,   -53,
     -53,   -53,   -53,   -53,   -18,   -53,   -53,   -15,   -10,    -3,
     -53,   -53,   -53,   -53,    -6,    -2,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -52,   -53,
     -53
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  TurtleSParser::yydefgoto_[] =
  {
        -1,     2,     3,     4,    49,     5,    67,    81,    89,    99,
      31,    32,    57,    58,    33,    60,    59,    71,    95,    96,
      91,    87,    77,    78,    83,    93,    88,    84,    72,    51,
      35,    55,    36,    52,    85,    54,    73,    38,    39,    63,
      64,    65,    40,    41,    42,    43,    44,    45,    46,    47,
      48
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char TurtleSParser::yytable_ninf_ = -1;
  const unsigned char
  TurtleSParser::yytable_[] =
  {
        34,    37,    53,    74,     8,    68,     1,    74,    74,    80,
       9,    25,    26,    27,     6,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,     7,    30,    50,    74,    56,
      76,    79,    66,    82,     8,    86,    69,    94,   101,    90,
       9,    92,    75,    34,    37,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    70,    30,    98,    61,   100,
      97,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    25,    26,    27,    62
  };

  /* YYCHECK.  */
  const signed char
  TurtleSParser::yycheck_[] =
  {
         3,     3,     8,    55,     5,     6,     3,    59,    60,    61,
      11,    31,    32,    33,    31,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,     0,    37,     7,    90,     7,
      59,    60,     4,    12,     5,    32,    52,    31,     7,     9,
      11,     8,    56,    56,    56,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    10,    37,    92,    15,    97,
      90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    31,    32,    33,    36
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  TurtleSParser::yystos_[] =
  {
         0,     3,    39,    40,    41,    43,    31,     0,     5,    11,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      37,    48,    49,    52,    67,    68,    70,    73,    75,    76,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    42,
       7,    67,    71,    72,    73,    69,     7,    50,    51,    54,
      53,    15,    36,    77,    78,    79,     4,    44,     6,    72,
      10,    55,    66,    74,    86,    48,    55,    60,    61,    55,
      86,    45,    12,    62,    65,    72,    32,    59,    64,    46,
       9,    58,     8,    63,    31,    56,    57,    66,    65,    47,
      62,     7
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  TurtleSParser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  TurtleSParser::yyr1_[] =
  {
         0,    38,    39,    40,    41,    41,    42,    42,    43,    45,
      46,    47,    44,    48,    48,    49,    50,    51,    51,    53,
      52,    54,    52,    55,    56,    57,    57,    58,    59,    59,
      60,    61,    61,    62,    63,    64,    64,    65,    66,    66,
      67,    67,    69,    68,    70,    71,    71,    72,    72,    73,
      74,    75,    75,    75,    75,    75,    75,    76,    77,    78,
      78,    79,    79,    80,    80,    80,    81,    81,    81,    82,
      82,    82,    83,    83,    83,    84,    84,    85,    85,    85,
      85,    86,    86,    87,    87,    88,    88
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  TurtleSParser::yyr2_[] =
  {
         0,     2,     2,     2,     0,     1,     0,     2,     3,     0,
       0,     0,     7,     0,     1,     2,     2,     0,     1,     0,
       3,     0,     3,     3,     2,     0,     1,     2,     0,     2,
       1,     0,     1,     2,     2,     0,     2,     1,     1,     1,
       1,     1,     0,     4,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const TurtleSParser::yytname_[] =
  {
    "\"end of file\"", "error", "$undefined", "IT_BASE", "IT_PREFIX",
  "GT_LPAREN", "GT_RPAREN", "GT_DOT", "GT_COMMA", "GT_SEMI", "IT_a",
  "GT_LBRACKET", "GT_RBRACKET", "GT_PLUS", "GT_MINUS", "GT_DTYPE",
  "IT_true", "IT_false", "INTEGER", "DECIMAL", "DOUBLE",
  "INTEGER_POSITIVE", "DECIMAL_POSITIVE", "DOUBLE_POSITIVE",
  "INTEGER_NEGATIVE", "DECIMAL_NEGATIVE", "DOUBLE_NEGATIVE",
  "STRING_LITERAL1", "STRING_LITERAL2", "STRING_LITERAL_LONG1",
  "STRING_LITERAL_LONG2", "IRI_REF", "PNAME_NS", "PNAME_LN",
  "BLANK_NODE_LABEL", "ANON", "LANGTAG", "NIL", "$accept", "Query",
  "Prologue", "_QBaseDecl_E_Opt", "_QPrefixDecl_E_Star", "BaseDecl",
  "PrefixDecl", "$@1", "$@2", "$@3", "_QConstructTriples_E_Opt",
  "ConstructTriples", "_O_QGT_DOT_E_S_QConstructTriples_E_Opt_C",
  "_Q_O_QGT_DOT_E_S_QConstructTriples_E_Opt_C_E_Opt", "TriplesSameSubject",
  "$@4", "$@5", "PropertyListNotEmpty", "_O_QVerb_E_S_QObjectList_E_C",
  "_Q_O_QVerb_E_S_QObjectList_E_C_E_Opt",
  "_O_QGT_SEMI_E_S_QVerb_E_S_QObjectList_E_Opt_C",
  "_Q_O_QGT_SEMI_E_S_QVerb_E_S_QObjectList_E_Opt_C_E_Star", "PropertyList",
  "_QPropertyListNotEmpty_E_Opt", "ObjectList",
  "_O_QGT_COMMA_E_S_QObject_E_C", "_Q_O_QGT_COMMA_E_S_QObject_E_C_E_Star",
  "Object", "Verb", "TriplesNode", "BlankNodePropertyList", "@6",
  "Collection", "_QGraphNode_E_Plus", "GraphNode", "VarOrTerm",
  "VarOrIRIref", "GraphTerm", "RDFLiteral", "_O_QGT_DTYPE_E_S_QIRIref_E_C",
  "_O_QLANGTAG_E_Or_QGT_DTYPE_E_S_QIRIref_E_C",
  "_Q_O_QLANGTAG_E_Or_QGT_DTYPE_E_S_QIRIref_E_C_E_Opt", "NumericLiteral",
  "NumericLiteralUnsigned", "NumericLiteralPositive",
  "NumericLiteralNegative", "BooleanLiteral", "String", "IRIref",
  "PrefixedName", "BlankNode", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const TurtleSParser::rhs_number_type
  TurtleSParser::yyrhs_[] =
  {
        39,     0,    -1,    40,    48,    -1,    41,    42,    -1,    -1,
      43,    -1,    -1,    42,    44,    -1,     3,    31,     7,    -1,
      -1,    -1,    -1,     4,    45,    32,    46,    31,    47,     7,
      -1,    -1,    49,    -1,    52,    51,    -1,     7,    48,    -1,
      -1,    50,    -1,    -1,    73,    53,    55,    -1,    -1,    67,
      54,    60,    -1,    66,    62,    59,    -1,    66,    62,    -1,
      -1,    56,    -1,     9,    57,    -1,    -1,    59,    58,    -1,
      61,    -1,    -1,    55,    -1,    65,    64,    -1,     8,    65,
      -1,    -1,    64,    63,    -1,    72,    -1,    74,    -1,    10,
      -1,    70,    -1,    68,    -1,    -1,    11,    69,    55,    12,
      -1,     5,    71,     6,    -1,    72,    -1,    71,    72,    -1,
      73,    -1,    67,    -1,    75,    -1,    86,    -1,    86,    -1,
      76,    -1,    80,    -1,    84,    -1,    88,    -1,    37,    -1,
      85,    79,    -1,    15,    86,    -1,    36,    -1,    77,    -1,
      -1,    78,    -1,    81,    -1,    82,    -1,    83,    -1,    18,
      -1,    19,    -1,    20,    -1,    21,    -1,    22,    -1,    23,
      -1,    24,    -1,    25,    -1,    26,    -1,    16,    -1,    17,
      -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,    31,
      -1,    87,    -1,    33,    -1,    32,    -1,    34,    -1,    35,
      -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  TurtleSParser::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    10,    12,    13,    16,    20,
      21,    22,    23,    31,    32,    34,    37,    40,    41,    43,
      44,    48,    49,    53,    57,    60,    61,    63,    66,    67,
      70,    72,    73,    75,    78,    81,    82,    85,    87,    89,
      91,    93,    95,    96,   101,   105,   107,   110,   112,   114,
     116,   118,   120,   122,   124,   126,   128,   130,   133,   136,
     138,   140,   141,   143,   145,   147,   149,   151,   153,   155,
     157,   159,   161,   163,   165,   167,   169,   171,   173,   175,
     177,   179,   181,   183,   185,   187,   189
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  TurtleSParser::yyrline_[] =
  {
         0,   192,   192,   199,   203,   205,   209,   211,   215,   221,
     223,   225,   221,   233,   234,   241,   249,   256,   257,   261,
     261,   264,   264,   271,   276,   280,   282,   287,   291,   293,
     298,   302,   304,   309,   314,   318,   320,   324,   333,   337,
     344,   347,   353,   353,   366,   388,   391,   398,   401,   407,
     413,   419,   422,   425,   428,   431,   434,   441,   449,   456,
     460,   468,   472,   478,   481,   484,   490,   493,   496,   502,
     505,   508,   514,   517,   520,   526,   529,   535,   538,   541,
     544,   550,   553,   559,   562,   568,   571
  };

  // Print the state stack on the debug stream.
  void
  TurtleSParser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  TurtleSParser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  TurtleSParser::token_number_type
  TurtleSParser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int TurtleSParser::yyeof_ = 0;
  const int TurtleSParser::yylast_ = 99;
  const int TurtleSParser::yynnts_ = 51;
  const int TurtleSParser::yyempty_ = -2;
  const int TurtleSParser::yyfinal_ = 7;
  const int TurtleSParser::yyterror_ = 1;
  const int TurtleSParser::yyerrcode_ = 256;
  const int TurtleSParser::yyntokens_ = 38;

  const unsigned int TurtleSParser::yyuser_token_number_max_ = 292;
  const TurtleSParser::token_number_type TurtleSParser::yyundef_token_ = 2;


/* Line 1054 of lalr1.cc  */
#line 1 "[Bison:b4_percent_define_default]"

} // w3c_sw

/* Line 1054 of lalr1.cc  */
#line 1573 "lib/TurtleSParser/TurtleSParser.cpp"


/* Line 1056 of lalr1.cc  */
#line 578 "lib/TurtleSParser/TurtleSParser.ypp"
 /*** Additional Code ***/

void w3c_sw::TurtleSParser::error(const TurtleSParser::location_type& l,
			    const std::string& m)
{
    driver.error(l, m);
}

/* START Driver (@@ stand-alone would allow it to be shared with other parsers */

namespace w3c_sw {

void TurtleSDriver::parse (IStreamContext& in) {
    streamname = in.nameStr;

    TurtleSScanner scanner(this, in.p);
    scanner.set_debug(trace_scanning);
    this->lexer = &scanner;

    TurtleSParser parser(*this);
    parser.set_debug_level(trace_parsing);
    parser.parse();
}

void TurtleSDriver::parse (IStreamContext& in, BasicGraphPattern* bgp) {
    setGraph(bgp);
    parse(in);
}

void TurtleSDriver::parse (IStreamContext& in, RdfDB* db) {
    setGraph(db->ensureGraph(NULL));
    parse(in);
}

void TurtleSDriver::parse(std::string turtleStr) {
    IStreamContext in(turtleStr.c_str(), IStreamContext::STRING);
    parse(in);
}

void TurtleSDriver::parse (std::string turtleStr, BasicGraphPattern* bgp) {
    IStreamContext in(turtleStr.c_str(), IStreamContext::STRING);
    setGraph(bgp);
    parse(in);
}

void TurtleSDriver::parse (std::string turtleStr, RdfDB* db) {
    IStreamContext in(turtleStr.c_str(), IStreamContext::STRING);
    setGraph(db->ensureGraph(NULL));
    parse(in);
}

} // namespace w3c_sw

/* END Driver */


