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
node * tree;

void yyerror(char * err_msg) {
					fprintf (stderr ,"Syntax error after \"%s\" at line [%d], character [%d]\n", yytext, line_num, line_char);
					for(int i = 0; i < strlen(yytext); i++)
						printf("Character [%d]: %d\n", i, yytext[i]);
			   }
int yylex(void);
union value val;

%}


%union{ //This is for clarity
  int		int_val;
  float		float_val;
  char *	str_val;
  struct node * node_val;
}

%token <int_val> INT
%token <float_val> FLOAT
%token <str_val> STRING
%token <str_val> ERR
%token <str_val> RW
%token <str_val> ID
%token <str_val> TERM
%token <str_val> END
%token <str_val> WHILE
%token <str_val> DO
%token <str_val> DONE
%token <str_val> IF
%token <str_val> THEN
%token <str_val> ELSE
%token <str_val> STRING_TYPE
%token <str_val> VAR
%token <str_val> INT_TYPE
%token <str_val> FLOAT_TYPE
%token <str_val> ADD
%token <str_val> MINUS
%token <str_val> MULT
%token <str_val> DIV
%token <str_val> KEY
%token <str_val> EQ
%token <str_val> ASSIGN
%token <str_val> EOP

%type<node_val> value type ops func_stmt while_stmt if_stmt stmt misc_stmt expr decl_stmt


%start expr
%left ADD MINUS
%left MULT DIV
%left UMINUS
%%


value		: INT          {
						   val.int_val = $1;
						   $$ = create_node(int_val, val);
						   }
			| FLOAT        {
						   val.float_val = $1;
						   $$ = create_node(float_val, val);
						   }
			| ID           {
						   val.str_val = $1;
						   $$ = create_node(id_val, val);
						   }
type        : INT_TYPE
						   {
						   val.str_val = $1;
						   $$ = create_node(type_int, val);
						   }
			| FLOAT_TYPE
						   {
						   val.str_val = $1;
						   $$ = create_node(type_float, val);
						   }
			| STRING_TYPE
						   {
						   val.str_val = $1;
						   $$ = create_node(type_str, val);
						   }

ops         : ADD
						   {
						   val.str_val = "+";
						   $$ = create_node(add_min_val, val);
						   }
			| MULT		   {
						   val.str_val = "*";
						   $$ = create_node(mul_div_val, val);
						   }
			| DIV		   {
						   val.str_val = "/";
						   $$ = create_node(mul_div_val, val);
						   }
			| MINUS
						   {
						   val.str_val = "-";
						   $$ = create_node(add_min_val, val);
						   printf("TESt\n");
						   }

expr        : decl_stmt stmt EOP
				{
				$$ = create_itm_node(expr);
				add_node($$, $1);
				add_node($$, $2);
				tree = $$;
				}
			| decl_stmt EOP
				{
				$$ = create_itm_node(expr);
				add_node($$, $1);
				tree = $$;
				}
			| stmt EOP
				{
				$$ = create_itm_node(expr);
				add_node($$, $1);
				tree = $$;
				}
			| ERR {yyerror($1); exit(1);}
			| EOP
				{
				printf("File is empty\n");
				$$ = create_itm_node(expr);
				tree = $$;
				}

decl_stmt	: VAR ID ASSIGN type TERM decl_stmt
				{
				$$ = $6;
				node * declaration = create_itm_node(decl_stmt);
				val.str_val = $2;
				add_node($$, declaration);
				add_node(declaration, create_node(id_val, val));
				add_node(declaration, $4);
				}
			| VAR ID ASSIGN type TERM
				{
				node * declaration = create_itm_node(decl_stmt);
				val.str_val = $2;
				add_node(declaration, create_node(id_val, val));
				add_node(declaration, $4);

				$$ = create_itm_node(decl_stmt_list);
				add_node($$, declaration);
				}

stmt		: misc_stmt stmt
				{
					$$ = $2;
					add_node($$, $1);
				}
			| misc_stmt
				{
					$$ = create_itm_node(stmt_list);
					add_node($$, $1);
				}
			| while_stmt stmt
				{
					$$ = $2;
					add_node($$, $1);
				}
			| while_stmt
				{
				    $$ = create_itm_node(stmt_list);
					add_node($$, $1);
				}
			| if_stmt stmt
				{
					$$ = $2;
					add_node($$, $1);
				}
			| if_stmt
				{
				    $$ = create_itm_node(stmt_list);
					add_node($$, $1);
				}

misc_stmt	: ID EQ func_stmt TERM
				{
				$$ = create_itm_node(assign_stmt);
				val.str_val = $1;
				add_node($$, create_node(id_val, val));
				add_node($$, $3);
				}
			| ID EQ STRING TERM
				{
				$$ = create_itm_node(assign_stmt);
				val.str_val = $1;
				add_node($$, create_node(id_val, val));
				val.str_val = $3;
				add_node($$, create_node(str_val, val));
				}
			| RW ID TERM
				{
				$$ = create_itm_node(rw_stmt);
				val.str_val = $1;
				add_node($$, create_node(rw_val, val));
				val.str_val = $2;
				add_node($$, create_node(id_val, val));
				}
			| RW STRING TERM
				{
				$$ = create_itm_node(rw_stmt);
				val.str_val = $1;
				add_node($$, create_node(rw_val, val));
				val.str_val = $2;
				add_node($$, create_node(str_val, val));
				}

func_stmt	: value
				{
						$$ = create_itm_node(func_stmt);
						add_node($$, $1);
				}
			| MINUS func_stmt %prec UMINUS
				{
						$$ = $2;
						val.str_val = $1;
						add_node($$, create_node(u_minus, val));
				}
			| value ops func_stmt
				{
						$$ = $3;
						add_node($$, $2);
						add_node($$, $1);
				}


while_stmt	: WHILE value DO stmt DONE
				{
				$$ = create_itm_node(while_stmt);
				add_node($$, $2);
				add_node($$, $4);
				//We can get rid of while, do done since we can consider $1 as equality check and $2 as the rest
				}

if_stmt		: IF value THEN stmt ELSE stmt END
				{
				$$ = create_itm_node(if_stmt);
				add_node($$, $2);
				add_node($$, $4);
				add_node($$, $6);
				}
			| IF value THEN stmt END
				{
				$$ = create_itm_node(if_stmt);
				add_node($$, $2);
				add_node($$, $4);
			}

%%