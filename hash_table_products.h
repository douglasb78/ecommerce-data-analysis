#ifndef hash_table_products_h
#define hash_table_products_h
#include "index.h"
// 1. gera hash com category_code, brand, price
// 2. ele d� o timestamp de um evento que aconteceu com os tr�s
// .. assim a fun��o hashing n�o usa o product_id do primeiro trabalho.

// .. a� procura no �ndice de acessos por busca bin�ria
// achar um produto que foi acessado no timestamp
// se as infos (category_code, brand, price) n�o baterem com a do produto, � colis�o de hash.. a� procura de novo

typedef struct entrada_tabela_produtos {
	unsigned int hash;
	long long int event_timestamp;
	int removed;
} HashEntradaProdutos;

typedef struct tabela_hash_produtos {
	unsigned long long int registros;
	struct entrada_tabela_produtos **hashes;
} TabelaHashProdutos;

TabelaHashProdutos* criar_tabela_hash_produtos(char filename[], unsigned long long int *linhas_escritas, TabelaHashProdutos *tabela);
unsigned int hash_produto(char category_code[64], char brand[64], unsigned long long int price);
long long int procurar_hash(TabelaHashProdutos *tabela, unsigned int hash_procurado, int skip, int mostrar, struct entrada_tabela_produtos **entrada_hash);
#endif
