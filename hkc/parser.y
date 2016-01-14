%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"
%defines
%define parser_class_name { hawk_parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{

# include <string>
# include <vector>
class hawk_driver;

class CodeGenContext;
class NStatement;
class NExpression;
class NIdentifier;
class NBlock;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NIdentifier*> VariableList;
typedef std::vector<NIdentifier*> TypeList;

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
}
%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
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
%token <long int>    INTEGER
%token <long double> DOUBLE
%token <std::string> STRING
%printer { yyoutput << $$; } <*>;


%type <NBlock*> module stmts block
%type <NStatement*> stmt func_decl
%type <NIdentifier*> ident
%type <NExpression*> numeric expr
%type <VariableList*> func_decl_args type_sig
%type <ExpressionList*> call_args

%%


%start module;
module : %empty { driver.result = NULL; }
       | stmts  { driver.result = $1; }
       ;

stmts : stmt       { $$ = new NBlock(); $$->statements.push_back($<NStatement*>1); }
      | stmts stmt { $1->statements.push_back($<NStatement*>1); }
      ;

stmt : func_decl
     | expr ";"        { $$ = new NExpressionStatement(*$1); }
     | RETURN expr ";" { $$ = new NReturnStatement(*$<NExpression*>2);    }
     ;
     
block : LCURLY stmts RCURLY { $$ = $2; }
      | LCURLY RCURLY       { $$ = new NBlock(); }
      ;
      
func_decl : ident func_decl_args ":=" block
            { $$ = new NFunctionDeclaration(*$1, *$2, *$4); }
          | ident func_decl_args ":" type_sig ":=" block
            { $$ = new NFunctionDeclaration(*$1, *$2, *$6, *$4); }
          ;

func_decl_args : %empty      { $$ = new VariableList(); }
               | ident       { $$ = new VariableList(); $$->push_back($<NIdentifier*>1); }
               | func_decl_args ident
                 { $1->push_back($<NIdentifier*>2); }
               ;
               
type_sig : ident { $$ = new TypeList(); $$->push_back($1); }
         | type_sig "->" ident { $1->push_back($3); }
         ;
      
ident : IDENT { $$ = new NIdentifier(*$<NIdentifier*>1); }
      ;
      
numeric : INTEGER { $$ = new NInteger($1); }
        | DOUBLE  { $$ = new NDouble($1);  }
        ;

expr : ident "=" expr  { $$ = new NAssignment(*$<NIdentifier*>1, *$3); }
     | ident           { $$ = new NFunctionCall(*$1); }
     | ident call_args { $$ = new NFunctionCall(*$1, *$2); }
     | numeric
     | expr "*" expr { $$ = new NBinaryOperator(*$1, "*", *$3); }
     | expr "/" expr { $$ = new NBinaryOperator(*$1, "/", *$3); }
     | expr "+" expr { $$ = new NBinaryOperator(*$1, "+", *$3); }
     | expr "-" expr { $$ = new NBinaryOperator(*$1, "-", *$3); }
     | "(" expr ")" { $$ = $2; }
     ;

call_args : expr   { $$ = new ExpressionList; $$->push_back($1); }
          | call_args expr { $1->push_back($2); }
          ;
     
%%


void
yy::hawk_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
