%{ /* -*- C++ -*- */
    #include <cerrno>
    #include <climits>
    #include <cfloat>
    #include <cstdlib>
    #include <assert.h>
    #include <string>
    #include <vector>
    #include <stack>
    
    #include "hkc/driver.h"
    #include "parser.hpp"

    // The location of the current token.
    static yy::location loc;
    
    std::stack<unsigned int> indent_stack;
    unsigned int indent_level = 0;
    unsigned int nesting = 0;
%}

%x do_bol
%x indent
%x ml_com
%s normal

%option noyywrap yylineno stack batch debug


id    [a-zA-Z][a-zA-Z_0-9]*
modid [a-zA-Z][a-zA-Z_0-9.]*
digit [0-9]+
op    [\!\#\$\%\&\*\+\.\/\<\=\>\?\@\\\^\|\-\~\:]+

integer {digit}+
decimal {digit}*[\.]{digit}*

string \"(\\.|[^\\"])*\"

comment     "//".*
ml_comment  [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]

%{
    #define YY_USER_ACTION loc.columns(yyleng);
%}


%%


%{
    // Code run each time yylex is called.
    loc.step();
%}


<*>^[ ]*\n              loc.lines(1); indent_level = 0;
<*>^[ ]*{comment}"\n"   loc.lines(1); indent_level = 0;
<*>{comment}            {}
"/*"                    yy_push_state(ml_com);

<do_bol>{
    ^[ ]*   indent_level = yyleng - 1;
    "\n"    loc.lines(1); indent_level = 0;
    "/*"    yy_push_state(ml_com);
    .       {
                unput(*yytext);
                
                if(indent_stack.size() > 1)
                {
                    while(indent_level < indent_stack.top())
                    {
                        indent_stack.pop();
                        unput('}');
                    }
                }
                
                if(indent_level == indent_stack.top())
                    unput(';');
                
                yy_pop_state();
            }
}

<indent>{
    ^[ ]*   indent_level = yyleng - 1;
    "\n"    loc.lines(1); indent_level = 0;
    "/*"    yy_push_state(ml_com);
    .       {
                yyless(0);
                indent_stack.push(indent_level);
                
                yy_pop_state();
                return yy::hawk_parser::make_LCURLY(loc);
            }

}
            
<normal>{

    \n          loc.lines(1); indent_level = 0; yy_push_state(do_bol); 
    \t          driver.error(loc, "Tabs are not allowed!"); exit(0);
    
    ".:"         return yy::hawk_parser::make_PCOLON(loc);
    ":."[ ]*"\n" yy_push_state(indent); loc.lines(1); return yy::hawk_parser::make_CCOLON(loc);
    ":."         return yy::hawk_parser::make_COLONP(loc);
    
    ":="[ ]*"\n" yy_push_state(indent); loc.lines(1); return yy::hawk_parser::make_FUNCDEC(loc);        
    ":="         return yy::hawk_parser::make_FUNCDEC(loc);
    
    ":-"[ ]*"\n" yy_push_state(indent); loc.lines(1); return yy::hawk_parser::make_TYPEDEC(loc);  
    ":-"         return yy::hawk_parser::make_TYPEDEC(loc);
    
    ":~"[ ]*"\n" yy_push_state(indent); loc.lines(1); return yy::hawk_parser::make_TYPECLASS(loc);
    ":~"         return yy::hawk_parser::make_TYPECLASS(loc);
    
    ":+"[ ]*"\n" yy_push_state(indent); loc.lines(1); return yy::hawk_parser::make_IMPLEMENT(loc);
    ":+"         return yy::hawk_parser::make_IMPLEMENT(loc);
    
    "<-"        return yy::hawk_parser::make_LARROW(loc);
    "<="        return yy::hawk_parser::make_LLARROW(loc);
    "->"        return yy::hawk_parser::make_RARROW(loc);
    "=>"        return yy::hawk_parser::make_RRARROW(loc);
    "<:"        return yy::hawk_parser::make_SUBTYPE(loc);
    
    "`"         return yy::hawk_parser::make_GRAVE(loc);
    "~"         return yy::hawk_parser::make_TILDE(loc);
    "!"         return yy::hawk_parser::make_EXCLAIM(loc);
    "?"         return yy::hawk_parser::make_QUESTION(loc);
    "@"         return yy::hawk_parser::make_AT(loc);
    "#"         return yy::hawk_parser::make_POUND(loc);
    "$"         return yy::hawk_parser::make_DOLLAR(loc);
    "^"         return yy::hawk_parser::make_CARET(loc);
    "&"         return yy::hawk_parser::make_AMPERSAND(loc);
    
    "("         nesting++; return yy::hawk_parser::make_LPAREN(loc);
    ")"         nesting--; return yy::hawk_parser::make_RPAREN(loc);
    "{"         return yy::hawk_parser::make_LCURLY(loc);
    "}"         return yy::hawk_parser::make_RCURLY(loc);
    "["         return yy::hawk_parser::make_LBRACKET(loc);
    "]"         return yy::hawk_parser::make_RBRACKET(loc);
    "|"         return yy::hawk_parser::make_BAR(loc);
    
    ":"         return yy::hawk_parser::make_COLON(loc);
    ";"         return yy::hawk_parser::make_SEMICOLON(loc);
    "."         return yy::hawk_parser::make_PERIOD(loc);
    ","         return yy::hawk_parser::make_COMMA(loc);
    "<"         return yy::hawk_parser::make_LESSER(loc);
    ">"         return yy::hawk_parser::make_GREATER(loc);
    
    "-"         return yy::hawk_parser::make_MINUS(loc);
    "+"         return yy::hawk_parser::make_PLUS(loc);
    "*"         return yy::hawk_parser::make_STAR(loc);
    "/"         return yy::hawk_parser::make_SLASH(loc);
    "="         return yy::hawk_parser::make_EQUALS(loc);
    
    {id}        return yy::hawk_parser::make_ID(yytext, loc);
    {op}        return yy::hawk_parser::make_OP(yytext, loc);
    {string}    return yy::hawk_parser::make_STRING(yytext, loc);
    {integer}   return yy::hawk_parser::make_INTEGER(yytext, loc);
    {decimal}   return yy::hawk_parser::make_DECIMAL(yytext, loc);
}

<ml_com>{
    [^*\n]*         /* eat anything not a '*' */
    "*"+[^*/\n]*    /* eat anything not follow by '/'s */ 
    \n              loc.lines(1);
    "*"+"/"         yy_pop_state();
}

[.\n]           { driver.error (loc, "invalid character"); }
<<EOF>>         {
                    if(indent_stack.size() == 1)
                        return yy::hawk_parser::make_END(loc);
                        
                    unput(NULL);
                    
                    while(indent_stack.size() > 1)
                    {
                        indent_stack.pop();
                        unput('}');
                    }
                    
                    std::cout << "Indent Level: " << indent_level << std::endl;
                    
                    unput(';');
                }

%%


void hawk_driver::scan_begin ()
{
    yy_push_state(normal);
    indent_stack.push(0);
    
    yy_flex_debug = trace_scanning;
    if (filename.empty () || filename == "-") {
        yyin = stdin;
    } else if(!(yyin = fopen(filename.c_str(), "r")))
    {
        error("cannot open " + filename + ": " + strerror(errno));
        exit(EXIT_FAILURE);
    }
}



void hawk_driver::scan_end ()
{
    fclose(yyin);
}