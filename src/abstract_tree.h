#include <stdio.h> /* for printf */
#include <stdlib.h>
#include <string.h>

typedef enum {
  int_val,
  float_val,
  str_val,
  err_val,
  mul_div_val,
  add_min_val,
  stmt_list,
  func_stmt,
  rw_val,
  id_val,
  key_val,
  type_int,
  type_float,
  type_str,
  u_minus,
  while_stmt,
  if_stmt,
  assign_stmt,
  decl_stmt,
  decl_stmt_list,
  expr,
  rw_stmt,
  end_val //it's 1, if true
}var;

union value{
  int		int_val;
  float		float_val;
  char *	str_val;
};

typedef struct node{
	int type;
	union value val;

	int num_child_nodes;//Number of direct childs
	int max_child_nodes;
	struct node ** childs;
	struct node *  parent;  //pointer to parent
}node;

typedef struct symbol_table{
  int * type;
  char ** id;

  int size;
  int max_size;
}sym_tbl;


node * create_node(int type, union value val);
node * create_itm_node(int type);
int add_node(node * parent, node * child);
int free_node(node * tree);
int delete_node(node * tree, node * deleted);
int adopt_children(node * new_parent, node * old_parent);
int print(node * node);

sym_tbl * create_sym_tbl();
int add_sym(int type, char * symbol, sym_tbl * table);
int free_sym_tbl(sym_tbl * table);
int load_sym_tbl(node * tree, sym_tbl * table);
int type_check_tree(node * tree);
int find_symbol(sym_tbl * table, char * name);
int evaluate_func_stmt(node * tree, sym_tbl * table);
int find_symbol(sym_tbl * table, char * name);

