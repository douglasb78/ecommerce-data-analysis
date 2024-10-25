#ifndef index_h
#define index_h
#include "parser.h"
typedef struct registro_indice_produto {
	unsigned long int product_id;
	unsigned long long int indice;
} RegistroIndiceProduto;

typedef struct registro_indice_acesso {
	long long int event_timestamp;
	unsigned long long int indice;
} RegistroIndiceAcesso;

RegistroIndiceProduto* criar_indice_produtos(char filename[]);
#endif
