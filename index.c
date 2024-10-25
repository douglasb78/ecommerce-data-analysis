#include <stdio.h>
#include <string.h> // memmove()
#include <stdlib.h> // malloc()
#include "index.h"
#include "parser.h"

/* =======================================================
	Mostrar índice acessos e índice produtos na memória:
   ======================================================= */

void mostrar_indice_acessos(RegistroIndiceAcesso *indice, unsigned long long int registros){
	printf("\n===\n");
	for(unsigned long long int i=0; i<registros; i++){
		printf("%lu - %llu\n", indice[i].event_timestamp, indice[i].indice);
	}
	printf("===\n\n");
}

void mostrar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros){
	printf("\n===\n");
	for(unsigned long long int i=0; i<registros; i++){
		printf("%lu - %llu\n", indice[i].product_id, indice[i].indice);
	}
	printf("===\n\n");
}

/* =======================================================
	Mostrar índice acessos e índice produtos no arquivo:
   ======================================================= */

void mostrar_indice_acessos_arquivo(char filename[]){
	FILE *file_indice_access = fopen("indice_acessos.bin", "rb");
	if(file_indice_access == NULL){
		printf("Erro ao ler o índice binário de acessos.\nCertifique-se dele ter sido criado!");
		return;
	}
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	RegistroIndiceAcesso *registro = (RegistroIndiceAcesso*)malloc(sizeof(struct registro_indice_acesso));
	fread(header, sizeof(struct header_indice), 1, file_indice_access);
	printf("Há %llu registros no arquivo.\n", header->registros);
	printf("Lendo registros do arquivo do índice de acessos...\n");
	while(fread(registro, (long)sizeof(struct registro_indice_acesso), 1, file_indice_access)){
		if(registro->indice != -1)
			printf("%lu %llu\n", registro->event_timestamp, registro->indice);
	}
	fclose(file_indice_access);
	free(header);
	free(registro);
	return;
}

void mostrar_indice_produtos_arquivo(char filename[]){
	FILE *file_indice_products = fopen("indice_produtos.bin", "rb");
	if(file_indice_products == NULL){
		printf("Erro ao ler o índice binário de produtos.\nCertifique-se dele ter sido criado!");
		return;
	}
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	RegistroIndiceProduto *registro = (RegistroIndiceProduto*)malloc(sizeof(struct registro_indice_produto));
	fread(header, sizeof(struct header_indice), 1, file_indice_products);
	printf("Há %llu registros no arquivo.\n", header->registros);
	printf("Lendo registros do arquivo do índice de produtos...\n");
	while(fread(registro, (long)sizeof(struct registro_indice_produto), 1, file_indice_products)){
		if(registro->indice != -1)
			printf("%lu %llu\n", registro->product_id, registro->indice);
	}
	fclose(file_indice_products);
	free(header);
	free(registro);
	return;
}

/* =======================================================
	Carregar os índices do arquivo para a memória:
   ======================================================= */
RegistroIndiceAcesso* carregar_indice_acessos(char filename[], unsigned long long int *registros){
	unsigned long long int i = 0;
	FILE *file_indice_access = fopen("indice_acessos.bin", "rb");
	if(file_indice_access == NULL){
		printf("Erro ao ler o índice binário de acessos.\nCertifique-se dele ter sido criado!");
		return NULL;
	}
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	fread(header, sizeof(struct header_indice), 1, file_indice_access);
	*registros = header->registros;
	
	RegistroIndiceAcesso *registro = (RegistroIndiceAcesso*)malloc(sizeof(struct registro_indice_acesso));
	RegistroIndiceAcesso *indice = (RegistroIndiceAcesso*)calloc(header->registros+1, (long)sizeof(struct registro_indice_acesso));
	
	while(fread(registro, (long)sizeof(struct registro_indice_acesso), 1, file_indice_access)){
		indice[i].event_timestamp = registro->event_timestamp;
		indice[i].indice = registro->indice;
		i++;
	}
	free(header);
	free(registro);
	fclose(file_indice_access);
	return indice;
}


RegistroIndiceProduto* carregar_indice_produtos(char filename[], unsigned long long int *registros){
	unsigned long long int i = 0;
	FILE *file_indice_products = fopen("indice_produtos.bin", "rb");
	if(file_indice_products == NULL){
		printf("Erro ao ler o índice binário de produtos.\nCertifique-se dele ter sido criado!");
		return NULL;
	}
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	fread(header, sizeof(struct header_indice), 1, file_indice_products);
	*registros = header->registros;
	
	RegistroIndiceProduto *registro = (RegistroIndiceProduto*)malloc(sizeof(struct registro_indice_produto));
	RegistroIndiceProduto *indice = (RegistroIndiceProduto*)calloc(header->registros+1, (long)sizeof(struct registro_indice_produto));
	
	while(fread(registro, (long)sizeof(struct registro_indice_produto), 1, file_indice_products)){
		indice[i].indice = registro->indice;
		indice[i].product_id = registro->product_id;
		i++;
	}
	free(header);
	free(registro);
	fclose(file_indice_products);
	return indice;
}

/* =======================================================
	Salvar os índices da memória para o arquivo:
   ======================================================= */

void salvar_indice_acessos(RegistroIndiceAcesso *indice, unsigned long long int registros, char filename[]){
	FILE *file_indice_access = fopen("indice_acessos.bin", "wb");
	if(file_indice_access == NULL){
		printf("Erro ao criar o índice binário de acessos.\nDelete o arquivo (se ele já existe) e coloque o programa em uma pasta que não seja a \"Downloads\"! (UAC)\n");
		return;
	}
	// escrever o "cabeçalho", que indica quantos elementos tem no índice:
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	header->registros = registros;
	fwrite(header, sizeof(struct header_indice), 1, file_indice_access);
	// escrever os registros do índice:
	fwrite(indice, (registros+1) * (long)sizeof(struct registro_indice_acesso), 1, file_indice_access);
	fclose(file_indice_access);
	free(header);
	return;
}

void salvar_indice_produtos(RegistroIndiceProduto *indice, unsigned long long int registros, char filename[]){
	FILE *file_indice_products = fopen("indice_produtos.bin", "wb");
	if(file_indice_products == NULL){
		printf("Erro ao criar o índice binário de produtos.\nDelete o arquivo (se ele já existe) e coloque o programa em uma pasta que não seja a \"Downloads\"! (UAC)\n");
		return;
	}
	// escrever o "cabeçalho", que indica quantos elementos tem no índice:
	HeaderIndice *header = (HeaderIndice*)malloc(sizeof(struct header_indice));
	header->registros = registros;
	fwrite(header, sizeof(struct header_indice), 1, file_indice_products);
	// escrever os registros do índice:
	fwrite(indice, (registros+1) * (long)sizeof(struct registro_indice_produto), 1, file_indice_products);
	fclose(file_indice_products);
	free(header);
	return;
}

/* =======================================================
	Inserir os produtos de acordo com o product_id:
   ======================================================= */

void insercao_ordenada_acessos(RegistroIndiceAcesso *indice, long long int timestamp_registro, unsigned long long int indice_registro, unsigned long long int registros){
	// se é o primeiro elemento:
	if(indice[0].indice == -1){
		//printf("a\n");
		indice[0].event_timestamp = timestamp_registro;
		indice[0].indice = indice_registro;
		return;
	} else {
		// se é antes do primeiro da lista:
		if(timestamp_registro < indice[0].event_timestamp){
			//printf("b\n");
			memcpy(indice+sizeof(struct registro_indice_acesso), indice, (registros-1) * sizeof(struct registro_indice_acesso));
			indice[0].indice = indice_registro;
			indice[0].event_timestamp = timestamp_registro;
			return;
		}  else { // se é depois do primeiro, no meio ou no fim:
			int flag = 1;
			unsigned long long int i=1, j=0;
			// loop até achar um valor maior .. ou posição vazia no final:
			while(i < registros){
				if(timestamp_registro < indice[i].event_timestamp){
					flag = 0;
					break;
				}
				if(indice[i].indice == -1) break;
				i++;
			}
			if(flag){ // inserção no fim
				//printf("c\n");
				indice[i].indice = indice_registro;
				indice[i].event_timestamp = timestamp_registro;
			} else { // inserção no meio:
				//printf("d\n");
				//pegar o último elemento:
				//for(j=1; j<registros; j++)
				//	if(indice[j].indice == -1) break;
				//j--;
				//ia fazer com memcpy, mas estava dando erros, vou só fazer do jeito mais simples para terminar:
				// deslocar:
				//printf("%lu - %llu\n", indice_registro, productid_registro);
				j = i;
				for(j=registros; j>i; j--){
					if(indice[j-1].event_timestamp == -1) continue;
					indice[j].event_timestamp = indice[j-1].event_timestamp;
					indice[j].indice = indice[j-1].indice;
				}
				indice[i].indice = indice_registro;
				indice[i].event_timestamp = timestamp_registro;
				//printf("salvou d\n");
			}
		}
	}
}


void insercao_ordenada_produtos(RegistroIndiceProduto *indice, unsigned long int productid_registro, unsigned long long int indice_registro, unsigned long long int registros){
	// se é o primeiro elemento:
	if(indice[0].indice == -1){
		//printf("a\n");
		indice[0].indice = indice_registro;
		indice[0].product_id = productid_registro;
		return;
	} else {
		// se é antes do primeiro da lista:
		if(productid_registro < indice[0].product_id){
			//printf("b\n");
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
				//printf("c\n");
				indice[i].indice = indice_registro;
				indice[i].product_id = productid_registro;
			} else { // inserção no meio:
				//printf("d\n");
				//pegar o último elemento:
				//for(j=1; j<registros; j++)
				//	if(indice[j].indice == -1) break;
				//j--;
				//ia fazer com memcpy, mas estava dando erros, vou só fazer do jeito mais simples para terminar:
				// deslocar:
				//printf("%lu - %llu\n", indice_registro, productid_registro);
				j = i;
				for(j=registros; j>i; j--){
					if(indice[j-1].product_id == -1) continue;
					indice[j].indice = indice[j-1].indice;
					indice[j].product_id = indice[j-1].product_id;
				}
				indice[i].indice = indice_registro;
				indice[i].product_id = productid_registro;
				//printf("salvou d\n");
			}
		}
	}
}

/* =======================================================
	Criar o índice de acessos e produtos:
   ======================================================= */

RegistroIndiceAcesso* criar_indice_acessos(char filename[], unsigned long long int *linhas_escritas){
	int i = 0;
	unsigned long long int registros = 0;
	FILE *file_access = fopen("arquivo_acessos.bin", "rb");
	if(file_access == NULL){
		printf("Erro ao ler o arquivo binário de acessos. Certifique-se dele ter sido gerado!\n");
		return NULL;
	}
	// Contar número de acessos:
	LineAccess *line_access_aux = (LineAccess*)malloc(sizeof(struct line_access));
	fseek(file_access, (long)sizeof(struct line_access) * -1, SEEK_END);
	fread(line_access_aux, (long)sizeof(struct line_access), 1, file_access);
	registros = line_access_aux->indice + 1; // Tem '+1'porque o índice começa em 0 (zero)
	printf("Criando índice...\n");
	printf("Há %llu registros no arquivo binário de acessos.\n", registros);
	
	// Criar índice:
	RegistroIndiceAcesso *indice = (RegistroIndiceAcesso*)calloc(registros*2+1, (long)sizeof(struct registro_indice_acesso));
	for(unsigned long long int i=0; i<registros; i++){
		indice[i].indice = -1;
	}
	
	fseek(file_access, 0, SEEK_SET);
	
	printf("Realizando inserção ordenada no índice...\n");
	while(fread(line_access_aux, (long)sizeof(struct line_access), 1, file_access)){
		insercao_ordenada_acessos(indice, line_access_aux->event_timestamp, line_access_aux->indice, registros);
		i++;
	}
	fclose(file_access);
	
	printf("Índice de acessos criado na memória.\n");
	*linhas_escritas = registros;
	return indice;
}

RegistroIndiceProduto* criar_indice_produtos(char filename[], unsigned long long int *linhas_escritas){
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
	RegistroIndiceProduto *indice = (RegistroIndiceProduto*)calloc(registros*2+1, (long)sizeof(struct registro_indice_produto));
	for(unsigned long long int i=0; i<registros; i++){
		indice[i].indice = -1;
	}
	
	fseek(file_products, 0, SEEK_SET);
	
	printf("Realizando inserção ordenada no índice...\n");
	while(fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products)){
		insercao_ordenada_produtos(indice, line_product_aux->product_id, line_product_aux->indice, registros);
		i++;
	}
	fclose(file_products);
	
	printf("Índice de produtos criado na memória.\n");
	*linhas_escritas = registros;
	return indice;
}

/* =======================================================
	Pesquisa binária, para fazer consultas:
   ======================================================= */

void mostrar_acessos_arquivo_intervalo(char filename[], long long int timestamp_start, long long int timestamp_end){
	unsigned long long int low = 0, mid = 0, high = 0;
	// abrir arquivo de índice
	FILE *file_indice_access = fopen("indice_acessos.bin", "rb");
	if(file_indice_access == NULL){
		printf("Erro ao ler o índice binário de acessos.\nCertifique-se dele ter sido criado!");
		return;
	}
	// abrir o arquivo normal:
	FILE *file_access = fopen("arquivo_acessos.bin", "rb");
	if(file_access == NULL){
		printf("Erro ao ler o arquivo binário de acessos.\nCertifique-se dele ter sido criado!");
		return;
	}
	
	RegistroIndiceAcesso *registro_acesso_aux = (RegistroIndiceAcesso*)malloc(sizeof(struct registro_indice_acesso));
	LineAccess *linha_acesso_aux = (LineAccess*)malloc(sizeof(struct line_access));
	
	
	fseek(file_indice_access, 0, SEEK_END);
	high = (ftell(file_indice_access) - - sizeof(struct header_indice)) / sizeof(struct registro_indice_acesso);
	
	while(timestamp_start != registro_acesso_aux->event_timestamp){		
		mid = low+(high-low)/2;
		fseek(file_indice_access, sizeof(struct header_indice) + mid*sizeof(struct registro_indice_acesso), SEEK_SET);
		fread(registro_acesso_aux, 1, sizeof(struct registro_indice_acesso), file_indice_access);
		//printf("teste: %lld - %llu (%llu - %llu - %llu)\n", timestamp_start, registro_acesso_aux->event_timestamp, low, mid, high);
		if(low == mid && registro_acesso_aux->event_timestamp != timestamp_start){
			printf("Não encontrou.");
			return;
		}
		if(timestamp_start > registro_acesso_aux->event_timestamp){
			low = mid;
		} else {
			high = mid;
		}
	}
	while(registro_acesso_aux->event_timestamp <= timestamp_end){
		fseek(file_indice_access, sizeof(struct header_indice) + mid * sizeof(struct registro_indice_acesso), SEEK_SET);
		fread(registro_acesso_aux, 1, sizeof(struct registro_indice_acesso), file_indice_access);
		if(registro_acesso_aux->event_timestamp > timestamp_end) break;
		printf("[Índice] timestamp: %lld - índice: %llu\n", registro_acesso_aux->event_timestamp, registro_acesso_aux->indice);
		fseek(file_access, sizeof(struct line_access) * registro_acesso_aux->indice, SEEK_SET);
		fread(linha_acesso_aux, sizeof(struct line_access), 1, file_access);
		printf("[Arquivo acesso]\n");
		printf("\ttimestamp: %lld\n\tindice: %llu\n\tuser_id: %lu\n\tevent_type: %s\n\tproduct_id: %lu\n\tuser_session: %s\n\tremoved: %d\n",
			linha_acesso_aux->event_timestamp,
			linha_acesso_aux->indice,
			linha_acesso_aux->user_id,
			linha_acesso_aux->event_type,
			linha_acesso_aux->product_id,
			linha_acesso_aux->user_session,
			linha_acesso_aux->removed
		);
		printf("\n");
		mid++;
	}
	
	fclose(file_access);
	fclose(file_indice_access);
	return;
}


