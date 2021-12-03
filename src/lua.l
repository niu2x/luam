%option noyywrap
%option nodefault
%option yylineno


%{

#include <string.h>
#include <stdarg.h>
#include "lua.tab.h"
#include "luam.h"
#include "ast.h"

#define MY_MAX(x, y) (x) > (y) ? (x) : (y)

static int input();

static char * str_pattern = 0;
static int str_pattern_size = 0;
static char * str_temp = 0;
static char * char_is_escaped = 0;
static int str_temp_size = 0;
static char last_char_is_escaped = 0;

struct buffer_t {
	char *base;
	int capacity;
	int size;
};

static void buffer_init(struct buffer_t *buffer) {
	buffer->base = 0;
	buffer->capacity = 0;
	buffer->size = 0;
}

static void buffer_free(struct buffer_t *buffer) {
	if(buffer->base) free(buffer->base);
	buffer->base = 0;
	buffer->capacity = 0;
	buffer->size = 0;
}

static void buffer_resize(struct buffer_t *buffer, int new_capacity) {
	char *new_base = (char *) malloc(new_capacity);
	if(buffer->size) {
		memcpy(new_base, buffer->base, buffer->size);
	}
	if(buffer->base)
		free(buffer->base);
	buffer->base = new_base;
	buffer->capacity = new_capacity;
}

static void buffer_push(struct buffer_t *buffer, char c) {
	int new_size = 1 + buffer->size;
	while(new_size > buffer->capacity) {
		buffer_resize(buffer, MY_MAX(buffer->capacity * 2, new_size));
	}
	buffer->base[buffer->size ++ ] = c;
}


static struct buffer_t str;


static int str_read_char() {
	int c = input();
	last_char_is_escaped = c == '\\';
	if(last_char_is_escaped && c) {
		c = input();
	}
	if(char_is_escaped)
	char_is_escaped[str_temp_size] = last_char_is_escaped;
	str_temp[str_temp_size ++] = c;
	return !c;
}


static void str_shift() {
	for(int i = 0; i < str_temp_size - 1; i ++){
		str_temp[i] = str_temp[i+1];
		if(char_is_escaped)
		char_is_escaped[i] = char_is_escaped[i+1];
	}
	-- str_temp_size;
}

extern int yyerror(char *message, ...);
	
%}

%x MACRO

%%

"MACRO_DEV_START()" {return TM_MACRO_DEV_START;}
"MACRO_DEV_END()" {return TM_MACRO_DEV_END;}
";" {return T_SEMICOLON;}
":" {return T_COLON;}
"return" {return T_RETURN;}
"break" {return T_BREAK;}
"=" {return T_ASSIGN;}
"do" {return T_DO;}
"end" {return T_END;}
"repeat" {return T_REPEAT;}
"until" {return T_UNTIL;}
"if" {return T_IF;}
"then" {return T_THEN;}
"else" {return T_ELSE;}
"elseif" {return T_ELSEIF;}
"for" {return T_FOR;}
"in" {return T_IN;}
"," {return T_COMMA;}
"function" {return T_FUNCTION;}
"local" {return T_LOCAL;}
"while" {return T_WHILE;}
"." {return T_DOT;}
"(" {return T_OP;}
")" {return T_CP;}
"[" {return T_OB;}
"]" {return T_CB;}
"{" {return T_OC;}
"}" {return T_CC;}
"nil" {return T_NIL;}
"true" {return T_TRUE;}
"false" {return T_FALSE;}
"..." {return T_VARGS;}
"function" {return T_FUNCTION;}
"-" {return T_MINUS;}
"not" {return T_NOT;}
"#" {return T_LEN;}
"function" {return T_FUNCTION;}
"==" {return T_EQ;}
"+" {return T_ADD;}
"*" {return T_MUL;}
"/" {return T_DIV;}
"^" {return T_POW;}
"%" {return T_MOD;}
".." {return T_CONCAT;}
"<" {return T_LESS;}
"<=" {return T_LE;}
">" {return T_GREATE;}
">=" {return T_GE;}
"~=" {return T_NE;}
"and" {return T_AND;}
"or" {return T_OR;}
[ \t] {}

"--["=*"[" {
	int n = strlen(yytext) - 2;
	char *sz = (char *)malloc(n + 1);
	int i = 0;
	sz[i ++ ] = ']';
	for(; i < n - 1; i ++)
		sz[i] = '=';
	sz[i ++ ] = ']';
	sz[i ++ ] = 0;

	str_temp = strdup(sz);
	str_temp_size = 0;
	last_char_is_escaped = 0;

	int eof = 0;
	while((!eof) && str_temp_size < n){ 
		if(str_read_char()) {
			eof = 1;
		}
	}

	while ((!eof) && strcmp(sz, str_temp) != 0){
		//printf("str_temp %02x%02x == %02x%02x \n", sz[0], sz[1], str_temp[0], str_temp[1]);
		str_shift();
		if(str_read_char()) {eof = 1;}
	}
	free(str_temp);
	free(sz);

	if(eof) yyterminate();

	// printf("comment: %s\n", yytext);

	// return T_COMMENT;
}

"--##" {BEGIN(MACRO);}

<MACRO>"define" {return TM_DEFINE;}
<MACRO>"ifdef" {return TM_IFDEF;}
<MACRO>"else" {return TM_ELSE;}
<MACRO>"endif" {return TM_ENDIF;}
<MACRO>" " {}
<MACRO>[_a-zA-Z][_a-zA-Z0-9]* {yylval.sz = strdup(yytext); return T_NAME;}
<MACRO>"\r" {}
<MACRO>. {yyerror("unexpected: %s line: %d\n", yytext, yylineno);}
<MACRO>"\n" {BEGIN(INITIAL);}

"--" {
	//buffer_init(&str);
	int eof = 0;
	int c = input();
	if(!c) eof = 1;
	while((!eof) && c != '\n'){
		c = input();
		//buffer_push(&str, c);
		if(!c) eof = 1;
	}
	//printf("comment: %s\n", yytext);
	if(eof) yyterminate();

	// yylval.sz = strdup(str.base);
	// buffer_free(&str);
	// return T_COMMENT;
}

[_a-zA-Z][_a-zA-Z0-9]* {yylval.sz = strdup(yytext); return T_NAME;}
[-]?(0x[0-9a-fA-F]+)|([0-9]+("."[0-9]*)?("e"([+-])?[0-9]+)?) { yylval.sz = strdup(yytext); return T_NUMBER;}
[-]?"."([0-9]+)?("e"([+-])?[0-9]+)? {yylval.sz = strdup(yytext); return T_NUMBER;}
("\"")|("\'")|("["=*"[")  {
	str_pattern = strdup(yytext);
	str_temp = strdup(yytext);
	str_pattern_size = strlen(str_pattern);
	str_temp_size = 0;
	last_char_is_escaped = 0;
	buffer_init(&str);

	for(int i = 0; i < str_pattern_size; i ++)
		buffer_push(&str, str_pattern[i]);

	//printf("str begin %d %s\n", yylineno, str_pattern);

	if(str_pattern_size >= 2) {
		str_pattern[0] = ']';
		str_pattern[str_pattern_size-1] = ']';
	}

	char_is_escaped = strdup(str_pattern);
	memset(char_is_escaped, 0, str_pattern_size);

	int eof = 0;
	while((!eof) && str_temp_size < str_pattern_size){ 
		if(str_read_char()) {
			eof = 1;
		}
	}
	//printf("str_pattern %s\n", str_pattern);
	//printf("str_temp %s %d\n", str_temp);
	while ((!eof) && (strcmp(str_pattern, str_temp) != 0 || (last_char_is_escaped && str_pattern_size == 1))){
		if(char_is_escaped[0])
			buffer_push(&str, '\\');
		buffer_push(&str, str_temp[0]);
		str_shift();
		if(str_read_char()) {
			eof = 1;
		}

		//printf("str_pattern %s\n", str_pattern);
		//printf("str_temp %s\n", str_temp);
		//printf("eof %d %x\n", eof, last_char);
	}

	for(int i = 0; i < str_pattern_size; i ++)
		buffer_push(&str, str_pattern[i]);
		
	buffer_push(&str, 0);


	free(str_pattern);
	free(str_temp);
	free(char_is_escaped);
	char_is_escaped = 0;
	//printf("got str: %s\n", str.base);

	if(eof) yyterminate();

	yylval.sz = strdup(str.base);
	buffer_free(&str);

	return T_STRING;
}

\r {}
\n {}
. {yyerror("unexpected: %s line: %d\n", yytext, yylineno);}

<<EOF>> {yyterminate();}

%%

static int had_error = 0;

int yyerror(char *message, ...) {
	had_error = 1;
	va_list ap;
	va_start(ap, message);
	int n = vfprintf(stderr, message, ap);
	va_end(ap);
	return n;
} 

block_t *root_block = 0;

block_t *luam_parse(char *sz) {
	yylineno = 1;
	root_block = 0;
	had_error = 0;
	YY_BUFFER_STATE bs = yy_scan_string(sz);
	yyparse();
	yy_delete_buffer(bs);

	if(had_error){
		destroy_block(root_block);
		root_block = 0;
	}

	return root_block;
}