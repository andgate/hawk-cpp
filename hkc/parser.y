%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%defines
%define parser_class_name { hawk_parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{

#include <vector>
#include <memory>
class hawk_driver;

class CodeGenContext;
class NStatement;
class NExpression;
class NIdentifier;
class NBlock;

typedef std::shared_ptr<NBlock> block;
typedef std::shared_ptr<NStatement> stmt;
typedef std::shared_ptr<NIdentifier> ident;
typedef std::shared_ptr<NExpression> expr;

typedef std::vector<std::shared_ptr<NStatement>> StatementList;
typedef std::vector<std::shared_ptr<NExpression>> ExpressionList;
typedef std::vector<std::shared_ptr<NIdentifier>> IdentList;

}

// The parsing context.
%param { hawk_driver& driver }
%locations
%initial-action
{
  // Initialize the initial location.
  
  @$.begin.filename = @$.end.filename = &driver.file;
};

%define parse.trace
%define parse.error verbose

%code
{
# include "hkc/parser/driver.h"
# include "hkc/parser/node.h"

# include <string>
}

%define api.token.prefix {TOK_}
%token
  END  0    "end of file"
  AT        "@"
  SEMICOLON ";"
  COLON     ":"
  EXCLAIM   "!"
  FUNDEC    ":="
  TYPDEC    ":-"
  LCURLY    "{"
  RCURLY    "}"
  LARROW    "<-"
  RARROW    "->"
  BAR       "|"
  STAR      "*"
  SLASH     "/"
  PLUS      "+" 
  MINUS     "-"
  LPAREN    "("
  RPAREN    ")"
  RETURN    "return"
;

%token <std::string> IDENT
%token <std::string> STRING INTEGER DECIMAL

%printer { yyoutput << (&$$); } <*>;


%type <block> top_stmts block stmts
%type <stmt> top_stmt stmt func_decl function
%type <ident> ident
%type <IdentList> idents type_sig options
%type <expr> numeric expr func_call
%type <ExpressionList> exprs

%%


%start module;
module : %empty        { driver.result = NULL; }
       | top_stmts END { driver.result = $1; }
       ;
       
top_stmts : top_stmt           { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<stmt>1); }
          | top_stmts top_stmt { $1->statements.push_back($<stmt>2); std::swap($$, $1); }
          ;

top_stmt : function { std::swap($$, $1); };

options : "@" idents ";" { std::swap($$, $2); };
          
stmts : stmt       { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); std::swap($$, $1); }
      ;

stmt : expr ";"        { $$ = std::make_shared<NExpressionStatement>($1);   }
     | RETURN expr ";" { $$ = std::make_shared<NReturnStatement>($<expr>2); }
     | function        { std::swap($$, $1);                                 }
     ;
     
     
block : LCURLY RCURLY       { $$ = std::make_shared<NBlock>(); }
      | LCURLY stmts RCURLY { std::swap($$, $2); }
      ;

function : func_decl         { std::swap($$, $1); }
         | options func_decl { $2->options.insert($2->options.end(), $1.begin(), $1.end()); std::swap($$, $2); };
      
func_decl : idents ":" type_sig ":=" block { $$ = std::make_shared<NFunctionDeclaration>($1, $3, $5); }
          | idents ":=" block              { $$ = std::make_shared<NFunctionDeclaration>($1, $3);     }
          | idents ":" type_sig ":=" stmt  { auto b = std::make_shared<NBlock>(); b->statements.push_back($<stmt>5);
                                             $$ = std::make_shared<NFunctionDeclaration>($1, $3, b); }
          | idents ":=" stmt               { auto b = std::make_shared<NBlock>(); b->statements.push_back($<stmt>3);
                                             $$ = std::make_shared<NFunctionDeclaration>($1, b);
                                           }
          ;
               
type_sig : ident               { $$ = IdentList(); $$.push_back($1); }
         | type_sig "->" ident { $1.push_back($3); std::swap($$, $1); }
         ;
      
ident : IDENT { $$ = std::make_shared<NIdentifier>($1); };

idents : ident        { $$ = IdentList(); $$.push_back($<nident>1); }
       | idents ident { $1.push_back($<nident>2); std::swap($$, $1); }
       ;
      
numeric : INTEGER { $$ = std::make_shared<NInteger>($<std::string>1); }
        | DECIMAL { $$ = std::make_shared<NDecimal>($<std::string>1);  }
        ;

expr : func_call       { std::swap($$, $1); }
     | ident "=" expr  { $$ = std::make_shared<NAssignment>($<nident>1, $3); }
     | numeric         { std::swap($$, $1); }
     | expr "*" expr   { $$ = std::make_shared<NBinaryOperator>($1, "*", $3); }
     | expr "/" expr   { $$ = std::make_shared<NBinaryOperator>($1, "/", $3); }
     | expr "+" expr   { $$ = std::make_shared<NBinaryOperator>($1, "+", $3); }
     | expr "-" expr   { $$ = std::make_shared<NBinaryOperator>($1, "-", $3); }
     | "(" expr ")"    { std::swap($$, $2); }
     ;

func_call : ident        { $$ = std::make_shared<NFunctionCall>($1);     }
          | ident exprs  { $$ = std::make_shared<NFunctionCall>($1, $2); }
          ;
          
exprs : expr        { $$ = ExpressionList(); $$.push_back($1); }
      | exprs expr  { $1.push_back($2); std::swap($$, $1);     }
      ;

%%


void
yy::hawk_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
