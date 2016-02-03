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
    
    enum Scheme {
        ROOT,
        FLOAT,
        ALIGN
    };
    
    struct Layout
    {
        unsigned int column;
        Scheme scheme;
        
        Layout(unsigned int column, Scheme scheme)
        : column(column), scheme(scheme) {}
    };
    
    std::stack<Layout> lstack;
%}

%x do_bol
%x layout_align
%x layout_float
%x inline_comment
%x block_comment
%s normal

%option noyywrap yylineno stack batch debug


id    [a-zA-Z][a-zA-Z_0-9]*
modid [a-zA-Z][a-zA-Z_0-9.]*
digit [0-9]+
op    [\!\#\$\%\&\*\+\.\/\<\=\>\?\@\\\^\|\-\~\:]+

integer {digit}+
decimal {digit}*[\.]{digit}*

string \"(\\.|[^\\"])*\"

inline_comment     "//".*

%{
    #define YY_USER_ACTION loc.columns(yyleng);
%}


%%


%{
    // Code run each time yylex is called.
    loc.step();
%}

<do_bol>{
    ^[ ]*   loc.step();
    "\n"    loc.lines(1); loc.step();
    "/*"    yy_push_state(block_comment);
    "//"    yy_push_state(inline_comment);
    .       {
                unput(*yytext);
                
                // Try to remove invalid scopes
                if(lstack.size() > 1)
                {
                    std::cout << "Checking for layouts that close on column " << loc.begin.column << std::endl;
                    while(loc.begin.column < lstack.top().column)
                    {
                        Layout l = lstack.top();
                        std::cout << "Closing ";
                        if(l.scheme == FLOAT)
                            std::cout << "floating layout";
                        if(l.scheme == ALIGN)
                            std::cout << "aligned layout";
                            
                        std::cout << " on column " << l.column << std::endl;
                        
                        lstack.pop();
                        unput('}');
                    }
                    
                }
                
                yy_pop_state();
                
                if(lstack.top().scheme == ALIGN
                   && lstack.top().column == loc.begin.column)
                {
                    yy_push_state(layout_float);
                }
            }
}

<layout_align>{
    ^[ ]*   loc.step();
    "\n"    loc.lines(1); loc.step();
    "/*"    yy_push_state(block_comment);
    "//"    yy_push_state(inline_comment);
    .       {
                unput(*yytext);
                unput('{');
                lstack.push(Layout(loc.begin.column, Scheme::ALIGN));
                
                std::cout << "Creating aligned layout on column " << loc.begin.column-1 << std::endl;
                
                yy_pop_state();
            }

}

<layout_float>{
    ^[ ]*   loc.step();
    "\n"    loc.lines(1); loc.step();
    "/*"    yy_push_state(block_comment);
    "//"    yy_push_state(inline_comment);
    .       {
                unput(*yytext);
                unput('{');
                lstack.push(Layout(lstack.top().column+1, Scheme::FLOAT));
                
                std::cout << "Creating floating layout on column " << lstack.top().column << std::endl;
                
                yy_pop_state();
            }

}
            
<normal>{

    [ ]         loc.step();
    \n          loc.lines(1); loc.step(); yy_push_state(do_bol); 
    \t          driver.error(loc, "Tabs are not allowed!"); exit(0);
    "/*"        yy_push_state(block_comment);
    "//"        yy_push_state(inline_comment);
    
    ".:"        return yy::hawk_parser::make_PCOLON(loc);
    ":."        yy_push_state(layout_float); return yy::hawk_parser::make_CCOLON(loc);
    ":="        yy_push_state(layout_align); return yy::hawk_parser::make_FUNCDEC(loc);
    ":-"        yy_push_state(layout_float); return yy::hawk_parser::make_TYPEDEC(loc);
    ":~"        yy_push_state(layout_float); return yy::hawk_parser::make_TYPECLASS(loc);
    ":+"        yy_push_state(layout_float); return yy::hawk_parser::make_IMPLEMENT(loc);
    
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
    
    "{"         loc.columns(-1); return yy::hawk_parser::make_LCURLY(loc);
    "}"         loc.columns(-1); return yy::hawk_parser::make_RCURLY(loc);
    "("         return yy::hawk_parser::make_LPAREN(loc);
    ")"         return yy::hawk_parser::make_RPAREN(loc);
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
    {string}    return yy::hawk_parser::make_STRING(yytext, loc);
    {integer}   return yy::hawk_parser::make_INTEGER(yytext, loc);
    {decimal}   return yy::hawk_parser::make_DECIMAL(yytext, loc);
}

<block_comment>{
    [^*\n]*         /* eat anything not a '*' */
    "*"+[^*/\n]*    /* eat anything not follow by '/'s */ 
    \n              loc.lines(1); loc.step();
    "*"+"/"         yy_pop_state(); loc.step();
}

<inline_comment>{
    \n              loc.step(); yy_pop_state(); unput('\n');
    .*              /* consume everything else */
}

[.\n]           { driver.error (loc, "invalid character"); }
<<EOF>>         {
                    if(lstack.size() == 1)
                        return yy::hawk_parser::make_END(loc);
                        
                    unput(NULL);
                    
                    while(lstack.size() > 1)
                    {
                        lstack.pop();
                        unput('}');
                    }
                    //std::cout << "Indent Level: " << ilevel << std::endl;
                }

%%


void hawk_driver::scan_begin ()
{
    yy_push_state(normal);
    lstack.push(Layout(1, Scheme::ROOT));
    
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