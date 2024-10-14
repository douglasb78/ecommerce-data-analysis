#include <stdio.h>
#include <ctype.h> // isdigit()
#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include "btree.h"

void print_data(LineData *data){
	printf("\"%s\" | \"%s\" | \"%lu\" | \"%llu\" | \"%s\" |  \"%s\" | \"%llu\" | \"%lu\" |  \"%s\" \n",
		data->event_time, data->event_type, data->product_id, data->category_id, data->category_code, data->brand, data->price, data->user_id, data->user_session);
	return;
}

// criar link simbólico:
// powershell New-Item -Path 'C:\Users\douglasb\OneDrive\Documents\eCommerce\2019-Nov.csv' -ItemType 'SymbolicLink' -Value "D:\ecommerce-data-analysis\2019-Nov.csv"
// powershell New-Item -Path 'C:\Users\douglasb\OneDrive\Documents\eCommerce\2019-Oct.csv' -ItemType 'SymbolicLink' -Value "D:\ecommerce-data-analysis\2019-Oct.csv"

// ler poucas linhas por vez:
// powershell Get-Content ARQUIVO -Head QUANTIDADE_LINHAS
// powershell Get-Content 2019-Nov.csv -Head 20
// powershell Get-Content 2019-Oct.csv -Head 20

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
					data->user_session[char_count] = charAux;
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
