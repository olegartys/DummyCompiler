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
#include <memory>

#include <Log.h>

#include <AST.h>

#undef LOG_TAG
#define LOG_TAG "BisonParser"

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
	EQUAL "="
	CEQ "=="
	CNEQ "!="
	CLT "<"
	CLTE "<="
	CGT ">"
	CGTE ">="
	
	LPAREN "("
	RPAREN ")"
	LBRACE "{"
	RBRACE "}"
	
	DOT "."
	COMMA ","
	PLUS "+"
	MINUS "-"
	MUL "*"
	DIV "/"
	
	RETURN "return"
	SEMICOLON ";"
	
	IF "if"
	ELSE "else"
	
	FOR "for"
	
	STRUCT "struct"
	PRIVATE "private"
	PUBLIC "public"
;

%token <std::string> INTEGER_CONST "integer"
%token <std::string> DOUBLE_CONST "double"

%token <std::string> IDENTIFIER "identifier"

%type <std::shared_ptr<NStatement>> stmt var_decl func_decl return_statement if_statement for_loop_statement struct_decl 
%type <std::shared_ptr<NStatement>> struct_field
%type <std::shared_ptr<NBlock>> struct_fields
%type <std::shared_ptr<NExpression>> expr assignment function_call struct_method_call
%type <std::shared_ptr<NStructFieldAccess>> struct_field_access
%type <std::shared_ptr<NExpression>> numeric
%type <std::shared_ptr<NIdentifier>> ident
%type <std::shared_ptr<NBlock>> program stmts block

%type <std::shared_ptr<ExpressionList>> call_args
%type <std::shared_ptr<VariableList>> func_decl_args

// FIXME : is not actually correct. Do sth with rule matching as it is mess
%type <int> binary_op 
%type <std::string> acess_specifier

%printer { yyoutput << $$; } <*>;

%start program

// FIXME: write macro to simpilfy move-construction
%%

program : 
	stmts { ctx.setRootNode($1); } 
;

stmts : 
	stmt { $$ = std::move(std::shared_ptr<NBlock>(new NBlock())); $$->mStatements.push_back($1); }
|   stmts stmt { $1->mStatements.push_back($2); /* Remember statement list for the next iteration (OR ir will be NULL) */ $$ = $1; }
;
	  
stmt : 
	var_decl { $$ = $1; } 
| 	func_decl { $$ = $1; }
|	struct_decl { $$ = $1; }
| 	return_statement { $$ = $1; }
|	if_statement { $$ = $1; }
|	for_loop_statement { $$ = $1; }
| 	expr { $$ = std::move(std::shared_ptr<NExpressionStatement>(new NExpressionStatement($1))); }
;

struct_decl :
	"struct" ident "{" struct_fields "}" { $$ = std::move(std::shared_ptr<NStructDecl>(new NStructDecl($2, $4))); }
;

struct_fields : 
	struct_field { $$ = std::move(std::shared_ptr<NBlock>(new NBlock())); $$->mStatements.push_back($1); }
|	struct_fields struct_field { $1->mStatements.push_back($2); $$ = $1; }
;

struct_field :
	acess_specifier var_decl { $$ = std::move(std::shared_ptr<NStructField>(new NStructField($1, $2))); }
|	acess_specifier func_decl { $$ = std::move(std::shared_ptr<NStructField>(new NStructField($1, $2))); }
;

acess_specifier :
	"private" { $$ = "private"; }
|	"public" { $$ = "public"; }
;

func_decl : 
	ident ident LPAREN func_decl_args RPAREN block { $$ = std::move(std::shared_ptr<NFunctionDeclaration>(new NFunctionDeclaration($1, $2, $4, $6))); }
;
		  
func_decl_args : 
		{ $$ = std::move(std::shared_ptr<VariableList>(new VariableList()));  }
| 	var_decl { $$ = std::move(std::shared_ptr<VariableList>(new VariableList())); $$->push_back($1); }
| 	func_decl_args COMMA var_decl { $1->push_back($3); $$ = $1; }
;
			   
block : 
	LBRACE stmts RBRACE  { $$ = $2; }
| 	LBRACE RBRACE { $$ = std::move(std::shared_ptr<NBlock>(new NBlock())); }
;
	  
var_decl : 
	ident ident { $$ = std::move(std::shared_ptr<NStatement>(new NVariableDeclaration($1, $2))); }
| 	ident ident EQUAL expr { $$ = std::move(std::shared_ptr<NStatement>(new NVariableDeclaration($1, $2, $4))); }
;

expr : 
	assignment { $$ = $1; }
| 	function_call { $$ = $1; }
| 	ident { $$ = $1; }
| 	expr binary_op expr { $$ = std::move(std::shared_ptr<NBinaryOp>(new NBinaryOp($1, $2, $3))); }
| 	LPAREN expr RPAREN { $$ = $2; }
| 	numeric { $$ = $1; }
|	struct_field_access { $$ = $1; }
|	struct_method_call { $$ = $1; }
;

struct_field_access :
	ident "." ident { $$ = std::move(std::shared_ptr<NStructFieldAccess>(new NStructFieldAccess($1, $3))); }
;

struct_method_call :
	ident "." function_call { $$ = std::move(std::shared_ptr<NStructMethodCall>(new NStructMethodCall($1, $3))); }
;

if_statement : 
	"if" "(" expr ")" block { $$ = std::move(std::shared_ptr<NIfElseStatement>(new NIfElseStatement($3, $5))); }
|	"if" "(" expr ")" block "else" block { $$ = std::move(std::shared_ptr<NIfElseStatement>(new NIfElseStatement($3, $5, $7))); }
;

for_loop_statement : 
	"for" "(" ident ";" expr ";" expr ")" block { $$ = std::move(std::shared_ptr<NForLoop>(new NForLoop($3, $5, $7, $9))); }
;

return_statement : 
	RETURN expr { $$ = std::move(std::shared_ptr<NStatement>(new NReturnStatement($2))); }
// | 	RETURN numeric { $$ = std::move(std::shared_ptr<NStatement>(new NReturnStatement($2))); } 
;
	 
assignment : 
	ident EQUAL expr { $$ = std::move(std::shared_ptr<NAssignment>(new NAssignment($1, $3))); }
|	struct_field_access EQUAL expr { $$ = std::move(std::shared_ptr<NAssignment>(new NAssignment($1, $3))); }
;
		   
function_call : 
	ident LPAREN call_args RPAREN { $$ = std::move(std::shared_ptr<NFunctionCall>(new NFunctionCall($1, $3))); }
;
			  
call_args : 
	{ $$ = std::move(std::shared_ptr<ExpressionList>(new ExpressionList())); }
| 	expr { $$ = std::move(std::shared_ptr<ExpressionList>(new ExpressionList())); $$->push_back($1) ; }
| 	call_args COMMA expr { $1->push_back($3); $$ = $1; }
;
		  
binary_op : 
	CEQ { $$ = token::TOK_CEQ; } 
| 	CNEQ { $$ = token::TOK_CNEQ; } 
|	CLT { $$ = token::TOK_CLT; } 
|	CLTE { $$ = token::TOK_CLTE; } 
| 	CGT { $$ = token::TOK_CGT; } 
| 	CGTE { $$ = token::TOK_CGTE; } 
|	PLUS { $$ = token::TOK_PLUS; } 
|	MINUS { $$ = token::TOK_MINUS; } 
| 	MUL { $$ = token::TOK_MUL; } 
|	DIV { $$ = token::TOK_DIV; }
;

numeric : 
	INTEGER_CONST { $$ = std::move(std::shared_ptr<NExpression>(new NIntegerConst($1))); }
|	DOUBLE_CONST { $$ = std::move(std::shared_ptr<NExpression>(new NDoubleConst($1))); }
;

ident : 
	"identifier" { $$ = std::move(std::shared_ptr<NIdentifier>(new NIdentifier($1))); }
;

%%

void yy::BisonParser::error(const std::string& err) { 
	Log::error(LOG_TAG, "Parser error: {}", err);
	exit(EXIT_FAILURE);
}



