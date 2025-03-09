#ifndef BTREE_H
#define BTREE_H

typedef struct BNode{
	int *keys; // array of m-1 keys
	struct BNode **children; // array of pointers for m children
	int n_keys; // number of inserted keys in the node
	bool is_leaf;
} BNode;

typedef struct BTree {
	BNode *root;
	int degree;
} BTree;

BTree* new_btree(int);
BNode* new_btree_node(int, bool);
void insert_not_full(BTree*, BNode*, int);
void split_child(BTree*, BNode*, BNode*, int);
void traverse(BNode*);
void free_btree_node(BNode*);

#endif
