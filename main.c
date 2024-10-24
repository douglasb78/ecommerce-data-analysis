#include <stdio.h>
#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "btree.h"
#include "parser.h"
#include "utils.h"

#define pause() getch()

void insert_value(BTree *btree, LineData *line_data){
	return;
}

void process_data(char filename[]){
	int aux = 0;
	char char_aux = '\0';
	char string_aux[256];
	memset(string_aux, '\0', sizeof(string_aux));
	FILE *file_data = fopen(filename, "r");
	if(file_data == NULL){
		printf("Erro ao ler o conjunto de dados. Coloque-o na mesma pasta do programa!\n");
		return;
	}
	FILE *file_products = fopen("arquivo_produtos.bin", "wb");
	FILE *file_access = fopen("arquivo_acessos.bin", "wb");
	
	while((char_aux = fgetc(file_data)) != EOF){
		string_aux[aux] = char_aux;
		aux += 1;
		if(char_aux == '\n'){  // (não se esquecer de ler + 1 vez no final do arquivo)
			LineData *line_data = read_line(string_aux);
			LineProduct *line_product = (LineProduct*)malloc(sizeof(struct line_product));
			LineAccess *line_access = (LineAccess*)malloc(sizeof(struct line_access));
			memset(string_aux, '\0', sizeof(string_aux));
			aux = 0;
			if(strcmp(line_data->event_time, "event_time")==0) continue;
			// produto
			line_product->product_id = line_data->product_id;
			line_product->category_id = line_data->category_id;
			memcpy(line_product->category_code, line_data->category_code, sizeof(line_data->category_code));
			memcpy(line_product->brand, line_data->brand, sizeof(line_data->brand));
			line_product->price = line_data->price;
			line_product->mandatory_newline = '\n';
			
			// acesso
			line_access->event_timestamp = generate_timestamp(line_data->event_time);
			line_access->user_id = line_data->user_id;
			memcpy(line_access->event_type, line_data->event_type, sizeof(line_data->event_type));
			line_access->product_id = line_data->product_id;
			memcpy(line_access->user_session, line_data->user_session, sizeof(line_data->user_session));
			line_access->mandatory_newline = '\n';
			
			fwrite(line_product, sizeof(struct line_product), 1, file_products);
			fwrite(line_access, sizeof(struct line_access), 1, file_access);
			free(line_data);
			free(line_access);
			free(line_product);
		}
	}
	fclose(file_products);
	fclose(file_access);
	
}

int main(){
	LineData *teste1, *teste2;
	// event_time,event_type,product_id,category_id,category_code,brand,price,user_id,user_session
	teste1 = read_line("2019-11-01 00:00:00 UTC,view,1003461,2053013555631882655,electronics.smartphone,xiaomi,489.07,520088904,4d3b30da-a5e4-49df-b1a8-ba5943f1dd33\n");
	teste2 = read_line("2019-11-01 00:00:04 UTC,view,34600011,2060981320581906480,,,20.54,512416379,4dfe2c67-e537-4dc2-ae69-0b65263db091\n");
	process_data("2019-Nov.csv");
	
	print_data(teste1);
	print_data(teste2);
	return 0;
}

