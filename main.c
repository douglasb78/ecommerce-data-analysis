#include <stdio.h>
#include <locale.h>
#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "parser.h"
#include "utils.h"

#define pause() getch()

void testar_linhas_produto(char filename[]){
	FILE *file_products = fopen("arquivo_produtos.bin", "rb");
	LineProduct *line = (LineProduct*)malloc(sizeof(struct line_product));
	while(fread(line, (long)sizeof(struct line_product), 1, file_products)){
		printf("%lu %llu %llu %s %s %llu\n", line->product_id, line->indice, line->category_id, line->category_code, line->brand, line->price);
	}
	fclose(file_products);
	return;
}

int write_data_to_binary_files(char filename[]){
	int aux = 0;
	unsigned long long int indice = 0;
	char char_aux = '\0';
	char string_aux[256];
	memset(string_aux, '\0', sizeof(string_aux));
	FILE *file_data = fopen(filename, "r");
	if(file_data == NULL){
		printf("Erro ao ler o conjunto de dados. Coloque-o na mesma pasta do programa!\n");
		return;
	}
	// Arquivo de produtos:
	FILE *file_products = fopen("arquivo_produtos.bin", "r+b");
	if(file_products == NULL){
		printf("Criando arquivo binário de produtos...\n");
		file_products = fopen("arquivo_produtos.bin", "wb");
		if(file_products == NULL){
			printf("Erro ao criar o arquivo binário de produtos.\nDelete o arquivo (se ele já existe) e coloque o programa em uma pasta que não seja a \"Downloads\"! (UAC)\n");
			return;
		}
	} else {
		LineProduct *line_product_aux = (LineProduct*)malloc(sizeof(struct line_product));
		fseek(file_products, (long)sizeof(struct line_product) * -1, SEEK_END);
		fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products);
		indice = line_product_aux->indice + 1;
		free(line_product_aux);
		printf("Acrescentando ao arquivo binário de produtos...\n");
		fseek(file_products, 0, SEEK_END);
	}
	
	// Arquivo de acessos:
	FILE *file_access = fopen("arquivo_acessos.bin", "r+b");
	if(file_access == NULL){
		printf("Criando arquivo binário de acessos...\n");
		file_access = fopen("arquivo_acessos.bin", "wb");
	} else {
		fseek(file_access, 0, SEEK_END);
		printf("Acrescentando ao arquivo binário de acessos...\n");
		if(file_access == NULL){
			printf("Erro ao criar o arquivo binário de acessos.\nDelete o arquivo (se ele já existe) e coloque o programa em uma pasta que não seja a \"Downloads\"! (UAC)\n");
			return;
		}
	}
	
	// Construir arquivos binários, não ordenados:
	while((char_aux = fgetc(file_data)) != EOF){
		string_aux[aux] = char_aux;
		aux += 1;
		if(char_aux == '\n' || char_aux == EOF){
			LineData *line_data = read_line(string_aux);
			LineProduct *line_product = (LineProduct*)malloc(sizeof(struct line_product));
			LineAccess *line_access = (LineAccess*)malloc(sizeof(struct line_access));
			memset(string_aux, '\0', sizeof(string_aux));
			aux = 0;
			if(strcmp(line_data->event_time, "event_time")==0) continue;
			// produto
			line_product->product_id = line_data->product_id;
			line_product->indice = indice;
			line_product->category_id = line_data->category_id;
			memcpy(line_product->category_code, line_data->category_code, sizeof(line_data->category_code));
			memcpy(line_product->brand, line_data->brand, sizeof(line_data->brand));
			line_product->price = line_data->price;
			line_product->removed = 0;
			line_product->mandatory_newline = '\n';
			
			// acesso
			line_access->event_timestamp = generate_timestamp(line_data->event_time);
			line_access->user_id = line_data->user_id;
			memcpy(line_access->event_type, line_data->event_type, sizeof(line_data->event_type));
			line_access->product_id = line_data->product_id;
			memcpy(line_access->user_session, line_data->user_session, sizeof(line_data->user_session));
			line_access->removed = 0;
			line_access->mandatory_newline = '\n';
			
			fwrite(line_product, sizeof(struct line_product), 1, file_products);
			fwrite(line_access, sizeof(struct line_access), 1, file_access);
			free(line_data);
			free(line_access);
			free(line_product);
			indice += 1;
		}
	}
	fclose(file_products);
	fclose(file_access);
	return indice;
}

int main(){
	unsigned long long int linhas_escritas;
	setlocale(LC_ALL, "Portuguese");
	write_data_to_binary_files("2019-Nov-red.csv");
	linhas_escritas = testar_linhas_produto("arquivo_produtos.bin");
	return 0;
}

