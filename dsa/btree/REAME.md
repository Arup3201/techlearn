
B-Tree is a data structure used by SQLite to manage both table and indexes. Here's the reason why a B-Tree is a good data structure for database -

1. Searching for a particular value (logarithmic time)
2. Inserting and deleting a value that you already found is fast
3. Traversing a range of values is fast 

Following diagram represents a B-Tree -

![B-Tree Diagram](https://en.wikipedia.org/wiki/File:B-tree.svg)

Unlike binary tree where only 2 max children of every node, b-tree can have m children. To make a b-tree balanced every internal node should atleast have m/2 (upper limit) children.

Before we dive into implementing the SQLite using B-Tree let's understand some concepts of B-Tree and how to implement their operation so that we can build our table and indices in SQLite.

### Concepts of B-Tree 

B-Tree is a balanced tree which is the generalized version of the binary search tree. It is a self-balancing tree data structure that allows search, insert and delete operation in logarithmic time. Unlike other self-balancing trees B-Tree is suited for storage systems that read and write large amount of data from memory like database and file systems.

A b-tree has 3 types of nodes -

- root node
- internal node 
- leaf node

A b-tree with of degree m has the following properties -

1. Every node has atmost m children
2. All leaf nodes appear at the same level
3. Every node except the root and leaf node has atleast $\ceil_(m/2)$ children
4. Root node has atleast 2 children unless it is a leaf node 
5. A non-leaf node with k children has k-1 keys 

For more information related to B-Tree look at [Wikipedia](https://en.wikipedia.org/wiki/B-tree).

A b-tree of order 5 looks like the following -

![B-Tree of order 5](https://en.wikipedia.org/wiki/B-tree#/media/File:B-tree.svg)

Some notations -
- $K$ - maximum number of potential search keys in the node 
- $k_{i}$ - search key at the 0-based node index 
- $pt_{i}$ - pointer to the i-th child that starts the sub-tree
- $pr_{i}$ - pointer to the record which stores the data for key $k_i$

The node structure of each type of nodes -

**Internal node**

$[pt_0, k_0, pt_1, pr_0, k_1, pt_2, pr_1, k_2, pt_3, pr_2, ..., k_i, pt_{i+1}, pr_i, ..., k_{K-1}, pt_{K}, pr_{K-1}]$

1. when $k_0$ exists - $pt_0}$ exists and points to the sub-tree where all keys are less than $k_0$.
2. when $k_i$ and $k_{i-1}$ exists - $pt_{i} will exist and point to a sub-tree which has keys less than $k_i$ and greater than $k_{i-1}$
3. when $k_{i-1}$ exist and $k_{i}$ does not exist - $pt_{i}$ will point to a sub-tree which has keys greater than $k_{i-1}$
4. when $k_{i-1}$ and $k_{i-1}$ does not exist - $pt_{i}$ will be empty
5. when $k_i$ exist - $pr_{i}$ will exist and point to a record which has the same value as $k_i$
6. when $k_i$ does not exist - $pr_i$ can't exist

**Leaf node**

$[pr_0, k_0, pr_1, k_1, ..., pr_{K-1}, k_{K-1}]$

1. when $k_i$ exist $pr_i$ points to a record of same value as $k_i$
2. when $k_i$ does not exist, $pr_i$ will be empty 

A point to note here - 

- Any internal node that is full need to be split before we can add any key. Same is true for root, we need to split the node first and then we get some space to add the key.
- Although the idea scenario is when the nodes that we try to insert do not have any space left we split it. But for implementation simplicity we split it before it. This makes the btree a little different than what it ideally would be but the end result remains same. And the main motive of searching will still give the same result.
- When we try to insert any key, they will be will be inserted at the leaf. All keys are present in the leaf, any value in the internal node is just a path to reach that leaf node which contains the key and data related to that key.

### Insertion in B-Tree

If a tree has degree `m` then -

1. Every internal node can have maximum `m` children and minimum `t = ceil(m/2)` children.
1. Every internal node can have maximum `m-1` keys.
2. Every internal node can have minimum `t-1` keys.

When inserting a node at B-Tree following cases may happen -

1. Tree is empty
2. Root of the tree is full 
3. One of the internal node in the path of the key is full
4. No internal node is full and key is inserted at a leaf node 

In case of 1 when tree is empty we just need to create a node and insert the key as it's first value.

In case of 2 when root is full, we split the root into 2 children, make a new root node, and put the last key of the left child (`t-1` th key) as first key in the new root.

In case of 3 where the root node is not full but when we tried to insert the key at the correct node we encountered a node that is full - then we can split the internal node into 2 children and put the right most key of the left child at the correct position in the parent node of this internal node.

You might have a question for case 2 and 3, how we are sure that when we try to insert the new key we need to split the root or the node in between? It could happen that where the key is supposed to go may already have space available to insert that key... Right?

The question is valid and it is true that when we try to insert a key, even root or some other node is full, the place where the key will be inserted may have space.

But the implementation of that will become tricker. This is a simplified method and makes sure that the end b-tree is valid and produces the same result as the ideal b-tree.

Here the logic is - 
- if root is full then one of the leaf nodes is full. And if the key that we try to insert now or later goes in that leaf node, then anyways we need splitting.
- if any other node apart from the root node is full then also the same logic follows. Some of it's leaf node attached to it is full,  and may need splitting if the key goes to that leaf node.
- when splitting the child, we just use the right most key of the left child to move it to parent. We do not try to use the key we are trying to insert because we can simply use the existing keys to split them into `(t-1)/2` parts and add the key to any suitable child at the right place. At the end the b-tree rule will still uphold.

The structure for b-tree and b-tree node -

```c
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
```

The function to insert keys at the b-tree -

```c
void insert(BTree *tree, int key) {
	if(tree->root ==  NULL) {
		// btree is empty
		BNode *node = new_btee_node(tree->degree, true);
		node->keys[0] = key;
		node->n_keys = 1;
	
		tree->root = node;
		return;
	}
	
	if(tree->root->n_keys == tree->degree-1) {
		// root is full
		// split the root into two children
	
		BNode *new_root = new_btee_node(tree->degree, false);
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
```

`split_child` function takes a parent node, and the child node that we want to split. It also takes the index of the child node in the parent nodes children array. It copies the keys from left child to right child from `t-1` till end. If it is an internal node then it also copies the children of the left node from `t-1` till `n+1`.

After that it attaches the key at `t-1` from left child at `child_index` to `parent` `keys` array. Here, `child_index` is the index of the current child in the children array of `parent` that we are trying to split. Then we shift the children of parent to right by 1 from `child_index+1` to make space for the new right child.

```c
void split_child(BTree *tree, BNode *parent, BNode *child, int child_index) {
	BNode *right_child = new_btee_node(tree->degree, child->is_leaf);

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

	j=child_index;
	while(j<parent->n_keys) {
		parent->keys[j+1] = parent->keys[j];
		j++;
	}
	parent->keys[child_index] = child->keys[min_keys-1];
	parent->n_keys += 1;

	j = child_index+1;
	while(j<parent->n_keys+1) {
		parent->children[j+1] = parent->children[j];
		j++;
	}
	parent->children[child_index+1] = right_child;
}
```

For nodes that are not full, we can proceed to insert the key at the right place if they are leaf node otherwise we go to their children. Before going to the children node for inserting, we need to make sure that the child node is not full. If it is full, then we need to split this child.

```c
void insert_not_full(BTree *tree, BNode *node, int key) {
	int i=node->n_keys-1;
	while(i>=0 && key<=node->keys[i]) {
		node->keys[i+1] = node->keys[i];
		i--;
	}
	
	if(node->is_leaf) {
		node->keys[i+1] = key;
		node->n_keys += 1;
	} else {
		if(node->children[i+1]->n_keys == tree->degree-1) {
			split_child(tree, node, node->children[i+1], i+1);

			if(key>node->keys[i+1]) i++;
		}

		insert_not_full(tree, node->children[i+1], key);
	}
}
```


