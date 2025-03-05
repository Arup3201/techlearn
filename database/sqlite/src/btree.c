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

#include<math.h>
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

BNode* insert(BNode *head, int key) {
	if(head == NULL) {
		// b-tree is empty
		BNode *node = new_btee_node();
		node->keys[0] = key;
		node->n_keys = 1;
		return node;
	}

	// find where the key can be found
	int i=0;
	while(i<head->n_keys && key>=head->keys[i]) {
		i++;
	}

	if(head->children[0] == NULL) {
		// a leaf node
		
		if(head->n_keys < m-1) {
			// node is not full yet

			// place the key at i
			for(int j=i+1; j<=head->n_keys; j++) head->keys[j] = head->keys[j-1];
			head->keys[i] = key;
			head->n_keys += 1;
			return head;
		} else {
			// root node which is full

			// find the median key that will become parent
			// we need to find median after appending the key to child keys array
			int keys[head->n_keys+1];
			for(int j=0; j<head->n_keys+1; j++) keys[j] = head->keys[j];
			for(int j=i+1; j<=head->n_keys; j++) keys[j] = keys[j-1];
			keys[i] = key;
			int median_index = ceil((head->n_keys+1)/2.0) - 1;

			BNode *splitted_right_child = new_btee_node();
			for(int j=median_index+1; j<head->n_keys; j++) {
				splitted_right_child->keys[splitted_right_child->n_keys] = keys[j];
				splitted_right_child->n_keys += 1;
			}
			head->n_keys = median_index;
			// parent is not full 
			// so we can insert the middle key of the current child as a key in the parent at the right position
			int k=i+1;
			while(k<head->n_keys+1) {
				head->keys[k] = head->keys[k-1];
			}

			BNode *new_root = new_btee_node();
			new_root->keys[0] = keys[median_index];
			new_root->n_keys += 1;
			new_root->children[0] = head;
			new_root->children[1] = splitted_right_child;

			return new_root;
		}
	}

	if(head->children[i]->n_keys >= m-1 && head->children[i]->children[0]==NULL) {
		// i-th children which is full and a leaf node

		if(head->n_keys < m-1) {
			// split the child into 2 nodes
			BNode *child = head->children[i];

			// find where the key can be found
			int i=0;
			while(i<child->n_keys && key>=child->keys[i]) {
				i++;
			}

			// find the median key that will become parent
			// we need to find median after appending the key to child keys array
			int keys[child->n_keys+1];
			for(int j=0; j<child->n_keys+1; j++) keys[j] = child->keys[j];
			for(int j=i+1; j<=child->n_keys; j++) keys[j] = keys[j-1];
			keys[i] = key;
			int median_index = ceil((child->n_keys+1)/2.0) - 1;

			BNode *splitted_right_child = new_btee_node();
			for(int j=median_index+1; j<child->n_keys; j++) {
				splitted_right_child->keys[splitted_right_child->n_keys] = keys[j];
				splitted_right_child->n_keys += 1;
			}
			child->n_keys = median_index;
			// parent is not full 
			// so we can insert the middle key of the current child as a key in the parent at the right position
			int k=i+1;
			while(k<head->n_keys+1) {
				head->keys[k] = head->keys[k-1];
			}
			head->keys[i] = keys[median_index];
			head->n_keys += 1;
			head->children[i+1] = splitted_right_child;
			return head;
		} else {
			// parent is full 
		}
	}

	head->children[i] = insert(head->children[i], key);
	return head;
}

void print_btree(BNode *current) {
	if(current == NULL) return;

	for(int i=0; i<current->n_keys+1; i++) {
		print_btree(current->children[i]);
	}
	
	for(int i=0; i<current->n_keys; i++) printf("%d ", current->keys[i]);
}

void free_btree(BNode *current) {
	for(int i=0; i<current->n_keys+1; i++) print_btree(current->children[i]);

	free(current->keys);
	free(current->children);
	free(current);
}

int main() {
	int keys[] = {5, 3, 10, 20, 12, 60};
	int n = sizeof(keys)/sizeof(keys[0]);
	BNode *head = NULL;
	for(int i=0; i<n; i++) head = insert(head, keys[i]);
	print_btree(head);
	printf("\n");
	// free_btree(head);
	return 0;
}
