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
    class Submodule;
    class ModuleIdentifier;
    class Export;
    class QExport;
    class Import;
    class QImport;
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
    class Typedef;
    class GlobalTypedef;
    class LocalTypedef;
    class Variable;
    class GlobalVariable;
    class LocalVariable;
    class Function;
    class GlobalFunction;
    class LocalFunction;
    
    typedef std::string Identifier;
    
    typedef std::shared_ptr<Source> pSource;
    typedef std::shared_ptr<Module> pModule;
    typedef std::shared_ptr<Submodule> pSubmodule;
    typedef std::shared_ptr<ModuleIdentifier> pModuleIdentifier;
    typedef std::shared_ptr<Export> pExport;
    typedef std::shared_ptr<QExport> pQExport;
    typedef std::shared_ptr<Import> pImport;
    typedef std::shared_ptr<QImport> pQImport;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    
    typedef std::shared_ptr<Typedef> pTypedef;
    typedef std::shared_ptr<GlobalTypedef> pGlobalTypedef;
    typedef std::shared_ptr<LocalTypedef> pLocalTypedef;
    
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Variable> pGlobalVariable;
    typedef std::shared_ptr<Variable> pLocalVariable;
    
    typedef std::shared_ptr<Function> pFunction;
    typedef std::shared_ptr<Function> pGlobalFunction;
    typedef std::shared_ptr<Function> pLocalFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<pModuleIdentifier> pModuleIdentifierVec;
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
  CCOLON    "::"
  COLON     ":"
  EXCLAIM   "!"
  TYPEDEC   ":-"
  FUNCDEC   ":="
  LCURLY    "{"
  RCURLY    "}"
  LARROW    "<-"
  LLARROW   "<="
  RARROW    "->"
  RRARROW   "=>"
  PERIOD    "."
  BAR       "|"
  STAR      "*"
  SLASH     "/"
  PLUS      "+" 
  MINUS     "-"
  LET       "let"
  OPEN_BLOCK
  CLOSE_BLOCK
  OPEN_STMT
  CLOSE_STMT
;

%token <std::string> ID OP STRING INTEGER DECIMAL

%printer { yyoutput << (&$$); } <*>;

%type <ast::pModule> module
%type <ast::pSubmodule> submod
%type <ast::pModuleIdentifier> mod_id
%type <ast::pModuleIdentifierVec> mod_ids mod_id_specifier
%type <ast::pExport> export
%type <ast::pImport> import
%type <std::string> ident mod_id_base
%type <ast::IdentifierVec> idents type_sig attribs attribs_stmt
%type <ast::pIdentifierRef> ident_ref
%type <ast::pExpression> primitive expr func_call top_stmt stmt
%type <ast::pExpressionVec> exprs top_stmts stmts block
%type <ast::pNameBindings> name_bindings
%type <ast::pVariable> var_def var local_var global_var
%type <ast::pFunction> func_def func global_func local_func


%%


%start module;
module : %empty        { driver.result = std::make_shared<ast::Module>(driver.filename, ast::pExpressionVec()); }
       | top_stmts END { driver.result = std::make_shared<ast::Module>(driver.filename, $1); }
       ;
       
       
top_stmts : top_stmt           { $$ = ast::pExpressionVec(); $$.push_back($1); }
          | top_stmts top_stmt { $1.push_back($2); $$ = $1; }
          ;

top_stmt : export          { ast::pExpression t = $1; $$ = t; }
         | import          { ast::pExpression t = $1; $$ = t; }
         | global_func     { ast::pExpression t = $1; $$ = t; }
         | global_var ";"  { ast::pExpression t = $1; $$ = t; }
         | submod          { ast::pExpression t = $1; $$ = t; }
         ;
         
submod : mod_id_base "::" "{" top_stmts "}" { $$ = std::make_shared<ast::Submodule>($1, $4); }
       | mod_id_base "::" top_stmt          { ast::pExpressionVec e;
                                              e.push_back($3);
                                              $$ = std::make_shared<ast::Submodule>($1, e);
                                            }
       ;
         
export : "<-" mod_id ";" { $$ = std::make_shared<ast::Export>($2); }
       | "<=" mod_id ";" { $$ = std::make_shared<ast::QExport>($2); }
       ;
       
import : "->" mod_id ";" { $$ = std::make_shared<ast::Import>($2); }
       | "=>" mod_id ";" { $$ = std::make_shared<ast::QImport>($2); }
       ;
       
mod_ids : mod_id         { $$ = ast::pModuleIdentifierVec(); $$.push_back($1); }
        | mod_ids mod_id { $1.push_back($2); $$ = $1; }
        ;
       
mod_id : mod_id_base mod_id_specifier { $$ = std::make_shared<ast::ModuleIdentifier>($1, $2); }
       ;
      
mod_id_base : ident                 { $$ = $1; }
            | mod_id_base "." ident { $$ = $1 + "." + $3; }
            ;
      
mod_id_specifier : %empty          { $$ = ast::pModuleIdentifierVec(); }
                 | "." "(" mod_ids ")" { $$ = $3; };

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
     
     
type_def : idents ":-"  {}
         ;
        

type_member : var
            | func
            ;
     
local_var  : var { $$ = ast::promote_local($1); };
global_var : var { $$ = ast::promote_global($1); };

var : var_def         { $$ = $1; }
    | attribs var_def { $2->attribs = $1; $$ = $2; }
    ;
      
var_def : "let" name_bindings     { $$ = mk_var($2, nullptr); }
         | name_bindings "<-" expr { $$ = mk_var($1, $3); }
         ;
          
local_func  : func { $$ = ast::promote_local($1); };
global_func : func { $$ = ast::promote_global($1); };

func : func_def         { $$ = $1; }
     | attribs func_def { $2->attribs = $1; $$ = $2; }
     ;
      
func_def : name_bindings ":=" block { $$ = ast::mk_func($1, $3); }
          | name_bindings ":=" stmt  { ast::pExpressionVec exprs;
                                       exprs.push_back($3);
                                       $$ = ast::mk_func($1, exprs);
                                     }
          ;
          
block : LCURLY RCURLY       { $$ = ast::pExpressionVec(); }
      | LCURLY stmts RCURLY { $$ = $2; }
      ;

name_bindings : idents ":" type_sig { $$ = std::make_shared<ast::NameBindings>($1, $3); }
              | idents  { $$ = std::make_shared<ast::NameBindings>($1, ast::IdentifierVec()); }
              ;
            
type_sig : ident { $$ = ast::IdentifierVec(); $$.push_back($1); }
         | type_sig "->" ident { $1.push_back($3); $$ = std::move($1); }
         ;

      
ident : ID { $$ = ast::Identifier($1); };

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
