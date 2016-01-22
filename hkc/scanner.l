%{ /* -*- C++ -*- */
    #include <cerrno>
    #include <climits>
    #include <cfloat>
    #include <cstdlib>
    #include <assert.h>
    #include <string>
    #include <vector>
    #include <stack>
    
    #include "hkc/parser/driver.h"
    #include "parser.hpp"

    // The location of the current token.
    static yy::location loc;
    
    std::stack<unsigned int> indent_stack;
    unsigned int indent_level = 0;
    unsigned int nesting = 0;
    
    unsigned int white_count(char* line);
%}

%x do_bol
%x indent
%s normal

%option noyywrap yylineno batch debug


id    [a-zA-Z][a-zA-Z_0-9]*
digit [0-9]+

integer {digit}+
decimal {digit}*[\.]{digit}*

string \"(\\.|[^\\"])*\"

single_line_comment     "//".*"\n"
multi_line_comment      [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]
unterminated_comment    [/][*]

%{
    #define YY_USER_ACTION loc.columns(yyleng);
%}


%%


%{
    // Code run each time yylex is called.
    loc.step();
%}


<*>^[ ]*\n    loc.lines(1);

<do_bol>{
    ^[ ]*   { indent_level = yyleng-1; }
    .       {
                unput(*yytext);
                
                while(indent_level < indent_stack.top())
                {
                    indent_level = indent_stack.top();
                    indent_stack.pop();
                    unput('}');
                    unput(';');
                }
                
                if (indent_level == indent_stack.top()){
                    unput(';');
                }
                
                BEGIN(normal);
            }
}

<indent>{
    ^[ ]*   indent_level = yyleng - 1;
            
    .       {
                yyless(0);
                indent_stack.push(indent_level);
                
                BEGIN(normal);
                return yy::hawk_parser::make_LCURLY(loc);
            }

}
            
<normal>{

    \n          loc.lines(1); indent_level = 0; BEGIN(do_bol);
                
    [\t]        driver.error(loc, "Tabs are not allowed!"); exit(0);

    {single_line_comment}   loc.lines(1);
    
    ":="[ ]*"\n" BEGIN(indent); loc.lines(1); indent_level = 0; return yy::hawk_parser::make_FUNDEC(loc);        
    ":="         return yy::hawk_parser::make_FUNDEC(loc);
    ":-"         BEGIN(indent); return yy::hawk_parser::make_TYPDEC(loc);
    
    "("         nesting++; return yy::hawk_parser::make_LPAREN(loc);
    ")"         nesting--; return yy::hawk_parser::make_RPAREN(loc);
    "^"         return yy::hawk_parser::make_CARET(loc);
    "@"         return yy::hawk_parser::make_AT(loc);
    ";"         return yy::hawk_parser::make_SEMICOLON(loc);
    ":"         return yy::hawk_parser::make_COLON(loc);
    "!"         return yy::hawk_parser::make_EXCLAIM(loc);
    "{"         return yy::hawk_parser::make_LCURLY(loc);
    "}"         return yy::hawk_parser::make_RCURLY(loc);
    "<-"        return yy::hawk_parser::make_LARROW(loc);
    "->"        return yy::hawk_parser::make_RARROW(loc);
    "|"         return yy::hawk_parser::make_BAR(loc);
    "-"         return yy::hawk_parser::make_MINUS(loc);
    "+"         return yy::hawk_parser::make_PLUS(loc);
    "*"         return yy::hawk_parser::make_STAR(loc);
    "/"         return yy::hawk_parser::make_SLASH(loc);
    "let"       return yy::hawk_parser::make_LET(loc);


    {id}        return yy::hawk_parser::make_IDENT(yytext, loc);
    {string}    return yy::hawk_parser::make_STRING(yytext, loc);
    {integer}   return yy::hawk_parser::make_INTEGER(yytext, loc);
    {decimal}   return yy::hawk_parser::make_DECIMAL(yytext, loc);
    
    <<EOF>>     {
                    if(indent_stack.top() > 0)
                    {
                        while(indent_stack.top() > 0)
                        {
                            unput('}');
                            indent_stack.pop();
                        }
                        unput(';');
                    } else
                    {
                        return yy::hawk_parser::make_END(loc);
                    }
                }
}

[.\n]       { driver.error (loc, "invalid character"); }
<<EOF>>     { yyless(0); BEGIN(normal); }

%%

unsigned int white_count(char* line) {
  unsigned int count = 0 ;
  while (*line == ' ')
    count++, line++ ;
  return count ;
}

void hawk_driver::scan_begin ()
{
    BEGIN(normal);
    indent_stack.push(0);
    
    yy_flex_debug = trace_scanning;
    if (file.empty () || file == "-") {
        yyin = stdin;
    } else if(!(yyin = fopen(file.c_str(), "r")))
    {
        error("cannot open " + file + ": " + strerror(errno));
        exit(EXIT_FAILURE);
    }
}



void hawk_driver::scan_end ()
{
    fclose(yyin);
}