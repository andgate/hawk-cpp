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
    class ExpressionGroup;
    class Source;
    class Module;
    class Submodule;
    class ModuleIdentifier;
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
    class Record;
    class TaggedUnion;
    class TaggedVariant;
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
    typedef std::shared_ptr<Import> pImport;
    typedef std::shared_ptr<QImport> pQImport;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<ExpressionGroup> pExpressionGroup;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    
    typedef std::shared_ptr<Record> pRecord;
    typedef std::shared_ptr<TaggedUnion> pTaggedUnion;
    typedef std::shared_ptr<TaggedVariant> pTaggedVariant;
    
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Variable> pGlobalVariable;
    typedef std::shared_ptr<Variable> pLocalVariable;
    
    typedef std::shared_ptr<Function> pFunction;
    typedef std::shared_ptr<Function> pGlobalFunction;
    typedef std::shared_ptr<Function> pLocalFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<pModuleIdentifier> pModuleIdentifierVec;
    typedef std::vector<Identifier> IdentifierVec;
    typedef std::vector<pTaggedUnion> pTaggedUnionVec;
    typedef std::vector<pTaggedVariant> pTaggedVariantVec;
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
  
  PCOLON    ".:"
  COLONP    ":."
  CCOLON    "::"
  
  FUNCDEC   ":="
  TYPEDEC   ":-"
  TYPECLASS ":~"
  IMPLEMENT ":+"
  
  LARROW    "<-"
  LLARROW   "<="
  RARROW    "->"
  RRARROW   "=>"
  SUBTYPE   "<:"
  
  GRAVE     "`"
  TILDE     "~"
  EXCLAIM   "!"
  QUESTION  "?"
  AT        "@"
  POUND     "#"
  DOLLAR    "$"
  PERCENT   "%"
  CARET     "^"
  AMPERSAND "&"
  
  LPAREN    "("
  RPAREN    ")"
  LCURLY    "{"
  RCURLY    "}"
  LBRACKET  "["
  RBRACKET  "]"
  BAR       "|"
  
  COLON     ":"
  SEMICOLON ";"
  PERIOD    "."
  COMMA     ","
  LESSER    "<"
  GREATER   ">"
  
  STAR      "*"
  SLASH     "/"
  PLUS      "+"
  MINUS     "-"
  EQUALS    "="
  
  
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
%type <ast::pImport> import
%type <std::string> ident mod_id_base module_name
%type <ast::IdentifierVec> idents type_sig attribs attribs_stmt
%type <ast::pIdentifierRef> ident_ref
%type <ast::pExpression> primitive expr func_call top_stmt stmt record_member typedef
%type <ast::pExpressionGroup> exprs top_stmts stmts block record_members global_vars global_var_defs local_vars local_var_defs
%type <ast::pNameBindings> name_bindings
%type <ast::pRecord> record
%type <ast::pTaggedUnion> tagged_union
%type <ast::pTaggedVariantVec> tagged_variants
%type <ast::pTaggedVariant> tagged_variant
%type <ast::pVariable> var_def
%type <ast::pFunction> func_def func global_func local_func


%%


%start module;
module : %empty                    { }
       | top_stmts END             { driver.result->group = $1; }
       | module_name top_stmts END { driver.result->id = $1; driver.result->group = $2; }
       ;
       
module_name : ".:" mod_id_base { $$ = $2; };
       
       
top_stmts : top_stmt           { auto g = std::make_shared<ast::ExpressionGroup>(); g->exprs.push_back($1); $$ = g; }
          | top_stmts top_stmt { $1->exprs.push_back($2); $$ = $1; }
          ;

top_stmt : import      { ast::pExpression t = $1; $$ = t; }
         | global_func { ast::pExpression t = $1; $$ = t; }
         | global_vars { ast::pExpression t = $1; $$ = t; }
         | submod      { ast::pExpression t = $1; $$ = t; }
         | typedef     { $$ = $1; }
         ;
         
submod : mod_id_base ":." "{" top_stmts "}" { $$ = std::make_shared<ast::Submodule>($1, $4); }
       ;
       
import : "->" mod_id { $$ = std::make_shared<ast::Import>($2); }
       | "=>" mod_id { $$ = std::make_shared<ast::QImport>($2); }
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

attribs_stmt : "@" "{" idents "}" { $$ = $3; };

             
stmts : stmt       { $$ = std::make_shared<ast::ExpressionGroup>(); $$->exprs.push_back($1); }
      | stmts stmt { $1->exprs.push_back($2); $$ = $1; }
      ;

stmt : "{" expr "}"       { $$ = $2; }
     | "{" "^" expr "}"   { $$ = std::make_shared<ast::Return>($3); }
     | "{" local_vars "}" { ast::pExpression t = $2; $$ = t; }
     | "{" local_func "}" { ast::pExpression t = $2; $$ = t; }
     ;
     
typedef : record       { ast::pExpression t = $1; $$ = t; }
        | tagged_union { ast::pExpression t = $1; $$ = t; }
        ;
        
        
record : ident ":-" "{" record_members "}"  { $$ = std::make_shared<ast::Record>($1, $4); }
       ;

record_members : record_member { auto g = std::make_shared<ast::ExpressionGroup>(); g->exprs.push_back($1); $$ = g; }
               | record_members record_member { $1->exprs.push_back($2); $$ = $1; }
               ;
               
record_member  : global_func { ast::pExpression t = $1; $$ = t; }
               | global_vars { ast::pExpression t = $1; $$ = t; }
               ;

               
tagged_union : ident ":-" "{" tagged_variants "}" { $$ = std::make_shared<ast::TaggedUnion>($1, $4); }
             ;
               
tagged_variants : tagged_variant { $$ = ast::pTaggedVariantVec(); $$.push_back($1); }
                | tagged_variants "|" tagged_variant { $1.push_back($3); $$ = $1; }
                ;
      
tagged_variant : ident        { $$ = std::make_shared<ast::TaggedVariant>($1, ast::IdentifierVec()); }
               | ident idents { $$ = std::make_shared<ast::TaggedVariant>($1, $2); }
               ;

               
global_vars : "$" "{" global_var_defs "}" { $$ = $3; }
            ;

global_var_defs : var_def   { $$ = std::make_shared<ast::ExpressionGroup>(); $$->exprs.push_back(promote_global($1)); }
                | global_var_defs var_def { $$ = $1; $$->exprs.push_back(promote_global($2)); }
                ;
    
local_vars : "$" "{" local_var_defs "}" { $$ = $3; }
           ;
                
local_var_defs : var_def    { auto g = std::make_shared<ast::ExpressionGroup>(); g->exprs.push_back(promote_local($1)); $$ = g; }
               | local_var_defs var_def { $1->exprs.push_back(promote_local($2)); $$ = $1; }
               ;
      
var_def : "{" name_bindings "}"          { $$ = mk_var($2, nullptr); }
        | "{" name_bindings "=" expr "}" { $$ = mk_var($2, $4); }
        ;

          
local_func  : func { $$ = ast::promote_local($1); };
global_func : func { $$ = ast::promote_global($1); };

func : func_def         { $$ = $1; }
     | attribs func_def { $2->attribs = $1; $$ = $2; }
     ;
      
func_def : name_bindings ":=" block { $$ = ast::mk_func($1, $3); }
         ;
          
block : "{" "}"       { $$ = std::make_shared<ast::ExpressionGroup>(); }
      | "{" stmts "}" { $$ = $2; }
      ;

name_bindings : idents ":" type_sig { $$ = std::make_shared<ast::NameBindings>($1, $3); }
              | idents  { $$ = std::make_shared<ast::NameBindings>($1, ast::IdentifierVec()); }
              ;
            
type_sig : ident { $$ = ast::IdentifierVec(); $$.push_back($1); }
         | type_sig "->" ident { $1.push_back($3); $$ = std::move($1); }
         ;

      
ident : ID { $$ = ast::Identifier($1); };

ident_ref : ident { $$ = std::make_shared<ast::IdentifierRef>($1); }
          | ident_ref "." ident { $1->str += '.' + $3; $$ = $1; }
          ;

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
          
exprs : expr        { $$ = std::make_shared<ast::ExpressionGroup>(); $$->exprs.push_back($1); }
      | exprs expr  { $1->exprs.push_back($2); $$ = $1; }
      ;

%%


void yy::hawk_parser::error (const location_type& l, const std::string& m)
{
  driver.error (l, m);
}
