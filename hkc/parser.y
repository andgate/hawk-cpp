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
  END  0  "end of file"
  SEMICOLON ";"
  COLON   ":"
  EXCLAIM "!"
  FUNDEC  ":="
  TYPDEC  ":-"
  LCURLY  "{"
  RCURLY  "}"
  LARROW  "<-"
  RARROW  "->"
  BAR     "|"
  STAR    "*"
  SLASH   "/"
  PLUS    "+" 
  MINUS   "-"
  LPAREN  "("
  RPAREN  ")"
  RETURN  "return"
;
%token <std::string> IDENT
%token <long long>    INTEGER
%token <long double> DOUBLE
%token <std::string> STRING
%printer { yyoutput << (&$$); } <*>;


%type <block> module top_stmts block stmts
%type <stmt> top_stmt stmt func_decl
%type <ident> ident
%type <expr> numeric expr
%type <ExpressionList> call_args
%type <IdentList> func_header type_sig

%%


%start module;
module : %empty { driver.result = NULL; }
       | top_stmts  { driver.result = $1; }
       ;
       
top_stmts : top_stmt           { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<stmt>1); }
          | top_stmts top_stmt { $1->statements.push_back($<stmt>1); }
          ;

top_stmt : func_decl;
          
stmts : stmt       { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>1); }
      ;

stmt : expr ";"        { $$ = std::make_shared<NExpressionStatement>($1);}
     | RETURN expr ";" { $$ = std::make_shared<NReturnStatement>($<expr>2); }
     ;
     
block : LCURLY RCURLY       { $$ = std::make_shared<NBlock>(); }
      | LCURLY stmts RCURLY { $$ = $2; }
      ;      
      
func_decl : func_header ":" type_sig ":=" block { $$ = std::make_shared<NFunctionDeclaration>($1, $5, $3); }
          | func_header ":=" block              { $$ = std::make_shared<NFunctionDeclaration>($1, $3); }
          ;

func_header : ident             { $$ = IdentList(); $$.push_back($<nident>1); }
            | func_header ident { $1.push_back($<nident>2); }
            ;
               
type_sig : ident               { $$ = IdentList(); $$.push_back($1); }
         | type_sig "->" ident { $1.push_back($3); }
         ;
      
ident : IDENT { $$ = std::make_shared<NIdentifier>($1); };
      
numeric : INTEGER { $$ = std::make_shared<NInteger>($1); }
        | DOUBLE  { $$ = std::make_shared<NDouble>($1);  }
        ;

expr : ident "=" expr  { $$ = std::make_shared<NAssignment>($<nident>1, $3); }
     | ident           { $$ = std::make_shared<NFunctionCall>($1); }
     | ident call_args { $$ = std::make_shared<NFunctionCall>($1, $2); }
     | numeric        { $$ = $1; }
     | expr "*" expr { $$ = std::make_shared<NBinaryOperator>($1, "*", $3); }
     | expr "/" expr { $$ = std::make_shared<NBinaryOperator>($1, "/", $3); }
     | expr "+" expr { $$ = std::make_shared<NBinaryOperator>($1, "+", $3); }
     | expr "-" expr { $$ = std::make_shared<NBinaryOperator>($1, "-", $3); }
     | "(" expr ")" { $$ = $2; }
     ;

call_args : expr   { $$ = ExpressionList(); $$.push_back($1); }
          | call_args expr { $1.push_back($2); }
          ;
     
%%


void
yy::hawk_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
