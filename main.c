#include <stdio.h>
#include <locale.h>
#include <stdlib.h> // malloc()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "parser.h"
#include "utils.h" // generate_timestamp()
#include "index.h"

#define pause() getch()

int main(){
	setlocale(LC_ALL, "Portuguese");
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
	mostrar_acessos_arquivo_intervalo(1569888001, 1569888010);
	mostrar_marcas_mais_compradas();
	return 0;
}


//testar_linhas_produto("arquivo_produtos.bin");
