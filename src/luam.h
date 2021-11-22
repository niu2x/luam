#ifndef LUAM_H
#define LUAM_H

extern char *token_name(int token);
extern int yyerror(char *message, ...);	
extern int yylex();

#endif