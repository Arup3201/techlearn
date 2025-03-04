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

#include <math.h>
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
	int index = 0;
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

BNode* insert(BNode *head, int key) {
	int key_index = binary_search(head->keys, head->n_keys, key);
	if(head->children[0] == NULL) {
		// it is a leaf node

		if(head->n_keys >= m-1) {
			// leaf node is full
			
			int median_index = ceil((head->n_keys + 1) / 2.0) - 1;
			int parent_key;
			if(key_index == median_index) parent_key = key;
			else if(key_index < median_index) parent_key = head->keys[median_index - 1];
			else parent_key = head->keys[median_index];


		}

		for(int i=head->n_keys; i>key_index; i--) {
			head->keys[i] = head->keys[i-1];
		}
		head->keys[key_index] = key;
		head->n_keys += 1;
		return head;
	}

	if(key >= head->keys[key_index]) {
		return insert(head->children[key_index+1], key);
	} else {
		return insert(head->children[key_index], key);
	}
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
