#ifndef btree_products_h
#define btree_products_h
#define ORDEM 5
#include "index.h"

typedef struct btree_products_node {
	int folha;
	int numeroElementos;
	struct registro_indice_produto* chaves[ORDEM - 1];
	struct btree_products_node* filhos[ORDEM];
} BT_Prod_Node;

typedef BT_Prod_Node* BTree_Prod;

BTree_Prod criar_arvore_produtos(void);
void destruir_arvore_produtos(BTree_Prod bt);
int procurarLinha(BTree_Prod bt, unsigned long int product_id);
void btInserir(BTree_Prod bt, RegistroIndiceProduto *novoProduto);
BTree_Prod criar_indice_produtos_arvore_b(char filename[], unsigned long long int *linhas_escritas);
void mostrar_arvore(BTree_Prod bt, int nivel, BT_Prod_Node* pai);
RegistroIndiceProduto* buscarProductArvore(BTree_Prod bt, unsigned long int product_id);
int removerProductArvore(BTree_Prod bt, unsigned long int product_id);
#endif
