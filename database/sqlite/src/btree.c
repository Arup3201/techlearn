/* 
 * B-tree implementation using C
 *
 * keys - 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
 * m = 3 => max children = 3 and max keys = 2
 * (1)
 * 1
 * (2)
 * 1 2
 * (3)
 * 1 2 [3]
 *   2
 *  / \
 * 1   3
 * (4)
 *   2
 *  / \
 * 1  3,4
 * (5)
 *   2
 *  / \
 * 1  3, 4[5]
 *   2, 4
 *  / | \
 *  1 3  5
 *  (6)
 *  2, 4
 * /  |  \
 * 1  3  5, 6
 * (7)
 *  2, 4
 * /  |  \
 * 1  3  5, 6[7]
 *     4
 *    / \
 *   2   6
 *  / \ / \
 *  1 3 5 7
 * (8)
 *     4
 *    / \
 *   2   6
 *  / \ / \
 * 1 3 5 7 [8]
 *     4
 *    / \
 *   2   6
 *  / \ / \
 * 1 3 5 7,8
 * (9)
 *     4
 *    / \
 *   2   6
 *  / \ / \
 * 1 3 5 7,8 [9]
 *     4
 *    / \
 *   2   6, 8
 *  / \ / \ \
 * 1 3 5  7  9
 * (10)
 *     4
 *    / \
 *   2   6, 8
 *  / \ / \ \
 * 1 3 5  7  9, 10
 */

#include<stdio.h>
#include<stdlib.h>

const int m = 3; // degree of the b-tree

typedef struct BNode{
	int *keys; // array of m-1 keys
	struct BNode **children; // array of pointers for m children
	int n_keys; // number of inserted keys in the node
} BNode;


BNode* new_btee_node() {
	BNode *node = (BNode*)malloc(sizeof(BNode));
	node->keys = (int*)calloc(sizeof(int), (m-1));
	node->children = (BNode**)calloc(sizeof(BNode*), m);
	node->n_keys = 0;
	return node;
}

int binary_search(int arr[], int n, int k) {
	int l=0, h=n-1;
	int index = -1;
	while(l<=h) {
		int mid = (h-l)/2 + l;
		if(arr[mid] > k) {
			h = mid - 1;
		}
		else {
			index = mid;
			l = mid + 1;
		}
	}

	return index;
}

void insert_key_to_node(BNode *node, int key, int pos) {

}

BNode* insert(BNode *head, int key) {
	BNode *node = new_btee_node();
	if(head == NULL) {
		node->keys[0] = key;
		node->n_keys = 1;
		return node;
	}

	// find the leaf node to add the key 
	int pos = binary_search(node->keys, node->n_keys, key);
	if(key < node->keys[pos] && node->children[pos] != NULL) insert(node->children[pos], key);
	else if(key < node->keys[pos] && node->children[pos] == NULL) insert_key_to_node(node, key, pos);
	else if(key >= node->keys[pos] && node->children[pos] != NULL) insert(node->children[pos+1], key);
	else if(key >= node->keys[pos] && node->children[pos] == NULL) insert_key_to_node(node, key, pos+1);
}

void print_btree(BNode *current) {
	for(int i=0; i<current->n_keys; i++) printf("%d ", current->keys[i]);
	
	for(int i=0; i<m; i++) {
		if(current->children[i] != NULL) {
			print_btree(current->children[i]);
		}
	}
}

void free_btree(BNode *current) {
	for(int i=0; i<current->n_keys+1; i++) print_btree(current->children[i]);

	free(current->keys);
	free(current->children);
	free(current);
}

int main() {
	int n=2;
	int keys[2] = {1, 2};
	BNode *head = NULL;
	for(int i=0; i<n; i++) head = insert(head, keys[i]);
	print_btree(head);
	printf("\n");
	// free_btree(head);
	return 0;
}
