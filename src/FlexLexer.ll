%{
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <string>

# include <CompilerContext.h>

# include "BisonParser.hpp"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// FIXME: some conversion error
#undef yyterminate
#define yyterminate() return yy::BisonParser::make_END();

%}

%option noyywrap nounput batch debug noinput

int [0-9]+
blank [ \t]+

%%

{int} return yy::BisonParser::make_NUMBER(-1);
heat return yy::BisonParser::make_HEAT();
on|off return yy::BisonParser::make_STATE(yytext);
target return yy::BisonParser::make_TARGET();
temperature return yy::BisonParser::make_TEMPERATURE();
[\n]+ ;
{blank}+ ;

%%