#ifndef index_h
#define index_h
#include "parser.h"

typedef struct header_indice {
	unsigned long long int registros;
} HeaderIndice;

typedef struct registro_indice_produto {
	unsigned long int product_id;
	unsigned long long int indice;
} RegistroIndiceProduto;

typedef struct registro_indice_acesso {
	long long int event_timestamp;
	unsigned long long int indice;
} RegistroIndiceAcesso;

RegistroIndiceProduto* criar_indice_produtos(char filename[], unsigned long long int *linhas_escritas);
RegistroIndiceProduto* carregar_indice_produtos(char filename[], unsigned long long int *registros);
void salvar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros, char filename[]);
void mostrar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros);
#endif
