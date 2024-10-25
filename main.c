#include <stdio.h>
#include <locale.h> // acentua��o
#include <stdlib.h> // malloc() e system()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "utils.h" // generate_timestamp()
#include "parser.h" // leitura do arquivo
#include "index.h" // arquivo de �ndice
#define clear() system("@cls||clear") // limpa tela

#define pause() getch()

int main(){
	setlocale(LC_ALL, "Portuguese");
	char entrada[3];
	char nome_arquivo[128];
	char data_inicio[32], data_final[32];
	char event_timestamp_str_aux[128], user_id_str_aux[128];
	long long int event_timestamp_aux = 0; unsigned long int user_id_aux = 0;
	long long int timestamp_inicio = 0, timestamp_fim = 0;
	int entrada_numero = -1;
	unsigned long long int registros = 0;
	unsigned long long int registros_produtos = 0;
	unsigned long long int registros_acessos = 0;
	RegistroIndiceProduto *indice_produtos = NULL;
	RegistroIndiceAcesso *indice_acessos = NULL; 

	do {
        clear();
        printf("Menu de op��es:\n\n");
        printf(" 1. Ler arquivo .csv\n");
        printf(" 2. Criar �ndice de acessos e produtos, e salv�-los em arquivos\n");
        printf(" 3. Carregar �ndice de acessos e produtos do arquivo para mem�ria\n");
        printf(" 4. Mostrar �ndice de produtos\n");
        printf(" 5. Mostrar �ndice de acessos\n");
        printf(" 6. Mostrar acessos em um dado intervalo de tempo\n");
        printf(" 7. Mostrar marcas mais vendidas\n");
        printf(" 8. Remover evento de acesso do arquivo bin�rio [em progresso]\n");
        printf(" 9. Remover produto do arquivo bin�rio (e seus acessos)  [em progresso]\n");
        printf("10. Reconstruir arquivos bin�rios  [em progresso]\n");
        printf("11. Testar arquivos bin�rio de produtos\n");
        printf("\nX. Sair\n\n");
        printf("Escolha uma op��o: ");

        scanf("%2s%*c", entrada);
        entrada_numero = -1;
        entrada_numero = atoi(entrada);
		switch (entrada_numero) {
			case 1:
				// ler arquivo .csv
				registros = 0;
				memset(nome_arquivo, '\0', sizeof(char) * 64);
				printf("Digite o nome do arquivo .csv a ser lido:\n> ");
				if(fgets(nome_arquivo, 128, stdin)){
					nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
				}
				printf("Arquivo: \"%s\"\n", nome_arquivo);
				pause();
				registros += write_data_to_binary_files(nome_arquivo, -1);
				registros_acessos += registros;
				registros_produtos += registros;
				break;
			case 2:
				// criar e salvar �ndice
				printf("Criando �ndice de produtos...\n");
				indice_produtos = criar_indice_produtos("arquivo_produtos.bin", &registros_produtos);
				printf("Criando �ndice de acessos...\n");
				indice_acessos = criar_indice_acessos("arquivo_acessos.bin", &registros_acessos);
				printf("Salvando �ndices para arquivo...\n");
				salvar_indice_produtos(indice_produtos, registros_produtos, "indice_produtos.bin");
				salvar_indice_acessos(indice_acessos, registros_acessos, "indice_acessos.bin");
				break;
			case 3:
				// carregar �ndice para a mem�ria
				printf("Carregando �ndices para a mem�ria...\n");
				indice_acessos = carregar_indice_acessos("indice_acessos.bin", &registros_acessos);
				indice_produtos = carregar_indice_produtos("indice_produtos.bin", &registros_produtos);
				break;
			case 4:
				// mostrar �ndice de produtos, j� carregado na mem�ria:
				if(indice_produtos != NULL){
					mostrar_indice_produtos(indice_produtos, registros_produtos);
				} else {
					printf("O �ndice de produtos ainda n�o foi gerado.\n");
				}
				break;
			case 5:
				// mostrar �ndice de acessos, j� carregado na mem�ria:
				if(indice_acessos != NULL){
					mostrar_indice_acessos(indice_acessos, registros_acessos);
				} else {
					printf("O �ndice de acessos ainda n�o foi gerado.\n");
				}
				break;
			case 6:
				// mostrar acessos em um intervalo de tempo:
				memset(data_inicio, '\0', sizeof(char) * 32);
				memset(data_final, '\0', sizeof(char) * 32);
				timestamp_inicio = 0;
				timestamp_fim = 0;
				printf("(Ainda n�o coloquei checagem de erros. Digitar corretamente.)\n");
				printf("Digite a data inicial: (Exemplo: \"2019-11-01 00:15:05 UTC\")\n> ");
				if(fgets(data_inicio, 32, stdin)){
					data_inicio[strcspn(data_inicio, "\n")] = 0;
				}
				timestamp_inicio = generate_timestamp(data_inicio);
				clear();
				printf("(Ainda n�o coloquei checagem de erros. Digitar corretamente.)\n");
				printf("Digite a data final: (Exemplo: \"2019-11-01 00:15:15 UTC\")\n> ");
				if(fgets(data_final, 32, stdin)){
					data_final[strcspn(data_final, "\n")] = 0;
				}
				timestamp_fim = generate_timestamp(data_final);
				mostrar_acessos_arquivo_intervalo(timestamp_inicio, timestamp_fim);
				break;
			case 7:
				// mostrar as marcas mais compradas:
				printf("Mostrando marcas mais compradas...\n");
				mostrar_marcas_mais_compradas();
				break;
			case 8:
				// remover linha de acesso do arquivo bin�rio
				
				// perguntar timestamp e id
				
				/*
				printf("(Ainda n�o coloquei checagem de erros. Digitar corretamente.)\n")
				printf("Digite o timestamp do acesso a ser removido:\n> ");
				if(fgets(event_timestamp_str_aux, 128, stdin)){
					event_timestamp_str_aux[strcspn(event_timestamp_str_aux, "\n")] = 0;
				}
				clear();
				printf("Digite o id de usu�rio do acesso a ser removido:\n> ");
				if(fgets(user_id_str_aux, 128, stdin)){
					user_id_str_aux[strcspn(user_id_str_aux, "\n")] = 0;
				}
				
				*/
				//FILE *arquivo_acesso = fopen("arquivo_acessos.bin", "rb");
				//fseek(arquivo_acesso, 0, SEEK_SET);
				printf("N�o implementado.\n");
				break;
			case 9:
				printf("N�o implementado.\n");
				// remover linha de produto do arquivo bin�rio .. e todas linhas de acesso que t�m o id dele
				break;
			case 10:
				printf("N�o implementado.\n");
				break;
			case 11:
				testar_linhas_produto();
				break;
			default:
				if(entrada[0] == 'X' || entrada[0] == 'x'){
					printf("Saindo.\n");
					break;
				} else {
					printf("Op��o inv�lida! Tente novamente.\n");
					break;	
				}
		}
	
		printf("Pressione Enter para continuar.\n");
		pause();
	} while (entrada[0] != 'x' && entrada[0] != 'X');
	return 0;
}


//testar_linhas_produto("arquivo_produtos.bin");
