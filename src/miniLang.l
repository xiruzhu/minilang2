%{
#include "y.tab.h"
#include <errno.h>
int line_num = 1;
int line_char = 0;

typedef struct token{
	char * val;
	char err_str[50];
	int type;
}token;

int set_val(int type);

token * tokens_list;
size_t numToken = 0;
size_t token_cap = 1000;

%}
ID [a-z|A-Z][a-z|A-Z|0-9|"_"]*
STRING \"[0-9|a-z|A-Z|"!"|"."|"?"|" "|","]*\"
INT 0|[1-9][0-9]*
FLOAT [1-9][0-9]*"."[0-9]*|0"."[0-9]*|"."[0-9]*[1-9][0-9]*
END	";"
OPS	"+"|"-"|"*"|"/"|"="|":"
RW	"read"|"print"
%x REALLYEND
%%

5 -4;

"#".*		printf("Eating ...%s\n", yytext);
{STRING}	return set_val(STRING);
"if"		{set_val(STRING); return IF;}
"var"		{set_val(STRING); return VAR;}
"int"		{set_val(STRING);
				return INT_TYPE;
			}
"float"		{set_val(STRING); return FLOAT_TYPE;}
"then"		{set_val(STRING); return THEN;}
"else"		{set_val(STRING); return ELSE;}
"while"		{set_val(STRING); return WHILE;}
"do"		{set_val(STRING); return DO;}
"done"		{set_val(STRING); return DONE;}
"string"	{set_val(STRING); return STRING_TYPE;}
"endif"		{set_val(STRING); return END;}
{RW}		return set_val(RW);
{ID}		{set_val(STRING); return ID;}
"+"			{line_char++; return ADD;}
"-"			{line_char++; return MINUS;}
"*"			{line_char++; return MULT;}
"/"			{line_char++; return DIV;}
":"			{line_char++; return ASSIGN;}
"="			{line_char++; return EQ;}
{FLOAT}		return set_val(FLOAT);
{INT}		return set_val(INT);
";"			{line_char++; return TERM; }
"\n"		{line_char = 0; line_num++; }
" "|"\t"	line_char++;
<INITIAL><<EOF>>        { BEGIN(REALLYEND); return EOP; }
<REALLYEND><<EOF>>      { return 0; }
.			{set_val(STRING); return ERR;}


%%

int set_val(int type){
	switch(type){
		case STRING:
					yylval.str_val = (char *)malloc(strlen(yytext) + 1);
					if(yylval.str_val  == NULL){
						printf("Mem Alloc Failed\n");
						free(yylval.str_val);
						exit(1);
					}
					printf("%s\n", yytext);
					strcpy(yylval.str_val , yytext);
					return STRING;

		case INT:
					printf("%s\n", yytext);
					errno = 0;
					yylval.int_val = strtol(yytext, NULL, 10);
					if(errno == ERANGE || errno == EINVAL){
						printf("Integer parse error %s\n", yytext);
						return ERR;
					}
					return INT;

		case FLOAT:
					printf("%s\n", yytext);
					errno = 0;
					yylval.float_val = strtof(yytext, NULL);
					if(errno == ERANGE || errno == EINVAL){
						printf("Integer parse error %s\n", yytext);
						return ERR;
					}
					return FLOAT;

		default: break;
	}
	return -1;
}
