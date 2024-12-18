#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "utils.h"

// o timestamp � usado de chave no �ndice de acessos.
// (n�o tem strptime() no time.h do GCC)
// 2019-11-01 00:15:05 UTC -> 1572578105
int is_date_string_valid(char date_string[]){
	for(int i=0; i<strlen(date_string); i++){
		// ano:
		if(i >= 0 && i <= 3){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i==4 && date_string[i] != '-') return 0;
		// m�s
		if(i >= 5 && i <= 6){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i==7 && date_string[i] != '-') return 0;
		// dia
		if(i >= 8 && i <= 9){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i==10 && date_string[i] != ' ') return 0;
		// hora:
		if(i >= 11 && i <= 12){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i==13 && date_string[i] != ':') return 0;
		// minuto:
		if(i >= 14 && i <= 15){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i==16 && date_string[i] != ':') return 0;
		// segundo:
		if(i >= 17 && i <= 18){
			if(date_string[i] < '0' || date_string[i] > '9') return 0;
		}
		if(i>18) break;
	}
	return 1;
}

long long int generate_timestamp(char date_string[]){
	long long int ts = 0;
	int i=0, count=0; char aux = 'A'; char aux_str[2] = {'\0', '\0'};
	int year = 0, month = 0, day = 0;
	int hour = 0, minute = 0, second = 0;
	int *variable_ptr = NULL;
	while(aux != '\n' && i<strlen(date_string)){
		aux = date_string[i];
		if(aux == ' ' || aux == '-' || aux == ':') count++;
		else {
			switch(count){
				case 0:
					variable_ptr = &year;
					break;
				case 1:
					variable_ptr = &month;
					break;
				case 2:
					variable_ptr = &day;
					break;
				case 3:
					variable_ptr = &hour;
					break;
				case 4:
					variable_ptr = &minute;
					break;
				case 5:
					variable_ptr = &second;
					break;
				default:
					aux = '\n';
					break;
			}
			if(variable_ptr != NULL && aux >= '0' && aux <= '9'){
				aux_str[0] = aux;
				*variable_ptr *= 10;
				*variable_ptr += atoi(aux_str);	
			}
		}
		i++;
	}
	// ano, com corre��o de ano bissexto:
	ts += (year - 1970) * 31536000;
	for(int j=1970; j<year; j++){
		if((j % 4 == 0) && (j % 100 != 0)){
			ts += 86400;
		}
	}
	
	// m�s:
	month--;
	if(month > 0) ts += 2678400; // janeiro
	if(month > 1){ // fevereiro
		 if((year %4 ==0) && (year % 100 !=0)){
		 	ts += 2505600;
		 } else {
		 	ts += 2419200; 
		 }
	}
	if(month > 2) ts += 2678400; // mar�o
	if(month > 3) ts += 2592000; // abril
	if(month > 4) ts += 2678400; // maio
	if(month > 5) ts += 2592000; // junho
	if(month > 6) ts += 2678400; // julho
	if(month > 7) ts += 2678400; // agosto
	if(month > 8) ts += 2592000; // setembro
	if(month > 9) ts += 2678400; // outubro
	if(month > 10) ts += 2592000; // novembro
	if(month > 11) ts += 2678400; // dezembro
	
	// dia, hora, minuto e segundo:
	ts += (day) * 86400;
	ts += hour * 3600;
	ts += minute * 60;
	ts += second;

	return ts;
}

// Fun��o de hashing que encontrei com boa distribui��o e efeito avalanche:
// hash: https://www.programmingalgorithms.com/algorithm/sdbm-hash/c/
// hash: https://theartincode.stanis.me/008-djb2/
unsigned int hashSDBM(char* str, unsigned int length) {
	unsigned int hash = 0;
	unsigned int i = 0;

	for (i = 0; i < length; str++, i++)
	{
		hash = (*str) + (hash << 6) + (hash << 16) - hash;
	}

	return hash;
}

double get_time() {
    LARGE_INTEGER t, f;
    QueryPerformanceCounter(&t);
    QueryPerformanceFrequency(&f);
    return (double)t.QuadPart/(double)f.QuadPart;
}

