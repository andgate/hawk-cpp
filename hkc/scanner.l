%{ /* -*- C++ -*- */
    # include <cerrno>
    # include <climits>
    #include  <cfloat>
    # include <cstdlib>
    # include <string>
    # include "hkc/parser/driver.h"
    # include "parser.hpp"

    // Work around an incompatibility in flex (at least versions
    // 2.5.31 through 2.5.33): it generates code that does
    // not conform to C89.  See Debian bug 333231
    // <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
    # undef yywrap
    # define yywrap() 1

    // The location of the current token.
    static yy::location loc;
%}

%option noyywrap nounput batch debug noinput
id    [a-zA-Z][a-zA-Z_0-9]*
digit [0-9]+
blank [ \t]

single_line_comment     "//".*
multi_line_comment      [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/]
unterminated_comment    [/][*]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}


%%


%{
  // Code run each time yylex is called.
  loc.step ();
%}

{single_line_comment}   loc.step();
{multi_line_comment}    loc.step();
{unterminated_comment}  { driver.error (loc, "unterminated comment found"); }

{blank}+    loc.step();
[\n]+       loc.lines(yyleng); loc.step();
";"         return yy::hawk_parser::make_SEMICOLON(loc);
":"         return yy::hawk_parser::make_COLON(loc);
"!"         return yy::hawk_parser::make_EXCLAIM(loc);
":="        return yy::hawk_parser::make_FUNDEC(loc);
":-"        return yy::hawk_parser::make_TYPDEC(loc);
"{"         return yy::hawk_parser::make_LCURLY(loc);
"}"         return yy::hawk_parser::make_RCURLY(loc);
"<-"        return yy::hawk_parser::make_LARROW(loc);
"->"        return yy::hawk_parser::make_RARROW(loc);
"|"         return yy::hawk_parser::make_BAR(loc);
"-"         return yy::hawk_parser::make_MINUS(loc);
"+"         return yy::hawk_parser::make_PLUS(loc);
"*"         return yy::hawk_parser::make_STAR(loc);
"/"         return yy::hawk_parser::make_SLASH(loc);
"("         return yy::hawk_parser::make_LPAREN(loc);
")"         return yy::hawk_parser::make_RPAREN(loc);
"return"    return yy::hawk_parser::make_RETURN(loc);


{digit}      {
  errno = 0;
  long int n = strtol (yytext, NULL, 10);
  if (! (LONG_MIN <= n && n <= LONG_MAX && errno != ERANGE))
    driver.error (loc, "integer is out of range");
  return yy::hawk_parser::make_INTEGER(n, loc);
}

{digit}*[\.]{digit}*      {
  errno = 0;
  long double n = strtold (yytext, NULL);
  if (! (LDBL_MIN <= n && n <= LDBL_MAX && errno != ERANGE))
    driver.error (loc, "integer is out of range");
  return yy::hawk_parser::make_DOUBLE(n, loc);
}

{id}       return yy::hawk_parser::make_IDENT(yytext, loc);
.          driver.error (loc, "invalid character");
<<EOF>>    return yy::hawk_parser::make_END(loc);


%%


void
hawk_driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      error ("cannot open " + file + ": " + strerror(errno));
      exit (EXIT_FAILURE);
    }
}



void
hawk_driver::scan_end ()
{
  fclose (yyin);
}
