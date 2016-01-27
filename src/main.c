#include "y.tab.h" 
#include <stdio.h>
int main (void)
{
	if(yyparse () == 0)
		printf("VALID\n");
	else
		printf("INVALID\n");
}