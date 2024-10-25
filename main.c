#include <stdio.h>
#include <locale.h> // acentuação
#include <stdlib.h> // malloc() e system()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "utils.h" // generate_timestamp()
#include "parser.h" // leitura do arquivo
#include "index.h" // arquivo de índice
#define clear() system("@cls||clear") // limpa tela

#define pause() getch()

int main(){
	setlocale(LC_ALL, "Portuguese");
	char entrada[3];
	unsigned long long int registros = 0;
	unsigned long long int registros2 = 0;
	
	//registros += write_data_to_binary_files("2019-Nov-small.csv", -1);
	//registros += write_data_to_binary_files("2019-Oct-small.csv", -1);
	
	//printf("%llu registros.\n", registros);
	
	//RegistroIndiceProduto *indice = criar_indice_produtos("arquivo_produtos.bin", &registros);
	//RegistroIndiceAcesso *indice2 = criar_indice_acessos("arquivo_acessos.bin", &registros2);
	
	//RegistroIndiceProduto *indice = carregar_indice_produtos("indice_produtos.bin", &registros);
	//mostrar_indice_produtos(indice, registros);
	//salvar_indice_produtos(indice, registros, "indice_produtos.bin");
	//salvar_indice_acessos(indice2, registros2, "indice_acessos.bin");
	//mostrar_indice_produtos_arquivo("indice_produtos.bin");
	//mostrar_indice_acessos_arquivo("indice_acessos.bin");
	
	//mostrar_acessos_arquivo_intervalo(1569888001, 1569888010);
	//mostrar_marcas_mais_compradas();
	do {
        clear();
        printf("Menu de opções:\n\n");
        printf("1. Ler arquivo .csv\n");
        printf("2. Criar índice de acessos e produtos, e salvá-los em arquivos\n");
        printf("3. Carregar índice de acessos e produtos do arquivo para memória\n");
        printf("4. Mostrar índice de produtos\n");
        printf("5. Mostrar índice de acessos\n");
        printf("6. Mostrar acessos em um dado intervalo de tempo\n");
        printf("7. Mostrar marcas mais vendidas\n");
        printf("0. Sair\n\n");
        printf("Escolha uma opção: ");

        scanf("%2s%*c", entrada);

		switch (entrada[0]) {
			case '1':
				// ler arquivo .csv
				break;
			case '2':
				// criar e salvar índice
				break;
			case '3':
				// carregar índice para a memória
				break;
			case '4':
				// mostrar índice de produtos
				break;
			case '5':
				// mostrar índice de acessos
				break;
			case '6':
				// mostrar acessos no intervalo
				break;
			case '7':
				// mostrar marcas mais vendidas
				break;
			case '0':
				printf("Saindo.\n");
				break;
			default:
				printf("Opção inválida! Tente novamente.\n");
				break;
		}
	
		printf("Pressione Enter para continuar.");
		pause();
	} while (entrada[0] != '0');
	return 0;
}


//testar_linhas_produto("arquivo_produtos.bin");
