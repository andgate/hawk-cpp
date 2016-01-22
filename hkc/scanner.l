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
    unsigned int current_line_indent = 0;
    unsigned int nesting = 0;
    
    unsigned int white_count(char* line);
%}

%x do_bol
%x indent
%x undent
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


<*>^[ ]*\n     {/* ignore blank lines forever */}

<do_bol>{
    ^[ ]*   { current_line_indent = white_count(yytext); }
    .       {
                yyless(0);
                
                if(current_line_indent < indent_stack.top()) {
                    BEGIN(undent);
                } else if(current_line_indent > indent_stack.top()) {
                    BEGIN(normal);
                } else {
                    BEGIN(normal);
                    return yy::hawk_parser::make_SEMICOLON(loc);
                }
            }
}

 /* The following are the rules that keep track of indentation. */
<indent>{
    "\n"    { /* ignore whitespace */ }
    ^[ ]*   { current_line_indent = white_count(yytext);
              indent_stack.push(current_line_indent);
              return yy::hawk_parser::make_LCURLY(loc);
            }
            
    .       {
                //unput(*yytext);
                yyless(0);
                BEGIN(normal);
            }
}

<undent>{
    .       {
                yyless(0);
                
                if(current_line_indent < indent_stack.top())
                {
                    indent_stack.pop();
                    return yy::hawk_parser::make_RCURLY(loc);
                } else
                {
                    indent_stack.push(current_line_indent);
                    BEGIN(normal);
                }
            }
}
            
<normal>{

    \n          BEGIN(do_bol);
                
    [\t]        driver.error(loc, "Tabs are not allowed!"); exit(0);

    {single_line_comment}   loc.lines(1);
    
    
    ":="        BEGIN(indent); return yy::hawk_parser::make_FUNDEC(loc);
    ":-"        BEGIN(indent); return yy::hawk_parser::make_TYPDEC(loc);
    
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
    
    
    <<EOF>>     yyless(0); BEGIN(undent); return yy::hawk_parser::make_SEMICOLON(loc);

    
}

[.\n]       { driver.error (loc, "invalid character"); }
<<EOF>>     {
                if(indent_stack.top() > 0)
                {
                    yyless(0);
                    indent_stack.pop();
                    return yy::hawk_parser::make_RCURLY(loc);
                } else
                {
                    return yy::hawk_parser::make_END(loc);
                }
            }

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