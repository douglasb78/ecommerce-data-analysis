#ifndef btree_h
#define btree_h
#include "btree.c"

typedef struct line_data { // (unsigned long long int: 18.446.744.073.709.551.615) (unsigned long int: 4.294.967.295)
	char event_time[32];
	char event_type[32];
	unsigned long int product_id;
	unsigned long long int category_id; // exemplo: 2.053.013.555.631.882.655
	char category_code[64];
	char brand[64];
	unsigned long long int price; // preço guardado em centavos
	unsigned long int user_id;
	char user_session[37];
} LineData;

typedef struct btree_node {
	struct btree_node *left;
	struct line_data *node_data;
	struct btree_node *right;
} BTreeNode;

typedef struct btree_page {
	struct btree_node *nodes[6];
} BTreePage;

typedef struct btree {
	struct btree_page *root;
} BTree;

#endif
