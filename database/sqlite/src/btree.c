#include <math.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

#include "btree.h"

BTree* new_btree(int d) {
	BTree *tree = (BTree*)malloc(sizeof(BTree));
	tree->root = NULL;
	tree->degree = d;
	return tree;
}

BNode* new_btree_node(int degree, bool is_leaf) {
	BNode *node = (BNode*)malloc(sizeof(BNode));
	node->keys = (int*)calloc(sizeof(int), (degree-1));
	node->children = (BNode**)calloc(sizeof(BNode*), degree);
	node->n_keys = 0;
	node->is_leaf = is_leaf;
	return node;
}

/**
 * inserts the `key` at the `node` which is a btree node
 *
 * the node is not full - it has less than m-1 keys. m is the degree of the btree
 */
void insert_not_full(BTree *tree, BNode *node, int key) {
	int i=node->n_keys-1;

	if(node->is_leaf) {
		// find the smallest key at i which is >= `key`
		// if no key is bigger then the key will be added at the end
		// key will be added at i+1
		// following loop is also shifting the keys to right and making space for the new key
		while(i>=0 && key<=node->keys[i]) {
			node->keys[i+1] = node->keys[i];
			i--;
		}

		node->keys[i+1] = key;
		node->n_keys += 1;
	} else {
		// before recursively calling this function for it's children
		// check whether the child is full or not
		// if full then we need to split it

		// find the child where key can be found - it is i+1
		while(i>=0 && key<=node->keys[i]) {
			i--;
		}
		
		if(node->children[i+1]->n_keys == tree->degree-1) {
			// child is full
			split_child(tree, node, node->children[i+1], i+1);

			// after splitting the children
			// we have another key at i+1
			// if key is >= keys[i+1] then we look at the new right child which is at i+2
			if(key>node->keys[i+1]) i++;
		}

		insert_not_full(tree, node->children[i+1], key);
	}
}

/** 
 * `split_child` will split the child `child` at index `child_index` of `parent` s children array into 2 parts. 
 * This function will split the `child` into 2 children whose parent is `parent`.
 * The `child` node needs to be filled - it should have m-1 keys inside it. 
 * For a children minimum keys mk = ceil(m/2) - 1.
 * The new right child will copy the keys from mk to n_keys - 1. 
 * It will move the mk-th child which is the left most key of the new right child to `parent`.
 * If the `child` is not a leaf node then it will copy the children from mk+1 till n_keys to the new right child.
 * **NOTE** `parent` should not be full when calling this function to split child
 */
void split_child(BTree *tree, BNode *parent, BNode *child, int child_index) {
	BNode *right_child = new_btree_node(tree->degree, child->is_leaf);

	// minimum number of keys for nodes except root node
	int min_keys = ceil(tree->degree / 2.0) - 1;

	int j=0;
	while(j<min_keys) {
		right_child->keys[j] = child->keys[j+min_keys];
		right_child->n_keys += 1;
		j++;
	}

	child->n_keys = min_keys;

	if(!right_child->is_leaf) {
		j = 0;
		while(j<=min_keys){
			right_child->children[j] = child->children[j+min_keys];
			child->children[j+min_keys] = NULL;
			j++;
		}
	}

	// shift the keys of parent to right starting from child_index
	// place the key at child index of parent
	j=child_index;
	while(j<parent->n_keys) {
		parent->keys[j+1] = parent->keys[j];
		j++;
	}
	parent->keys[child_index] = child->keys[min_keys-1];
	parent->n_keys += 1;

	// shift the children to right starting from child_index+1
	j = child_index+1;
	while(j<parent->n_keys+1) {
		parent->children[j+1] = parent->children[j];
		j++;
	}
	parent->children[child_index+1] = right_child;
}


void insert(BTree *tree, int key) {
	if(tree->root ==  NULL) {
		// btree is empty
		BNode *node = new_btree_node(tree->degree, true);
		node->keys[0] = key;
		node->n_keys = 1;
	
		tree->root = node;
		return;
	}
	
	if(tree->root->n_keys == tree->degree-1) {
		// root is full
		// split the root into two children
	
		BNode *new_root = new_btree_node(tree->degree, false);
		new_root->children[0] = tree->root;
		split_child(tree, new_root, tree->root, 0);

		// find which child will have the key
		// new root has only one key
		int i=0;
		if(key>new_root->keys[i]) i++;
		insert_not_full(tree, new_root->children[i], key);
		
		tree->root = new_root;
		return;
	}

	insert_not_full(tree, tree->root, key);
}

void traverse(BNode *current) {
	if(current == NULL) return;

	for(int i=0; i<current->n_keys; i++) {

		if(current->is_leaf) {
			printf("%d ", current->keys[i]);
		} else {
			traverse(current->children[i]);
		}
	}
	
	if(!current->is_leaf) {
		// last n-th child 
		// there are m children and m-1 keys in a btree 
		traverse(current->children[current->n_keys]);
	}
}

void free_btree(BNode *current) {
	if(current == NULL) return;

	if(current->is_leaf) {
		free(current->keys);
		free(current->children);
		free(current);
		return;
	}

	for(int i=0; i<=current->n_keys; i++) {
		free_btree(current->children[i]);
	}

	free(current->keys);
	free(current->children);
	free(current);
}

int main() {
	// int keys[] = {5, 3, 7, 1, 40, 20, 10, 59, 46, 23, 90, 2, 124};
	int keys[] = {5, 3, 7, 1, 40};
	int n = sizeof(keys)/sizeof(keys[0]);
	
	BTree *tree = new_btree(3);
	for(int i=0; i<n; i++) insert(tree, keys[i]);

	traverse(tree->root);
	printf("\n");

	free_btree(tree->root);
	free(tree);
	return 0;
}
