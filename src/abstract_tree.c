#include "abstract_tree.h"


//int float string
//int
//float
//string
//Vertical is for left
//Horizontal for right
int add_minus_type[3][3] = {
   {int_val, float_val, -1} ,   /*  initializers for row indexed by 0 */
   {float_val, float_val, -1} ,   /*  initializers for row indexed by 1 */
   {-1, -1, str_val}   /*  initializers for row indexed by 2 */
};

int mul_div_type[3][3] = {
   {int_val, float_val, -1} ,   /*  initializers for row indexed by 0 */
   {float_val, float_val, -1} ,   /*  initializers for row indexed by 1 */
   {-1, -1, -1}   /*  initializers for row indexed by 2 */
};

//Creates a node
node * create_node(int type, union value val){
	//Given a type and a union value
	node * ret = (node *)calloc(1, sizeof(node));
	ret->childs = (node **)calloc(8, sizeof(node *));
	if(ret == NULL || ret->childs == NULL){
		fprintf(stderr, "Mem Alloc Error\n");
		exit(1);
	}
	ret->max_child_nodes = 8;
	ret->type = type;
	ret->val = val;
	ret->parent = NULL;
	return ret;
}

// Add a child node to parent node
int add_node(node * parent, node * child){
	//Next step we set the Child's parent to parent
	if(child != NULL){
		child->parent = parent;//add the child to the childs in the parent
	}
	else
		return 1; //If a child is NULL, we cannot add it to a parent and thus return 1

	//So we need to add a child to parent node
	if(parent != NULL){
		if(parent->num_child_nodes >= parent->max_child_nodes){
			//We need to increase the size of childs
			printf("Testing\n");
			parent->childs = (node **)realloc(parent->childs, sizeof(node *) * parent->max_child_nodes * 2);
			if(parent->childs == NULL){
				fprintf(stderr, "Mem Realloc Error\n");
				exit(1);
			}
			parent->max_child_nodes *= 2;
		}
		parent->childs[parent->num_child_nodes++] = child;
	}
	return 0;
}

//Free festival
int free_node(node * tree){
	int i;
	node * parent;
	for(int i = 0; i < tree->num_child_nodes; i++){
		free_node(tree->childs[i]);
	}
	free(tree->childs);
	//So we freed all the childs but now we need to remove this from the parent as well
	parent = tree->parent;
	if(parent != NULL){
		for(i = 0; i < parent->num_child_nodes; i++){
			if(parent->childs[i] == tree){
				//Well we have to shift the array now
				for(int j = i; j < parent->num_child_nodes - 1; j++){
					parent->childs[j] = parent->childs[j+1];
				}
				parent->childs[parent->num_child_nodes -1] = NULL;
				parent->num_child_nodes--;
				break;
			}
		}
	}
	free(tree);
	return 0;
}

//Given a node pointer, we delete it from tree. We are assuming each tree has a unique node and not reused ones which may improve efficiency
//but make my life harder
//Returns 0 on success
//Returns 1 on failure
int delete_node(node * tree, node * deleted){
	int ret;
	if(tree == deleted){
		free_node(tree);
		return 0;
	}

	for(int i = 0; i < tree->num_child_nodes; i++){
		if((ret = delete_node(tree->childs[i], deleted)) == 0) //is zero
			return 0;
	}
	return 1;
}

//The purpose of this function is the transfer all children of old parent to new parent
int adopt_children(node * new_parent, node * old_parent){
	if(new_parent == NULL || old_parent == NULL)
		return 1; //Failure if either parent is equal to NULL
	for(int i = 0; i < old_parent->num_child_nodes; i++){
		add_node(new_parent, old_parent->childs[i]);
	}
	//now we free the old parent since it's pretty useless
	free_node(old_parent);
	return 0;
}

node * create_itm_node(int type){
	node * ret;
	ret = calloc(1, sizeof(node));
	ret->childs = (node **)calloc(8, sizeof(node *));
	if(ret == NULL || ret->childs == NULL){
		fprintf(stderr, "Mem Alloc Error\n");
		exit(1);
	}
	ret->max_child_nodes = 8;
	ret->type = type;
	ret->parent = NULL;

	return ret;
}

int print_traverse(node * n){
	int i;
	int err;
	if(n == NULL){
		printf("Invalid Tree\n");
		exit(1);
	}

	switch(n->type){
		case int_val:
					printf("%d", n->val.int_val); break;
		case float_val:
					printf("%f", n->val.float_val); break;
		case str_val:
					printf("%s", n->val.str_val); break;
		case mul_div_val:
					printf("%s", n->val.str_val); break;
		case add_min_val:
					printf("%s", n->val.str_val); break;
		case rw_val:
					printf("%s ", n->val.str_val); break;
		case stmt_list:
					for(int i = n->num_child_nodes - 1; i >= 0; i--){
						err = print_traverse(n->childs[i]);
						if(err != 0)
							return -1;
					}
					break;
		case assign_stmt:
					if(n->num_child_nodes == 2){
						print_traverse(n->childs[0]);
						printf(" = ");
						print_traverse(n->childs[1]);
						printf(";\n");
						break;
					}
					else{
						return -1;
					}
		case rw_stmt:
					if(n->num_child_nodes == 2){
						print_traverse(n->childs[0]);
						print_traverse(n->childs[1]);
						printf(";\n");
						break;
					}
					else{
						return -1;
					}
		case id_val:
					printf("%s", n->val.str_val); break;
		case type_int:
					printf("%s", n->val.str_val); break;
		case type_float:
					printf("%s", n->val.str_val); break;
		case type_str:
					printf("%s", n->val.str_val); break;
		case u_minus:
					printf("-"); break;
		case while_stmt:
					if(n->num_child_nodes == 2){
						printf("\nwhile ");
						print_traverse(n->childs[0]);
						printf(" do\n");
						print_traverse(n->childs[1]);
						printf("done\n");
						break;
					}
					else{
						return -1;
					}
		case if_stmt:
					if(n->num_child_nodes == 2){
						printf("\nif ");
						print_traverse(n->childs[0]);
						printf(" then\n");
						print_traverse(n->childs[1]);
						printf("endif\n");
						break;
					}
					else if(n->num_child_nodes == 3){
						printf("\nif ");
						print_traverse(n->childs[0]);
						printf(" then\n");
						print_traverse(n->childs[1]);
						printf("else\n");
						print_traverse(n->childs[1]);
						printf("endif\n");
						break;
					}
					else{
						return -1;
					}
		case decl_stmt_list:
					for(int i = n->num_child_nodes - 1; i >= 0; i--){
						err = print_traverse(n->childs[i]);
						if(err != 0)
							return -1;
					}
					printf("\n");
					break;
		case decl_stmt:
					if(n->num_child_nodes == 2){
						printf("var ");
						print_traverse(n->childs[0]);
						printf(": ");
						print_traverse(n->childs[1]);
						printf(";\n");
						break;
					}
					else{
						return -1;
					}
		case func_stmt:
					for(int i = n->num_child_nodes - 1; i >= 0; i--){
						err = print_traverse(n->childs[i]);
						if(err != 0)
							return -1;
					}
					break;
		case expr:
					for(int i = 0; i < n->num_child_nodes; i++){
						err = print_traverse(n->childs[i]);
						if(err != 0)
							return -1;
					}
					printf("\n");
					break;
		default:
				return -1;

	}

	return 0;
}

int print(node * n){
	char * string;
	if(n == NULL){
		printf("Invalid Tree\n");
		exit(1);
	}

	if( print_traverse(n) != 0){
		printf("Tree Printing Failure\n");
		return 1;
	}

	return 0;
}

sym_tbl * create_sym_tbl(){
	sym_tbl * ret = calloc(1, sizeof(sym_tbl));
	if(ret == NULL){
		printf("Memory Alloc Failed\n");
		exit(1);
	}

	ret->type = calloc(30, sizeof(int));
	ret->id = calloc(30, sizeof(char *));

	if(ret->type == NULL || ret->id == NULL){
		printf("Memory Alloc Failed\n");
		exit(1);
	}
	ret->size = 0;
	ret->max_size = 30;

	return ret;
}

int add_sym(int type, char * symbol, sym_tbl * ret){
	if( find_symbol(ret, symbol) != -1){
		printf("Identifier Redefinition...\n");
		exit(1);
	}
	if(ret->size >= ret->max_size){
		ret->type = realloc(ret->type, ret->max_size * 2 * sizeof(int));
		ret->id = realloc(ret->id, ret->max_size * 2 * sizeof(char *));
		ret->max_size *= 2;
		if(ret->type == NULL || ret->id == NULL){
		printf("Memory Alloc Failed\n");
		exit(1);
		}
	}

	ret->type[ret->size] = type;
	ret->id[ret->size++] = symbol;

	return 0;
}

int free_sym_tbl(sym_tbl * table){
	for(int i = 0; i < table->size; i++){
		free(table->id[i]);
	}
	free(table->id);
	free(table->type);
	free(table);
	return 0;
}

int find_symbol(sym_tbl * table, char * name){
	for(int i = 0; i < table->size; i++){
		if(strcmp(table->id[i], name) == 0)
			return table->type[i];
	}
	return -1;
}

//What is documentation
int load_sym_tbl(node * tree, sym_tbl * table){
	//
	node * d_l;
	int type;
	if(tree->type != expr){
		printf("Please give the base of a tree\n");
		return 1;
	}

	if(tree->num_child_nodes == 0){
		printf("Empty Tree\n");
		return 0;
	}

	if(tree->childs[0]->type == decl_stmt_list){
		printf("Loading Symbols....\n");
		d_l = tree->childs[0];
		for(int i = 0; i < d_l->num_child_nodes; i++){
			if(d_l->childs[i]->type == decl_stmt){
				add_sym(d_l->childs[i]->childs[1]->type, d_l->childs[i]->childs[0]->val.str_val, table);
			}
		}
		printf("Done...\n");
	}
	else{
		printf("Cannot find declaration list\n");
	}

	return 0;
}

int type_square(int l, int r, int op){
	if(l == int_val)
		l = 0;
	else if(l == float_val)
		l = 1;
	else
		l = 2;
	if(r == int_val)
		r = 0;
	else if(r == float_val)
		r = 1;
	else
		r = 2;
	if(op == add_min_val)
		return add_minus_type[l][r];
	else
		return mul_div_type[l][r];
}

int evaluate_func_stmt(node * tree, sym_tbl * table){
	int type1;
	int type2;
	printf("Test %d %d\n", tree->type, func_stmt);
	if(tree->type != func_stmt){
		return -1;
	}
	printf("Testing\n");
	switch(tree->num_child_nodes){
		case 1:
				if(tree->childs[0]->type == id_val)
					return find_symbol(table, tree->childs[0]->val.str_val) -11;
				return tree->childs[0]->type;
		case 2:
				return evaluate_func_stmt(tree->childs[1], table); //Since type will be the same as the - func
		default:
				//this will be something like x1 ops x2 ops... xn
				type1 = tree->childs[0]->type;
				if(type1 == id_val)
						type1 = find_symbol(table, tree->childs[0]->val.str_val) - 11;
				for(int i = 0; i < tree->num_child_nodes - 2; i += 2){
					type2 = tree->childs[i+2]->type;
					if(type2 == id_val)
						type2 = find_symbol(table, tree->childs[i+2]->val.str_val) - 11;

					printf("Testing %d %d %d\n", type1, type2, tree->childs[i+1]->type);

					type1 = type_square(type1, type2, tree->childs[i+1]->type);
					printf("Testing %d\n", type1);
				}
				printf("Returned %d\n", type1);
				return type1;
	}

}

int traverse_tree(node * tree, sym_tbl * table){
	int sym;
	//for(int i = 0; i < tree->num_child_nodes; i++){
		switch(tree->type){
			case while_stmt:
					//So a while is like this ....  while id|int do stmt done
					if(tree->childs[0]->type == id_val){
						sym = find_symbol(table, tree->childs[0]->val.str_val) - 11;
						if(sym != int_val && sym != float_val ){
							printf("Parsing Error... non int/float in while loop test\n");
							return 1;
						}
					}
					else if(tree->childs[0]->type != int_val && tree->childs[0]->type != float_val ){
						printf("Parsing Error... non int/float in while loop test\n");
						return 1;
					}
					if(traverse_tree(tree->childs[1], table) != 0)
						return 1;
			case if_stmt:
					if(tree->childs[0]->type == id_val){
						sym = find_symbol(table, tree->childs[0]->val.str_val) - 11;
						if(sym != int_val && sym != float_val ){
							printf("Parsing Error... non int/float in if test\n");
							return 1;
						}
					}
					else if(tree->childs[0]->type != int_val && tree->childs[0]->type != float_val ){
						printf("Parsing Error... non int/float in if test\n");
						return 1;
					}
					if(traverse_tree(tree->childs[1], table) != 0)
						return 1;
					if(tree->num_child_nodes == 3){
						if(traverse_tree(tree->childs[1], table) != 0)
						return 1;
					}
			case func_stmt:
					if( evaluate_func_stmt(tree, table) == -1){
						printf("Invalid types for operation\n");
						return 1;
					}
			case assign_stmt:
					if(tree->childs[1]->type == str_val){
						sym = find_symbol(table, tree->childs[0]->val.str_val);
						if(sym != str_val || sym == -1)
							return 1;
						return 0;
					}
					printf("testing %d\n", tree->childs[1]->type);
					sym = evaluate_func_stmt(tree->childs[1], table);
					if(sym == -1)
						return 1;
					if(tree->childs[0]->type != id_val){
						printf("Variable Required\n");
						return 1;
					}
					else
						if( type_square(find_symbol(table, tree->childs[0]->val.str_val)-11, sym, add_min_val) == -1)
							return 1;
			case stmt_list:
						print(tree);
						printf("%d\n", tree->num_child_nodes);
					for(int i = 0; i < tree->num_child_nodes; i++){
						printf("//////////////////////////////////\n");
						print(tree->childs[i]);
						printf("%d\n", tree->childs[i]->type);
						sym = traverse_tree(tree->childs[i], table);
						printf("sym %d\n", sym);
						if(sym == 1)
							return 1;
					}
			default:
					return 0;
		}
	//}
	return 0;
}

//Type check the damn tree
int type_check_tree(node * tree){
	sym_tbl * table = create_sym_tbl();
	load_sym_tbl(tree, table);

	if(tree->type != expr){
		printf("Please give the base of a tree\n");
		return 1;
	}

	for(int i = 0; i < tree->num_child_nodes; i++){
		if(tree->childs[i]->type == stmt_list){
			//Start traversing the tree

			if(traverse_tree(tree->childs[i], table) != 0)
				printf("Invalid Stmt %d \n", i);
			else
				printf("Valid Stmt %d \n", i);


		}
	}
	free(table);
}

