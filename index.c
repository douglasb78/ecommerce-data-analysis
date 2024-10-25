#include <stdio.h>
#include <string.h> // memmove()
#include <stdlib.h> // malloc()
#include "index.h"
#include "parser.h"


void mostrar_indice(RegistroIndiceProduto *indice, unsigned long long int registros){
	printf("\n===\n");
	for(unsigned long long int i=0; i<registros; i++){
		printf("%lu - %llu\n", indice[i].product_id, indice[i].indice);
	}
	printf("===\n\n");
}


void insercao_ordenada(RegistroIndiceProduto *indice, unsigned long int productid_registro, unsigned long long int indice_registro, unsigned long long int registros){
	// se é o primeiro elemento:
	if(indice[0].indice == -1){
		indice[0].indice = indice_registro;
		indice[0].product_id = productid_registro;
		return;
	} else {
		// se é antes do primeiro da lista:
		if(productid_registro < indice[0].product_id){
			memcpy(indice+sizeof(struct registro_indice_produto), indice, (registros-1) * sizeof(struct registro_indice_produto));
			indice[0].indice = indice_registro;
			indice[0].product_id = productid_registro;
			return;
		}  else { // se é depois do primeiro, no meio ou no fim:
			int flag = 1;
			unsigned long long int i=1, j=0;
			// loop até achar um valor maior .. ou posição vazia no final:
			while(i < registros){
				if(productid_registro < indice[i].product_id){
					flag = 0;
					break;
				}
				if(indice[i].indice == -1) break;
				i++;
			}
			if(flag){ // inserção no fim
				indice[i].indice = indice_registro;
				indice[i].product_id = productid_registro;
			} else { // inserção no meio:
				//pegar o último elemento:
				//for(j=1; j<registros; j++)
				//	if(indice[j].indice == -1) break;
				//j--;
				//ia fazer com memcpy, mas estava dando erros, vou só fazer do jeito mais simples para terminar:
				// deslocar:
				j = i;
				for(j=registros; j>i; j--){
					if(indice[j-1].product_id == -1) continue;
					indice[j].indice = indice[j-1].indice;
					indice[j].product_id = indice[j-1].product_id;
				}
				indice[i].indice = indice_registro;
				indice[i].product_id = productid_registro;
			}
		}
	}
}

RegistroIndiceProduto* criar_indice_produtos(char filename[]){
	int i = 0;
	unsigned long long int registros = 0;
	FILE *file_products = fopen("arquivo_produtos.bin", "rb");
	if(file_products == NULL){
		printf("Erro ao ler o arquivo binário de produtos. Certifique-se dele ter sido gerado!\n");
		return NULL;
	}
	// Contar número de produtos:
	LineProduct *line_product_aux = (LineProduct*)malloc(sizeof(struct line_product));
	fseek(file_products, (long)sizeof(struct line_product) * -1, SEEK_END);
	fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products);
	registros = line_product_aux->indice + 1; // Tem '+1'porque o índice começa em 0 (zero)
	printf("Criando índice...\n");
	printf("Há %llu registros no arquivo binário de produtos.\n", registros);
	
	// Criar índice:
	RegistroIndiceProduto *indice = (RegistroIndiceProduto*)calloc(registros+1, (long)sizeof(struct registro_indice_produto));
	for(unsigned long long int i=0; i<registros; i++){
		indice[i].indice = -1;
	}
	
	fseek(file_products, 0, SEEK_SET);
	
	// (escrever uma função pra fazer inserção ordenada.)
	
	printf("Realizando inserção ordenada no índice...\n");
	while(fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products)){
		insercao_ordenada(indice, line_product_aux->product_id, line_product_aux->indice, registros);
		i++;
	}
	fclose(file_products);
	
	printf("Índice criado na memória.\n");
	
	return indice;
}


