#ifndef btree_h
#define btree_h
#include "btree.c"

typedef struct btree_node {
	struct btree_node *left;
	void *data;
	struct btree_node *right;
} BTreeNode;

typedef struct btree_page {
	struct btree_node *nodes[6];
} BTreePage;

typedef struct btree {
	struct btree_page *root;
} BTree;

#endif

