#include "abstract_tree.h"

//Creates a node
node * create_node(int type, union value val){
	//Given a type and a union value
	node * ret = (node *)calloc(1, sizeof(node));
	ret->childs = (node **)calloc(8, sizeof(node *));
	if(ret == NULL || ret->childs == NULL){
		fprintf(stderr, "Mem Alloc Error\n");
		exit(1);
	}
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
			parent->childs = (node **)realloc(parent->childs, parent->max_child_nodes * 2);
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