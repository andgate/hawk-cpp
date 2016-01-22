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
class NFunctionCall;
class NameBindings;
class NVariableDeclaration;
class NFunctionDeclaration;

typedef std::shared_ptr<NBlock> nblock;
typedef std::shared_ptr<NStatement> nstmt;
typedef std::shared_ptr<NIdentifier> nident;
typedef std::shared_ptr<NExpression> nexpr;
typedef std::shared_ptr<NFunctionDeclaration> nfuncdecl;
typedef std::shared_ptr<NVariableDeclaration> nvardecl;
typedef std::shared_ptr<NFunctionCall> nfunc_call;
typedef std::shared_ptr<NameBindings> nbindings;

typedef std::vector<std::shared_ptr<NStatement>> StatementList;
typedef std::vector<std::shared_ptr<NExpression>> ExpressionList;
typedef std::vector<std::shared_ptr<NIdentifier>> IdentList;

class NameBindings;

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
#include "hkc/parser/driver.h"
#include "hkc/parser/node.h"

#include <string>
}

%define api.token.prefix {TOK_}
%token
  END  0    "end of file"
  LPAREN    "("
  RPAREN    ")"
  CARET     "^"
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
  LET       "let"
;

%token <std::string> IDENT STRING INTEGER DECIMAL

%printer { yyoutput << (&$$); } <*>;


%type <nblock> top_stmts block stmts
%type <nstmt> top_stmt stmt function func_decl variable var_decl
%type <nident> ident
%type <IdentList> idents type_sig options_stmt options_list
%type <nexpr> primitive expr func_call
%type <ExpressionList> exprs
%type <nbindings> name_bindings

%%


%start module;
module : %empty        { driver.result = std::make_shared<NBlock>(); }
       | top_stmts END { driver.result = $1; }
       ;
       
block : LCURLY RCURLY       { $$ = std::make_shared<NBlock>(); }
      | LCURLY stmts RCURLY { $$ = std::move($2); }
      ;
       
top_stmts : top_stmt           { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<nstmt>1); }
          | top_stmts top_stmt { $1->statements.push_back($<nstmt>2); std::swap($$, $1); }
          ;

top_stmt : function { std::swap($$, $1); }
         | variable ";" { std::swap($$, $1); }
         ;


options_stmt : "@" idents ";" { $$ = std::move($2); };

options_list : options_stmt   { $$ = std::move($1); }
             | options_list options_stmt { $1.insert($1.end(), $2.begin(), $2.end()); $$ = std::move($1); }
             ;

             
stmts : stmt       { $$ = std::make_shared<NBlock>(); $$->statements.push_back($<nstmt>1); }
      | stmts stmt { $1->statements.push_back($<nstmt>2); std::swap($$, $1); }
      ;

stmt : expr ";"        { $$ = std::make_shared<NExpressionStatement>($1); }
     | "^" expr ";" { $$ = std::make_shared<NReturnStatement>($<nexpr>2); }
     | variable ";"    { $$ = std::move($1); }
     | function      { $$ = std::move($1); }
     ;

variable : var_decl { $$ = std::move($1); }
         | options_list var_decl { $2->options = std::move($1); $$ = std::move($2); };
      
var_decl : "let" name_bindings     { $$ = std::make_shared<NVariableDeclaration>($2, nullptr); }
         | name_bindings "<-" expr { $$ = std::make_shared<NVariableDeclaration>($1, $3); }
         ;
           
function : func_decl { $$ = std::move($1); }
         | options_list func_decl { $2->options = std::move($1); $$ = std::move($2); };
      
func_decl : name_bindings ":=" block { $$ = std::make_shared<NFunctionDeclaration>($1, $3); }
          | name_bindings ":=" stmt  { auto b = std::make_shared<NBlock>();
                                       b->statements.push_back($<nstmt>3);
                                       $$ = std::make_shared<NFunctionDeclaration>($1, b);
                                     }
          ;

name_bindings : idents type_sig { $$ = std::make_shared<NameBindings>($1, $2); };
            
type_sig : %empty              { $$ = IdentList(); }
         | ":" ident           { $$ = IdentList(); $$.push_back(std::move($2)); }
         | type_sig "->" ident { $1.push_back($3); $$ = std::move($1); }
         ;
         
      
ident : IDENT { $$ = std::make_shared<NIdentifier>($1); };

idents : ident        { $$ = IdentList(); $$.push_back($<nident>1); }
       | idents ident { $1.push_back($<nident>2); $$ = std::move($1); }
       ;
      

      
primitive : INTEGER { $$ = std::make_shared<NInteger>($<std::string>1); }
          | DECIMAL { $$ = std::make_shared<NDecimal>($<std::string>1); }
          | STRING  { $$ = std::make_shared<NString>($<std::string>1); }
          ;
        


expr : func_call       { std::swap($$, $<nexpr>1); }
     | ident "=" expr  { $$ = std::make_shared<NAssignment>($<nident>1, $3); }
     | primitive       { std::swap($$, $1); }
     | expr "*" expr   { $$ = std::make_shared<NBinaryOperator>($1, "*", $3); }
     | expr "/" expr   { $$ = std::make_shared<NBinaryOperator>($1, "/", $3); }
     | expr "+" expr   { $$ = std::make_shared<NBinaryOperator>($1, "+", $3); }
     | expr "-" expr   { $$ = std::make_shared<NBinaryOperator>($1, "-", $3); }
     | "(" expr ")"    { std::swap($$, $2); }
     ;
         

func_call : ident       { $$ = std::make_shared<NFunctionCall>($1); }
          | ident exprs { $$ = std::make_shared<NFunctionCall>($1, $2); }
          ;
          
exprs : expr        { $$ = ExpressionList(); $$.push_back($1); }
      | exprs expr  { $1.push_back($2); $$ = std::move($1);    }
      ;

%%


void
yy::hawk_parser::error (const location_type& l,
                          const std::string& m)
{
  driver.error (l, m);
}
