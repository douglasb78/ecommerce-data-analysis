#include <stdio.h>
#include <ctype.h> // isdigit()
#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include "utils.h" // generate_timestamp()
#include "parser.h"

void print_data(LineData *data){
	printf("\"%s\" | \"%s\" | \"%lu\" | \"%llu\" | \"%s\" |  \"%s\" | \"%llu\" | \"%lu\" |  \"%s\" \n",
		data->event_time, data->event_type, data->product_id, data->category_id, data->category_code, data->brand, data->price, data->user_id, data->user_session);
	return;
}

// entradas nos arquivos do kaggle:
// Nov 67501979
// Oct 42448765 // total: 109950744

// criar link simbólico:
// powershell New-Item -Path 'DESTINO\2019-Nov.csv' -ItemType 'SymbolicLink' -Value "D:\ecommerce-data-analysis\2019-Nov.csv"
// powershell New-Item -Path 'DESTINO\2019-Oct.csv' -ItemType 'SymbolicLink' -Value "D:\ecommerce-data-analysis\2019-Oct.csv"

// ler poucas linhas por vez:
// powershell Get-Content ARQUIVO -Head QUANTIDADE_LINHAS
// powershell Get-Content 2019-Nov.csv -Head 20
// powershell Get-Content 2019-Oct.csv -Head 20

// ler linha específica:
// powershell (Get-Content 2019-Nov.csv)[N - 1]
// 80.000 = 2019-11-01 03:29:46 UTC,view,4700727,2053013560899928785,auto.accessories.videoregister,navitel,51.46,543578026,9eb059dc-3fdc-467f-a723-af6330893212

// criar versões menores do arquivo, para fins de teste do programa:
// powershell Get-Content 2019-Nov.csv -Head 10000 | Set-Content 2019-Nov-small.csv
// powershell Get-Content 2019-Oct.csv -Head 10000 | Set-Content 2019-Oct-small.csv

// converter carriage return
// $FilePath = "C:\Test\File.txt"
// (Get-Content -Raw -Path $FilePath) -replace '\n','\r\n' | Set-Content -Path $FilePath
// (Get-Content -Raw -Path $FilePath) -replace '\r\n','\n' | Set-Content -Path $FilePath


// event_time,event_type,product_id,category_id,category_code,brand,price,user_id,user_session
// 2019-11-01 00:00:00 UTC,view,1003461,2053013555631882655,electronics.smartphone,xiaomi,489.07,520088904,4d3b30da-a5e4-49df-b1a8-ba5943f1dd33
// 2019-11-01 00:00:03 UTC,view,2701517,2053013563911439225,appliances.kitchen.refrigerators,,155.11,518427361,c89b0d96-247f-4044-9c91-bb5f38c6af9b

LineData* read_line(char string[]){
	LineData *data = malloc(sizeof(struct line_data));
	char charAux = '\0', strAux[2] = {'\0', '\n'};
	int i = 0, parameter = 0, char_count = 0, flag_price = 0;
	
	// inicializar:
	memset(data->event_time, '\0', sizeof(data->event_time));
	memset(data->event_type, '\0', sizeof(data->event_type));
	data->product_id = 0;
	data->category_id = 0;
	memset(data->category_code, '\0', sizeof(data->category_code));
	memset(data->brand, '\0', sizeof(data->brand));
	data->price = 0;
	data->user_id = 0;
	memset(data->user_session, '\0', sizeof(data->user_session));
	
	// ler linha:
	while(charAux != '\n'){
		if(i!=0 && charAux != ','){
			switch(parameter){
				case 0: // event_time, char[32]
					data->event_time[char_count] = charAux;
					break;
				case 1: // event_type, char[32]
					data->event_type[char_count] = charAux;
					break;
				case 2: // product_id, unsigned long int
					strAux[0] = charAux;
					if(isdigit(charAux)){
						data->product_id *= 10;
						data->product_id += atoi(strAux);
					}
					break;
				case 3: // category_id, unsigned long long int
					strAux[0] = charAux;
					if(isdigit(charAux)){
						data->category_id *= 10;
						data->category_id += atoi(strAux);
					}
					break;
				case 4: // category_code, char[64]
					data->category_code[char_count] = charAux;
					break;
				case 5: // brand, char[64]
					data->brand[char_count] = charAux;
					break;
				case 6: // price, unsigned long long int
					strAux[0] = charAux;
					if(isdigit(charAux)){
						data->price *= 10;
						data->price += atoi(strAux);
					}
					if(charAux == '.') flag_price = 1;
					break;
				case 7: // user_id, unsigned long int
					strAux[0] = charAux;
					if(isdigit(charAux)){
						data->user_id *= 10;
						data->user_id += atoi(strAux);
					}
					break;
				case 8: // user_session, char[37]
					if(char_count<36) data->user_session[char_count] = charAux;
					break;
				default:
					break;
			}
			char_count += 1;
		} // : if caractere válido
		if(charAux == ','){
			char_count = 0;
			parameter += 1;
		}
		charAux = string[i];
		i++;
	} // : while ler linha
	// salvaguardas:
	// se a linha não tiver os centavos, multiplicar por 100:
	if(flag_price == 0){
		data->price *= 100;
	}
	// se strings estiverem vazias, pôr "NOT_SPECIFIED":
	// event_time, event_type, category_code, brand, user_session
	if(data->event_time[0] == '\0') memcpy(data->event_time, "NOT_SPECIFIED", sizeof(char)*13);
	if(data->event_type[0] == '\0') memcpy(data->event_type, "NOT_SPECIFIED", sizeof(char)*13);
	if(data->category_code[0] == '\0') memcpy(data->category_code, "NOT_SPECIFIED", sizeof(char)*13);
	if(data->brand[0] == '\0') memcpy(data->brand, "NOT_SPECIFIED", sizeof(char)*13);
	if(data->user_session[0] == '\0') memcpy(data->user_session, "NOT_SPECIFIED", sizeof(char)*13);
	return data;
}

unsigned long long int write_data_to_binary_files(char filename[], int count){
	int aux = 0;
	unsigned long long int indice = 0;
	char char_aux = '\0';
	char string_aux[512];
	memset(string_aux, '\0', sizeof(string_aux));
	FILE *file_data = fopen(filename, "r");
	if(file_data == NULL){
		printf("Erro ao ler o conjunto de dados. Coloque-o na mesma pasta do programa!\n");
		return -1;
	}
	// Arquivo de produtos:
	FILE *file_products = fopen("arquivo_produtos.bin", "r+b");
	if(file_products == NULL){
		printf("Criando arquivo binário de produtos...\n");
		file_products = fopen("arquivo_produtos.bin", "wb");
		if(file_products == NULL){
			printf("Erro ao criar o arquivo binário de produtos.\nDelete o arquivo (se ele já existe) e coloque o programa em uma pasta que não seja a \"Downloads\"! (UAC)\n");
			return -1;
		}
	} else {
		LineProduct *line_product_aux = (LineProduct*)malloc(sizeof(struct line_product));
		//fseek(file_products, (long)sizeof(struct line_product) * -1L, SEEK_END);
		//fread(line_product_aux, (long)sizeof(struct line_product), 1L, file_products);
		//indice = line_product_aux->indice + 2L;
		while(fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products)){
			;
		}
		indice = line_product_aux->indice + 1L;
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
			return -1;
		}
	}
	// Construir arquivos binários, não ordenados:
	while((char_aux = fgetc(file_data)) != EOF && (count > 0 || count == -1)){
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
			line_access->indice = indice; 
			line_access->mandatory_newline = '\n';
			
			fwrite(line_product, sizeof(struct line_product), 1, file_products);
			fwrite(line_access, sizeof(struct line_access), 1, file_access);
			free(line_data);
			free(line_access);
			free(line_product);
			indice += 1;
			if(count > 0)count--;
		}
	}
	fclose(file_products);
	fclose(file_access);
	fclose(file_data);
	return indice;
}

void testar_linhas_produto(unsigned long long int limiar){
	FILE *file_products = fopen("arquivo_produtos.bin", "rb");
	LineProduct *line = (LineProduct*)malloc(sizeof(struct line_product));
	while(fread(line, (long)sizeof(struct line_product), 1, file_products)){
		if(line->indice > limiar)
			printf("%lu %llu %llu %s %s %llu\n", line->product_id, line->indice, line->category_id, line->category_code, line->brand, line->price);
	}
	fclose(file_products);
	return;
}
