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

// FIXME : check for location argument to be sent when constructing

// FIXME: some conversion error
#undef yyterminate
#define yyterminate() return yy::BisonParser::make_END();

%}

%option noyywrap nounput batch debug noinput

int [0-9]+
double [0-9]+\.[0-9]*
identifier [a-zA-Z_][a-zA-Z0-9_]*

blank [ \t]+

%%

"=" { return yy::BisonParser::make_EQUAL(); }
"==" { return yy::BisonParser::make_CEQ(); }
"!=" { return yy::BisonParser::make_CNEQ(); }
"<" { return yy::BisonParser::make_CLT(); }
"<=" { return yy::BisonParser::make_CLTE(); }
">" { return yy::BisonParser::make_CGT(); }
">=" { return yy::BisonParser::make_CGT(); }
"(" { return yy::BisonParser::make_LPAREN(); }
")" { return yy::BisonParser::make_RPAREN(); }
"{" { return yy::BisonParser::make_LBRACE(); }
"}" { return yy::BisonParser::make_RBRACE(); }
"." { return yy::BisonParser::make_DOT(); }
"," { return yy::BisonParser::make_COMMA(); }
"+" { return yy::BisonParser::make_PLUS(); }
"-" { return yy::BisonParser::make_MINUS(); }
"*" { return yy::BisonParser::make_MUL(); }
"/" { return yy::BisonParser::make_DIV(); }
";" { return yy::BisonParser::make_SEMICOLON(); }

"if" { return yy::BisonParser::make_IF(); }
"else" { return yy::BisonParser::make_ELSE(); }

"for" { return yy::BisonParser::make_FOR(); }

"struct" { return yy::BisonParser::make_STRUCT(); }
"private" { return yy::BisonParser::make_PRIVATE(); }
"public" { return yy::BisonParser::make_PUBLIC(); }

"return" { return yy::BisonParser::make_RETURN(); }

{int} { return yy::BisonParser::make_INTEGER_CONST(yytext); }
{double} { return yy::BisonParser::make_DOUBLE_CONST(yytext); }
{identifier} { return yy::BisonParser::make_IDENTIFIER(yytext); }

[ \n]+;
{blank}+;

%%