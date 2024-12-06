#include <stdio.h>
#include <string.h> // memset()
#include "hash_table_products.h"
#include "utils.h"
#include "parser.h"

// category_code, brand, price
unsigned int hash_produto(char category_code[64], char brand[64], unsigned long long int price){
	char string_concatenada[512];
	memset(string_concatenada, '\0', sizeof(string_concatenada));
	snprintf(string_concatenada, sizeof(string_concatenada), "%s%s%llu", category_code, brand, price);
	unsigned int generated_hash = hashSDBM(string_concatenada, 512);
	return generated_hash;
}

// hash(category_code, brand, price) | timestamp
TabelaHashProdutos* criar_tabela_hash_produtos(char filename[], unsigned long long int *linhas_escritas, TabelaHashProdutos *tabela){
	LineAccess *line_access_aux = malloc(sizeof(struct line_access));
	LineProduct *line_product_aux = malloc(sizeof(struct line_product));
	unsigned long long int registros = 0;
	char buffer_linha[512];
	char char_aux = '\0';
	unsigned long long int indice = 0;
	int aux = 0;
	// pra incrementar, se já existir tabela hash:
	if(tabela->registros != 0){
		indice = (tabela->registros)+1;
		registros = (tabela->registros);
	}
	printf("registros: %llu indice: %llu\n", registros, indice);
	// abrir arquivo .csv
	FILE *file_access = fopen(filename, "r");
	if (file_access == NULL) {
        printf("Erro ao ler o conjunto de dados.\nColoque-o na mesma pasta do programa!");
        return NULL;
    }
    // contar número de registros:
    while (fgets(buffer_linha, sizeof(buffer_linha), file_access) != NULL) {
        registros++;
    }
    registros -= 1; // porque a 1ª linha do csv é exemplificação, diz o que vai ter
    printf("Número de linhas: %llu\n", registros);
    // criar entradas na tabela hash:
	struct entrada_tabela_produtos **hashes = malloc(registros * sizeof(struct entrada_tabela_produtos*));
	memset(hashes, 0, sizeof(hashes));
	if(tabela->registros != 0){
		memcpy(hashes, tabela->hashes, sizeof(tabela->hashes));
	}
	// popular a tabela hash:
    fseek(file_access, 0, SEEK_SET);
	while((char_aux = fgetc(file_access)) != EOF){
		buffer_linha[aux] = char_aux;
		aux++;
		if(char_aux == '\n' || char_aux == EOF){
			LineData *line_data = read_line(buffer_linha);
			memset(buffer_linha, '\0', sizeof(buffer_linha));
			aux = 0;
			if(strcmp(line_data->event_time, "event_time")==0) continue;
			hashes[indice] = (HashEntradaProdutos*)malloc(sizeof(struct entrada_tabela_produtos));
			hashes[indice]->hash = hash_produto(line_data->category_code, line_data->brand, line_data->price);
			hashes[indice]->event_timestamp = generate_timestamp(line_data->event_time);
			hashes[indice]->removed = 0;
			//printf("%llu %lld\n", hashes[indice]->hash, hashes[indice]->event_timestamp);
			free(line_data);
			indice += 1;
		}
	}
	// salvar no struct:
    tabela->hashes = hashes;
    tabela->registros = registros;
    *linhas_escritas = indice;
	return tabela;
}

long long int procurar_hash(TabelaHashProdutos *tabela, unsigned int hash_procurado, int skip, int mostrar, struct entrada_tabela_produtos **entrada_hash){
	if(mostrar && skip > 0)
		printf("* Colisão %d *\n", skip);
	for (unsigned long long int i = 0; i < tabela->registros; i++) {
        if (tabela->hashes[i] != NULL && tabela->hashes[i]->hash == hash_procurado && tabela->hashes[i]->removed != 1) {
        	if(skip == 0){
        		*entrada_hash = tabela->hashes[i];
            	return tabela->hashes[i]->event_timestamp;
			} else {
				skip -= 1;
			}
        }
    }
    return -1;
}
