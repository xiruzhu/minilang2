#include <stdio.h> /* for printf */
#include <stdlib.h>
#include <string.h>

typedef enum {
  int_val,
  float_val,
  str_val,
  err_val,
  ops_val,
  rw_val,
  id_val,
  key_val,
  end_val //it's 1, if true
}var;

union value{
  int		int_val;
  float		float_val;
  char *	str_val;
  char *	err_val;
  int       ops_val;
  int       rw_val;
  char *	id_val;
  char *	key_val;
  int       end_val; //it's 1, if true
};

typedef struct node{
	int type;
	union value val;

	int num_child_nodes;//Number of direct childs
	int max_child_nodes;
	struct node ** childs;
	struct node *  parent;  //pointer to parent
}node;

node * create_node(int type, union value val);
int add_node(node * parent, node * child);
int free_node(node * tree);
int delete_node(node * tree, node * deleted);
int adopt_children(node * new_parent, node * old_parent);

