/* Minilang Parser */
/* miniLang.y */

%{
#include <stdio.h> /* for printf */
#include <stdlib.h>
#include <string.h>
#include "abstract_tree.h"

extern char *yytext; /* string from scanner */
extern int line_num;
extern int line_char;

void yyerror(char * err_msg) {
					fprintf (stderr ,"Syntax error after \"%s\" at line [%d], character [%d]\n", yytext, line_num, line_char);
					for(int i = 0; i < strlen(yytext); i++)
						printf("Character [%d]: %d\n", i, yytext[i]);
			   }
int yylex(void);

%}


%union{ //This is for clarity
  int		int_val;
  float		float_val;
  char *	str_val;
  char *	err_val;
  int       ops_val;
  int       rw_val;
  char *	id_val;
  char *	key_val;
  int       term_val; //it's 1, if true

  struct node * root;
}

%token <int_val> INT
%token <float_val> FLOAT
%token <str_val> STRING
%token <err_val> ERR
%token <rw_val> RW
%token <id_val> ID
%token <term_val> TERM
%token <key_val> END
%token <key_val> WHILE
%token <key_val> DO
%token <key_val> DONE
%token <key_val> IF
%token <key_val> THEN
%token <key_val> ELSE
%token <key_val> STRING_TYPE
%token <key_val> VAR
%token <key_val> INT_TYPE
%token <key_val> FLOAT_TYPE
%token <ops_val> PLUS
%token <ops_val> MINUS
%token <ops_val> MULT
%token <ops_val> DIV
%token <key_val> KEY
%token <ops_val> EQ
%token <ops_val> ASSIGN
%token <int_val> EOP


%start expr
%left ADD MINUS
%left MULT DIV
%left UMINUS
%%


value		: INT
			| FLOAT
			| ID

type        : INT_TYPE
			| FLOAT_TYPE
			| STRING_TYPE

ops         : ADD
			| MULT
			| DIV
			| MINUS

expr        : decl_stmt stmt EOP
			| decl_stmt EOP
			| stmt EOP
			| ERR {yyerror($1); exit(1);}
			| EOP

decl_stmt	: VAR ID ASSIGN type TERM decl_stmt
			| VAR ID ASSIGN type TERM

stmt		: misc_stmt stmt
			| misc_stmt
			| while_stmt stmt
			| while_stmt
			| if_stmt stmt
			| if_stmt

misc_stmt	: ID EQ func_stmt TERM
			| ID EQ STRING TERM
			| RW ID TERM
			| RW STRING TERM

func_stmt	: value
			| MINUS func_stmt %prec UMINUS
			| value ops func_stmt


while_stmt	: WHILE value DO stmt DONE

if_stmt		: IF value THEN stmt ELSE stmt END
			| IF value THEN stmt END

%%