%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.4"

%defines
%define parser_class_name { hawk_parser }
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires
{

#include <memory>
#include <string>
#include <vector>

class hawk_driver;

namespace ast
{
    class Source;
    class Module;
    class Expression;
    class IdentifierRef;
    class Integer;
    class Decimal;
    class String;
    class FunctionCall;
    class BinaryOperator;
    class Assignment;
    class Return;
    class NameBindings;
    class Variable;
    class LocalVariable;
    class GlobalVariable;
    class Function;
    class LocalFunction;
    class GlobalFunction;
    
    typedef std::string Identifier;
    
    typedef std::shared_ptr<Source> pSource;
    typedef std::shared_ptr<Module> pModule;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Function> pFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<Identifier> IdentifierVec;
    typedef std::vector<pExpression> pExpressionVec;
    typedef std::vector<pVariable> pVariableVec;
}

}

// The parsing context.
%param { hawk_driver& driver }

%locations
%initial-action
{
  // Initialize the initial location.
  
  @$.begin.filename = @$.end.filename = &driver.filename;
};

%define parse.trace
%define parse.error verbose

%code
{
#include "hkc/driver.h"
#include "hkc/ast.h"

#include <string>
#include <cstdlib>

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

%type <ast::pModule> module
%type <std::string> ident
%type <ast::IdentifierVec> idents type_sig attribs attribs_stmt
%type <ast::pIdentifierRef> ident_ref
%type <ast::pExpression> primitive expr func_call top_stmt stmt
%type <ast::pExpressionVec> exprs top_stmts stmts block
%type <ast::pNameBindings> name_bindings
%type <ast::pVariable> var_decl var local_var global_var
%type <ast::pFunction> func_decl func global_func local_func


%%


%start module;
module : %empty        { driver.result = std::make_shared<ast::Module>(driver.filename, ast::pExpressionVec()); }
       | top_stmts END { driver.result = std::make_shared<ast::Module>(driver.filename, $1); }
       ;
       
top_stmts : top_stmt           { $$ = ast::pExpressionVec(); $$.push_back($1); }
          | top_stmts top_stmt { $1.push_back($2); $$ = $1; }
          ;

top_stmt : global_func     { ast::pExpression t = $1; $$ = t; }
         | global_var ";"  { ast::pExpression t = $1; $$ = t; }
         ;


attribs : attribs_stmt         { $$ = $1; }
        | attribs attribs_stmt { $1.insert($1.end(), $2.begin(), $2.end()); $$ = $1; }
        ;

attribs_stmt : "@" idents ";" { $$ = $2; };

             
stmts : stmt       { $$ = ast::pExpressionVec(); $$.push_back($1); }
      | stmts stmt { $1.push_back($2); $$ = $1; }
      ;

stmt : expr ";"     { $$ = $1; }
     | "^" expr ";" { $$ = std::make_shared<ast::Return>($2); }
     | local_var ";" { ast::pExpression t = $1; $$ = t; }
     | local_func    { ast::pExpression t = $1; $$ = t; }
     ;
     
local_var  : var { $$ = ast::promote_local($1); };
global_var : var { $$ = ast::promote_global($1); };

var : var_decl         { $$ = $1; }
    | attribs var_decl { $2->attribs = $1; $$ = $2; }
    ;
      
var_decl : "let" name_bindings     { $$ = mk_var($2, nullptr); }
         | name_bindings "<-" expr { $$ = mk_var($1, $3); }
         ;
          
local_func  : func { $$ = ast::promote_local($1); };
global_func : func { $$ = ast::promote_global($1); };

func : func_decl         { $$ = $1; }
     | attribs func_decl { $2->attribs = $1; $$ = $2; }
     ;
      
func_decl : name_bindings ":=" block { $$ = ast::mk_func($1, $3); }
          | name_bindings ":=" stmt  { ast::pExpressionVec exprs;
                                       exprs.push_back($3);
                                       $$ = ast::mk_func($1, exprs);
                                     }
          ;
          
block : LCURLY RCURLY       { $$ = ast::pExpressionVec(); }
      | LCURLY stmts RCURLY { $$ = $2; }
      ;

name_bindings : idents type_sig { $$ = std::make_shared<ast::NameBindings>($1, $2); };
            
type_sig : %empty              { $$ = ast::IdentifierVec(); }
         | ":" ident           { $$ = ast::IdentifierVec(); $$.push_back($2); }
         | type_sig "->" ident { $1.push_back($3); $$ = std::move($1); }
         ;

      
ident : IDENT { $$ = ast::Identifier($1); };

ident_ref : ident { $$ = std::make_shared<ast::IdentifierRef>($1); };  

idents : ident        { $$ = ast::IdentifierVec(); $$.push_back($1); }
       | idents ident { $1.push_back($2); $$ = $1; }
       ;
      

      
primitive : INTEGER { $$ = std::make_shared<ast::Integer>(atoll($1.c_str())); }
          | DECIMAL { $$ = std::make_shared<ast::Decimal>(strtold($1.c_str(), nullptr)); }
          | STRING  { $$ = std::make_shared<ast::String>($<std::string>1); }
          ;
        


expr : func_call           { $$ = $<ast::pExpression>1; }
     | ident_ref           { ast::pExpression t = $1; $$ = t; }
     | ident_ref "=" expr  { $$ = std::make_shared<ast::Assignment>($1, $3); }
     | primitive           { $$ = $1; }
     | expr "*" expr       { $$ = std::make_shared<ast::BinaryOperator>($1, "*", $3); }
     | expr "/" expr       { $$ = std::make_shared<ast::BinaryOperator>($1, "/", $3); }
     | expr "+" expr       { $$ = std::make_shared<ast::BinaryOperator>($1, "+", $3); }
     | expr "-" expr       { $$ = std::make_shared<ast::BinaryOperator>($1, "-", $3); }
     | "(" expr ")"        { $$ = $2; }
     ;
         

func_call : ident_ref exprs { $$ = std::make_shared<ast::FunctionCall>($1, $2); }
          ;        
          
exprs : expr        { $$ = ast::pExpressionVec(); $$.push_back($1); }
      | exprs expr  { $1.push_back($2); $$ = $1;    }
      ;

%%


void yy::hawk_parser::error (const location_type& l, const std::string& m)
{
  driver.error (l, m);
}
