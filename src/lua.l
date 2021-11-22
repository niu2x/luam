%option noyywrap
%option nodefault
%option yylineno


%{

#include <string.h>
#include <stdarg.h>
#include "lua.tab.h"
#include "luam.h"

#define MY_MAX(x, y) (x) > (y) ? (x) : (y)

static int input();

static char * str_pattern = 0;
static int str_pattern_size = 0;
static char * str_temp = 0;
static int str_temp_size = 0;

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
	if(c == '\\'){
		c = input();
	}
	str_temp[str_temp_size ++] = c;
	return !c;
}


static void str_shift() {
	for(int i = 0; i < str_temp_size - 1; i ++){
		str_temp[i] = str_temp[i+1];
	}
	-- str_temp_size;
}

extern int yyerror(char *message, ...);
	
%}

%%

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

	//printf("comment: %s\n", yytext);
}

"--" {
	int c = input();
	while(c && c != '\n'){
		c = input();
	}
	//printf("comment: %s\n", yytext);
}

[_a-zA-Z][_a-zA-Z0-9]* {return T_NAME;}
[-]?(0x[0-9a-fA-F]+)|([0-9]+("."[0-9]+)?) {return T_NUMBER;}
("\"")|("\'")|("["=*"[")  {
	str_pattern = strdup(yytext);
	str_temp = strdup(yytext);
	str_pattern_size = strlen(str_pattern);
	str_temp_size = 0;
	buffer_init(&str);
	//printf("str begin %d %s\n", yylineno, str_pattern);

	if(str_pattern_size >= 2) {
		str_pattern[0] = ']';
		str_pattern[str_pattern_size-1] = ']';
	}

	int eof = 0;
	while((!eof) && str_temp_size < str_pattern_size){ 
		if(str_read_char()) {
			eof = 1;
		}
	}
	while ((!eof) && strcmp(str_pattern, str_temp) != 0){
		buffer_push(&str, str_temp[0]);
		str_shift();
		if(str_read_char()) {
			eof = 1;
		}
	}
	buffer_push(&str, 0);

	free(str_pattern);
	free(str_temp);
	//printf("got str: %s\n", str.base);
	buffer_free(&str);

	if(eof) yyterminate();

	return T_STRING;
}

\r {}
\n {}
. {yyerror("unexpected: %s line: %d\n", yytext, yylineno);}

<<EOF>> {yyterminate();}

%%

int yyerror(char *message, ...) {
	va_list ap;
	va_start(ap, message);
	int n = vfprintf(stderr, message, ap);
	va_end(ap);
	return n;
} 


int main() {
		
	struct buffer_t input;
	buffer_init(&input);

	int c = fgetc(stdin);
	while(!feof(stdin)) {
		buffer_push(&input, c);
		c = fgetc(stdin);
	}
	buffer_push(&input, 0);

	YY_BUFFER_STATE bs = yy_scan_string(input.base);

	int token;

	//while(token = yylex()) {
	//	 printf("token %s\n", token_name(token));
	//}

	yyparse();

	yy_delete_buffer(bs);

	buffer_free(&input);

	return 0;
}