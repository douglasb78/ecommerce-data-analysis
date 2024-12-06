#ifndef hash_table_products_h
#define hash_table_products_h
#include "index.h"
// 1. gera hash com category_code, brand, price
// 2. ele dá o timestamp de um evento que aconteceu com os três
// .. assim a função hashing não usa o product_id do primeiro trabalho.

// .. aí procura no índice de acessos por busca binária
// achar um produto que foi acessado no timestamp
// se as infos (category_code, brand, price) não baterem com a do produto, é colisão de hash.. aí procura de novo

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
