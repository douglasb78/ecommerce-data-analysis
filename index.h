#ifndef index_h
#define index_h
#include "parser.h"

typedef struct header_indice {
	unsigned long long int registros;
} HeaderIndice;

typedef struct registro_indice_produto {
	unsigned long int product_id;
	unsigned long long int indice;
	int removed;
} RegistroIndiceProduto;

typedef struct registro_indice_acesso {
	long long int event_timestamp;
	unsigned long long int indice;
	int removed;
} RegistroIndiceAcesso;

typedef struct registro_marcas {
	char brand[64];
	unsigned long long int count;
} RegistroMarcas;

RegistroIndiceAcesso* criar_indice_acessos(char filename[], unsigned long long int *linhas_escritas);
RegistroIndiceProduto* criar_indice_produtos(char filename[], unsigned long long int *linhas_escritas);

RegistroIndiceAcesso* carregar_indice_acessos(char filename[], unsigned long long int *registros);
RegistroIndiceProduto* carregar_indice_produtos(char filename[], unsigned long long int *registros);

void salvar_indice_acessos(RegistroIndiceAcesso *indice, unsigned long long int registros, char filename[]);
void salvar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros, char filename[]);

void mostrar_indice_acessos_arquivo(char filename[]);
void mostrar_indice_produtos_arquivo(char filename[]);

void mostrar_indice_acessos(RegistroIndiceAcesso *indice, unsigned long long int registros);
void mostrar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros);

void mostrar_acessos_arquivo_intervalo(long long int timestamp_start, long long int timestamp_end);
int mostrar_acessos_produtos_intervalo_hash(char category_code_aux[64], char brand_aux[64], unsigned long long int price_aux,
										long long int timestamp_start, long long int timestamp_end, int mostrar
);

void mostrar_marcas_mais_compradas();

LineProduct* encontrar_produto_arquivo(char filename[], unsigned long int product_id);
#endif
