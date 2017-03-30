%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0.2"
%defines
%define parser_class_name {BisonParser}
%define api.token.constructor
%define api.value.type variant
%define parse.assert
%code requires
{
# include <string>
#include <cstdio>
class CompilerContext;
}

%param { CompilerContext& ctx }

%define parse.trace
%define parse.error verbose
%code
{
#include <CompilerContext.h>
}

%define api.token.prefix {TOK_}

%token 
	END  0  "end of file"
	TEMPERATURE "temperature"
	HEAT "heat"
	TARGET "target"
;

%token <int> NUMBER "number"
//%type <int> NUMBER

%token <std::string> STATE "state"
//%type <std::string> STATE

%printer { yyoutput << $$; } <*>;

%%

commands: | commands command;

command: heat_switch | target_set;

heat_switch: HEAT STATE {
	printf("\nHere\n");
};

target_set: TARGET TEMPERATURE NUMBER {
	printf("\%d\n", $3);
};

%%

void yy::BisonParser::error(const std::string&) { /* FIXME: handler errors there */}



