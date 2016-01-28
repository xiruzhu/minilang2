#include "y.tab.h"
#include <stdio.h>
#include "abstract_tree.h"

extern node * tree;

int main (void)
{
	if(yyparse () == 0)
		printf("VALID\n");
	else
		printf("INVALID\n");

	printf("%p\n", tree);
	print(tree);

	type_check_tree(tree);

	free(tree);
}