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
    
    void print_curr_loc();
    
    
    void step_tok();
    void skip_loc();
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
    // Code run each time a pattern is matched
    //#define YY_USER_ACTION      \
        
%}


%%


%{
    // Code run each time yylex is called.
    loc.step();
%}

<do_bol>{
    [ \t]*  step_tok();
    "\n"    step_tok(); loc.lines(1);
    "/*"    step_tok(); yy_push_state(block_comment);
    "//"    step_tok(); yy_push_state(inline_comment);
    .       {
                // Get the current character's position
                step_tok();
                
                yy_pop_state();
                
                std::string yytext_old(yytext);
                std::string out_buf;
                
                // Attempt to remove invalid layouts
                bool checkingInvalid = true;
                std::cout << "Checking for layouts that close left of column " << loc.begin.column << std::endl;
                while(checkingInvalid)
                {
                    Layout l = lstack.top();
                    
                    switch(l.scheme)
                    {
                    case ROOT:
                        checkingInvalid = false; // Root scheme is never invalid
                        break;
                    case FLOAT:
                        // Check to see if out of bounds
                        if(l.column >=  loc.begin.column)
                        {
                            // Close floating layout
                            std::cout << "Closing floating layout on column " << l.column << std::endl;
                            lstack.pop();
                            out_buf += '}';
                        } else { checkingInvalid = false; }
                        break;
                    case ALIGN:
                        // Check to see if out of bounds
                        if(l.column > loc.begin.column)
                        {
                            // Close aligned layout
                            std::cout << "Closing aligned layout on column " << l.column << std::endl;
                            lstack.pop();
                            out_buf += '}';
                        } else { checkingInvalid = false; }
                        break;
                    }
                }
                
                // An aligned layout is the signal to start a new floating layout.
                if(lstack.top().scheme == Scheme::ALIGN && lstack.top().column == loc.begin.column)
                {
                    // Manually add a new layout to the stack
                    lstack.push(Layout(lstack.top().column, Scheme::FLOAT));
                    std::cout << "Creating floating layout on column " << lstack.top().column << std::endl;
                    // Add the start of the new layout to the buffer
                    out_buf += '{';
                }
                
                // Add the yytext to the buffer.
                out_buf += yytext_old;
                
                std::cout << "Out buffer contains \"" << out_buf << "\"" << std::endl;
                
                for(auto it = out_buf.rbegin(); it != out_buf.rend(); ++it)
                    unput(*it);

                // Set the location back one character.
                loc -= 1;
                loc.step(); 
            }
            
    <<EOF>> yy_pop_state();
}

<layout_align>{
    [ \t]*  step_tok();
    "\n"    step_tok(); loc.lines(1);
    "/*"    step_tok(); yy_push_state(block_comment);
    "//"    step_tok(); yy_push_state(inline_comment);
    .       {
                step_tok();
                
                unput(*yytext);
                yy_pop_state();
                
                unput('{');
                unput('{');
                
                lstack.push(Layout(loc.begin.column, Scheme::ALIGN));
                std::cout << "Creating aligned layout on column " << lstack.top().column << std::endl;
                lstack.push(Layout(lstack.top().column, Scheme::FLOAT));
                std::cout << "Creating floating layout on column " << lstack.top().column << std::endl;
                
                loc -= 1;
                loc.step(); 
            }
            
    <<EOF>> yy_pop_state();
}

<layout_float>{
    [ \t]*  step_tok();
    "\n"    step_tok(); loc.lines(1);
    "/*"    step_tok(); yy_push_state(block_comment);
    "//"    step_tok(); yy_push_state(inline_comment);
    .       {
                step_tok();
                
                unput(*yytext);
                yy_pop_state();
                
                unput('{');
                
                lstack.push(Layout(lstack.top().column, Scheme::FLOAT));
                
                std::cout << "Creating floating layout on column " << lstack.top().column << std::endl;
                
                loc -= 1;
                loc.step(); 
            }
    <<EOF>> yy_pop_state();
}
            
<normal>{

    [ \t]*      step_tok();
    \n          step_tok(); loc.lines(1); yy_push_state(do_bol);
    "/*"        step_tok(); yy_push_state(block_comment);
    "//"        step_tok(); yy_push_state(inline_comment);
    
    ".:"        step_tok(); return yy::hawk_parser::make_PCOLON(loc);
    ":."        step_tok(); yy_push_state(layout_float); return yy::hawk_parser::make_COLONP(loc);
    ":="        step_tok(); yy_push_state(layout_align); return yy::hawk_parser::make_FUNCDEC(loc);
    ":-"        step_tok(); yy_push_state(layout_float); return yy::hawk_parser::make_TYPEDEC(loc);
    ":~"        step_tok(); yy_push_state(layout_float); return yy::hawk_parser::make_TYPECLASS(loc);
    ":+"        step_tok(); yy_push_state(layout_float); return yy::hawk_parser::make_IMPLEMENT(loc);
    
    "$"  step_tok(); yy_push_state(layout_align); return yy::hawk_parser::make_DOLLAR(loc);
    
    
    "->"        step_tok(); return yy::hawk_parser::make_RARROW(loc);
    "=>"        step_tok(); return yy::hawk_parser::make_RRARROW(loc);
    "<-"        step_tok(); return yy::hawk_parser::make_LARROW(loc);
    "<="        step_tok(); return yy::hawk_parser::make_LLARROW(loc);
    "<:"        step_tok(); return yy::hawk_parser::make_SUBTYPE(loc);
    
    "`"         step_tok(); return yy::hawk_parser::make_GRAVE(loc);
    "~"         step_tok(); return yy::hawk_parser::make_TILDE(loc);
    "!"         step_tok(); return yy::hawk_parser::make_EXCLAIM(loc);
    "?"         step_tok(); return yy::hawk_parser::make_QUESTION(loc);
    "@"         step_tok(); return yy::hawk_parser::make_AT(loc);
    "#"         step_tok(); return yy::hawk_parser::make_POUND(loc);
    "^"         step_tok(); return yy::hawk_parser::make_CARET(loc);
    "&"         step_tok(); return yy::hawk_parser::make_AMPERSAND(loc);
    
    "{"         return yy::hawk_parser::make_LCURLY(loc);
    "}"         return yy::hawk_parser::make_RCURLY(loc);
    "("         step_tok(); return yy::hawk_parser::make_LPAREN(loc);
    ")"         step_tok(); return yy::hawk_parser::make_RPAREN(loc);
    "["         step_tok(); return yy::hawk_parser::make_LBRACKET(loc);
    "]"         step_tok(); return yy::hawk_parser::make_RBRACKET(loc);
    "|"         step_tok(); return yy::hawk_parser::make_BAR(loc);
    
    ":"         step_tok(); return yy::hawk_parser::make_COLON(loc);
    ";"         step_tok(); return yy::hawk_parser::make_SEMICOLON(loc);
    "."         step_tok(); return yy::hawk_parser::make_PERIOD(loc);
    ","         step_tok(); return yy::hawk_parser::make_COMMA(loc);
    "<"         step_tok(); return yy::hawk_parser::make_LESSER(loc);
    ">"         step_tok(); return yy::hawk_parser::make_GREATER(loc);
    
    "-"         step_tok(); return yy::hawk_parser::make_MINUS(loc);
    "+"         step_tok(); return yy::hawk_parser::make_PLUS(loc);
    "*"         step_tok(); return yy::hawk_parser::make_STAR(loc);
    "/"         step_tok(); return yy::hawk_parser::make_SLASH(loc);
    "="         step_tok(); return yy::hawk_parser::make_EQUALS(loc);
    
    {id}        step_tok(); return yy::hawk_parser::make_ID(yytext, loc);
    {string}    step_tok(); return yy::hawk_parser::make_STRING(yytext, loc);
    {integer}   step_tok(); return yy::hawk_parser::make_INTEGER(yytext, loc);
    {decimal}   step_tok(); return yy::hawk_parser::make_DECIMAL(yytext, loc);
}

<block_comment>{
    [^*\n]*         /* eat anything not a '*' */
    "*"+[^*/\n]*    /* eat anything not follow by '/'s */ 
    \n              loc.lines(1); loc.step();
    "*"+"/"         yy_pop_state(); loc.step();
    <<EOF>>         yy_pop_state();
}

<inline_comment>{
    \n              yy_pop_state(); unput('\n');
    .*              step_tok();
    <<EOF>>         yy_pop_state();
}

[.\n]           { driver.error (loc, "invalid character"); }
<<EOF>>         {
                    if(lstack.size() == 1)
                        return yy::hawk_parser::make_END(loc);
                        
                    while(lstack.top().scheme != ROOT)
                    {
                        lstack.pop();
                        unput('}');
                    }
                    //std::cout << "Indent Level: " << ilevel << std::endl;
                }

%%

void step_tok()
{
    loc.step();
    loc += yyleng;
    print_curr_loc();
}

void skip_loc()
{
}

void print_curr_loc()
{
    std::cout << "Current position "
              << loc.begin.line << "." << loc.begin.column << "-"
              << loc.end.line << "." << loc.end.column << std::endl;
}

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