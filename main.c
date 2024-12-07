// TODO: pôr remoção no arquivo de dados e fazer a tabela de tempo, pra apresentação do trabalho.

#include <stdio.h>
#include <locale.h> // acentuação
#include <stdlib.h> // malloc() e system()
#include <string.h> // memset()
#include <conio.h> // getch()
#include "utils.h" // generate_timestamp()
#include "parser.h" // leitura do arquivo
#include "index.h" // arquivo de índice
#include "btree_products.h" // arquivo de árvores B
#include "hash_table_products.h" // tabela hash de produtos
#define clear() system("@cls||clear") // limpa tela

#define pause() getch()

int main(){
	setlocale(LC_ALL, "Portuguese");
	double start_time, end_time;
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
	unsigned long long int registros_produtos_b = 0;
	unsigned long long int registros_acessos_hash = 0;
	RegistroIndiceProduto *indice_produtos = NULL;
	RegistroIndiceAcesso *indice_acessos = NULL; 
	BTree_Prod bt_produtos = NULL;
	unsigned long int product_id_busca = -1;
	RegistroIndiceProduto *produto_aux = NULL;
	LineProduct *line_product_aux = NULL;
	LineProduct *line_product_aux2 = NULL;
	unsigned long long int category_id_aux = -1;
	char category_code_aux[64];
	char brand_aux[64];
	unsigned long long int price_aux = -1;
	TabelaHashProdutos *tabela_hash_produtos = malloc(sizeof(struct tabela_hash_produtos));;
	tabela_hash_produtos->hashes = NULL;
	tabela_hash_produtos->registros = 0;
	unsigned int hash_aux = 0;
	int sucesso = -1;
	int skip;
	unsigned long long int limiar = 0;
	unsigned long long int indice_aux;
	struct entrada_tabela_produtos *entrada_hash_remover = NULL;

	do {
        clear();
        printf("== Trabalho 1 ==\n");
        printf("Opções:\n");
        printf(" 1. Ler arquivo .csv\n");
        printf(" 2. Criar índices exaustivos de acessos e produtos, e salvá-los em arquivos.\n");
        printf(" 3. Carregar índice de acessos e produtos do arquivo para memória.\n");
        printf(" 4. Mostrar índice de produtos.\n");
        printf(" 5. Mostrar índice de acessos.\n");
        printf(" 6. Mostrar acessos em um dado intervalo de tempo.\n");
        printf(" 7. Mostrar marcas mais vendidas.\n");
        printf(" 8. Testar arquivos binário de produtos.\n"); // 67501978
        printf(" 9. Remover produto.\n");
        printf("----------------------------------------\n");
        printf("== Trabalho 2 ==\n");
        printf("Opções Árvore B:\n");
        printf("10. Construir índice de produtos na memória, com árvore B.\n");
        printf("11. Procurar produto pelo índice.\n");
        printf("12. Remover produto do índice árvore B.\n");
        printf("13. Criar um produto e adicioná-lo ao arquivo binário de dados da árvore B.\n");
        printf("\nOpções Tabela Hash:\n");
        printf("14. Construir tabela hash.\n");
        printf("15. Pesquisa na tabela hash.\n");
        printf("16. Remover produto da tabela hash.\n");
        printf("\nX. Sair\n\n");
        printf("Escolha uma opção: ");

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
				registros = write_data_to_binary_files(nome_arquivo, -1);
				printf("%llu registros.\n", registros);
				registros_acessos += registros;
				registros_produtos += registros;
				break;
			case 2:
				// criar e salvar índice
				printf("Criando índice de produtos...\n");
				start_time = get_time();
				indice_produtos = criar_indice_produtos("arquivo_produtos.bin", &registros_produtos);
				end_time = get_time();
				printf("* %lf segundos para criar o arquivo de produtos.\n", end_time-start_time);
				//
				printf("Criando índice de acessos...\n");
				start_time = get_time();
				indice_acessos = criar_indice_acessos("arquivo_acessos.bin", &registros_acessos);
				end_time = get_time();
				printf("* %lf segundos para criar o arquivo de acessos.\n", end_time-start_time);
				//
				printf("Salvando índices para arquivo...\n");
				start_time = get_time();
				salvar_indice_produtos(indice_produtos, registros_produtos, "indice_produtos.bin");
				salvar_indice_acessos(indice_acessos, registros_acessos, "indice_acessos.bin");
				end_time = get_time();
				printf("* %lf segundos para salvar ambos os arquivos em disco.\n", end_time-start_time);
				break;
			case 3:
				// carregar índice para a memória
				printf("Carregando índices para a memória...\n");
				indice_acessos = carregar_indice_acessos("indice_acessos.bin", &registros_acessos);
				indice_produtos = carregar_indice_produtos("indice_produtos.bin", &registros_produtos);
				break;
			case 4:
				// mostrar índice de produtos, já carregado na memória:
				if(indice_produtos != NULL){
					mostrar_indice_produtos(indice_produtos, registros_produtos);
				} else {
					printf("O índice de produtos ainda não foi gerado.\n");
				}
				break;
			case 5:
				// mostrar índice de acessos, já carregado na memória:
				if(indice_acessos != NULL){
					mostrar_indice_acessos(indice_acessos, registros_acessos);
				} else {
					printf("O índice de acessos ainda não foi gerado.\n");
				}
				break;
			case 6:
				// mostrar acessos em um intervalo de tempo:
				memset(data_inicio, '\0', sizeof(char) * 32);
				memset(data_final, '\0', sizeof(char) * 32);
				timestamp_inicio = 0;
				timestamp_fim = 0;
				while(is_date_string_valid(data_inicio) == 0 || data_inicio[0] == '\0'){
					clear();
					printf("Digite a data inicial: (Exemplo: \"2019-11-01 00:15:05 UTC\")\n> ");
					if(fgets(data_inicio, 32, stdin)){
						data_inicio[strcspn(data_inicio, "\n")] = 0;
					}
					if(is_date_string_valid(data_inicio) == 0){
						printf("Erro, digite uma data válida!\n");
						pause();
					}
				}
				timestamp_inicio = generate_timestamp(data_inicio);
				while(is_date_string_valid(data_final) == 0 || data_final[0] == '\0'){
					clear();
					printf("Digite a data final: (Exemplo: \"2019-11-01 00:15:15 UTC\")\n> ");
					if(fgets(data_final, 32, stdin)){
						data_final[strcspn(data_final, "\n")] = 0;
					}
					if(is_date_string_valid(data_final) == 0){
						printf("Erro, digite uma data válida!\n");
						pause();
					}
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
				printf("Digite o índice para as linhas serem exibidas: ");
				scanf("%llu", &limiar);
				testar_linhas_produto(limiar);
				break;
			case 9:
				indice_aux = 0;
				line_product_aux =	(LineProduct*)malloc(sizeof(struct line_product));
				printf("Digite o product_id para remover do arquivo de dados:\t(Exemplo: 1004856)\n"); // 1004856
				scanf("%lu", &product_id_busca);
				FILE *file_ptr = fopen("arquivo_produtos.bin", "rb+");
				if(file_ptr == NULL){
					printf("Erro ao abrir o arquivo binário de produtos. Certifique-se dele ter sido gerado!\n");
					break;
				}
				start_time = get_time();
				fseek(file_ptr, 0, SEEK_SET);
				while(fread(line_product_aux, (long)sizeof(struct line_product), 1, file_ptr)){
					printf("%s\n", line_product_aux->category_code);
					if(line_product_aux->product_id == product_id_busca) break;
				}
				indice_aux = line_product_aux->indice;
				line_product_aux->removed = 1;
				fwrite(line_product_aux, (long)sizeof(struct line_product), 1, file_ptr);
				end_time = get_time();
				fseek(file_ptr, (long)sizeof(struct line_product)*indice_aux, SEEK_SET);
				fread(line_product_aux, (long)sizeof(struct line_product), 1, file_ptr);
				//printf("%lu %d\n", line_product_aux->product_id, line_product_aux->removed);
				printf("* %lf segundos para remover o produto do arquivo binário.\n", end_time-start_time);
				fclose(file_ptr);
				break;
			case 10:
				start_time = get_time();
				bt_produtos = criar_indice_produtos_arvore_b("arquivo_produtos.bin", &registros_produtos_b);
				end_time = get_time();
				printf("* %lf segundos para criar o índice de produtos árvore B na memória.\n", end_time-start_time);
				//mostrar_arvore(bt_produtos, 0, NULL);
				break;
			case 11:
				printf("Digite o product_id para procurar na árvore B:\t(Exemplo: 1004856)\n"); // 1004856
				scanf("%lu", &product_id_busca);
				produto_aux = buscarProductArvore(bt_produtos, product_id_busca);
				if(produto_aux != NULL){
					printf("Produto encontrado: %llu\n", produto_aux->product_id);
				} else {
					printf("Produto não encontrado!\n");
					break;
				}
				printf("Informações do produto, no arquivo de dados:\n");
				start_time = get_time();
				line_product_aux = encontrar_produto_arquivo("indice_produtos.bin", product_id_busca);
				end_time = get_time();
				printf("* %lf segundos para encontrar o produto na Árvore B.\n", end_time-start_time);
				if(line_product_aux != NULL){
					printf("\t\tproduct_id: %llu\n\t\tindice: %llu\n\t\tcategory_id: %llu\n\t\tcategory_code: %s\n\t\tbrand: %s\n\t\tprice: %Lf\n\t\tremoved: %d\n",
						line_product_aux->product_id,
						line_product_aux->indice,
						line_product_aux->category_id,
						line_product_aux->category_code,
						line_product_aux->brand,
						((double)line_product_aux->price)/100.0,
						line_product_aux->removed
					);
				} else {
					printf("Linha do produto não encontrada!\n");
					break;
				}
				produto_aux = NULL;
				line_product_aux = NULL;
				break;
			case 12:
				printf("Digite o product_id para remover da árvore B:\t(Exemplo: 1004856)\n"); // 1004856
				scanf("%lu", &product_id_busca);
				start_time = get_time();
				if (removerProductArvore(bt_produtos, product_id_busca)){
					printf("Produto removido do índice.\n");
					end_time = get_time();
					printf("* %lf segundos para remover o produto da Árvore B.\n", end_time-start_time);
				} else {
					printf("Produto não encontrado no índice!\n");
				}
				break;
			case 13:
				// inicializar:
				line_product_aux = (LineProduct*)malloc(sizeof(struct line_product));
				line_product_aux2 = (LineProduct*)malloc(sizeof(struct line_product));
				product_id_busca = -1;
				category_id_aux = -1;
				price_aux = -1;
				memset(category_code_aux, '\0', sizeof(char) * 64);
				memset(brand_aux, '\0', sizeof(char) * 64);
				// prompts:
				printf("Digite o product_id do produto:\t(Exemplo: 1004856)\n");
				scanf("%lu%*c", &product_id_busca);
				printf("Digite o category_id do produto:\t(Exemplo: 2053013555631882655)\n");
				scanf("%llu%*c", &category_id_aux);
				printf("Digite o category_code do produto:\t(Exemplo: electronics.smartphone)\n");
				if(fgets(category_code_aux, 64, stdin))
					category_code_aux[strcspn(category_code_aux, "\n")] = 0;
				printf("Digite a marca do produto:\t(Exemplo: samsung)\n");
				if(fgets(brand_aux, 64, stdin))
					brand_aux[strcspn(brand_aux, "\n")] = 0;
				printf("Digite o preço do produto em centavos:\t(Exemplo: 48907 para $489.07)\n");
				scanf("%llu%*c", &price_aux);
				// criar linha:
				line_product_aux->product_id = product_id_busca;
				line_product_aux->category_id = category_id_aux;
				strcpy(line_product_aux->category_code, category_code_aux);
				strcpy(line_product_aux->brand, brand_aux);
				line_product_aux->price = price_aux;
				line_product_aux->removed = 0;
				line_product_aux->mandatory_newline = '\n';
				// abrir o arquivo e olhar quantas linhas ele tem, para atualizar o line_product_aux->indice:
				FILE *file_products = fopen("arquivo_produtos.bin", "r+");
				if (file_products == NULL) {
			        printf("Erro ao abrir o arquivo, ou ele não existe, ou é erro do UAC.\t(Coloque o programa em uma pasta que não seja a \"Downloads\")\n");
			    } else {
					fseek(file_products, (long)sizeof(struct line_product) * -1, SEEK_END);
					fread(line_product_aux2, (long)sizeof(struct line_product), 1, file_products);
					printf("Há %d entradas. Escrevendo linha %d.\n", line_product_aux2->indice, (line_product_aux2->indice)+1);
					line_product_aux->indice = (line_product_aux2->indice)+1;
					fseek(file_products, 0, SEEK_END);
					fwrite(line_product_aux, sizeof(struct line_product), 1, file_products);
					fclose(file_products);
					RegistroIndiceProduto *registro_aux = malloc(sizeof(struct registro_indice_produto));
					registro_aux->product_id = product_id_busca;
					registro_aux->indice = line_product_aux->indice;
					start_time = get_time();
					btInserir(bt_produtos, registro_aux);
					end_time = get_time();
					printf("* %lf segundos para adicionar o produto na Árvore B.\n", end_time-start_time);
				}
				break;
			case 14:
				memset(nome_arquivo, '\0', sizeof(char) * 64);
				printf("Digite o nome do arquivo .csv a ser lido, para criar a tabela hash:\t(Exemplos: 2019-Nov-small.csv, 2019-Nov.csv)\n> ");
				if(fgets(nome_arquivo, 128, stdin)){
					nome_arquivo[strcspn(nome_arquivo, "\n")] = 0;
				}
				printf("Arquivo: \"%s\"\n", nome_arquivo);
				start_time = get_time();
				criar_tabela_hash_produtos(nome_arquivo, &registros_acessos_hash, tabela_hash_produtos);
				end_time = get_time();
				printf("* %lf segundos para criar a tabela hash.\n", end_time-start_time);
				break;
			// (category_code, brand, price)
			case 15:
				// [Índice] timestamp: 1569888057 - índice: 10068
				printf("Exemplo: appliances.sewing_machine -- janome -- 29365\n");
				printf("Exemplo: construction.tools.welding -- magnetta -- 25478\n");
				printf("Digite o category_code do produto:\n");
				if(fgets(category_code_aux, 64, stdin))
					category_code_aux[strcspn(category_code_aux, "\n")] = 0;
				printf("Digite a marca do produto:\n");
				if(fgets(brand_aux, 64, stdin))
					brand_aux[strcspn(brand_aux, "\n")] = 0;
				printf("Digite o preço do produto em centavos:\n");
				scanf("%llu%*c", &price_aux);
				start_time = get_time();
				for(int i=0; i<2; i++){
					timestamp_inicio = -1;
					sucesso = -1;
					skip = 0;
					hash_aux = hash_produto(category_code_aux, brand_aux, price_aux);
					if(i)
						printf("* Hash gerado: %llu *\n", hash_aux);
					while(sucesso != 0){
						timestamp_inicio = procurar_hash(tabela_hash_produtos, hash_aux, skip, i, &entrada_hash_remover);
						if(timestamp_inicio == -1) break;
						sucesso = mostrar_acessos_produtos_intervalo_hash(category_code_aux, brand_aux, price_aux, timestamp_inicio, timestamp_inicio, i);
						skip += 1;
					}
				}
				if(sucesso == -1 || sucesso == 1){
					printf("Não encontrado.");
				} else {
					end_time = get_time();
					printf("* %lf segundos para encontrar o produto pela tabela hash + índice de acessos.\n", end_time-start_time);
				}
				break;
			case 16:
				// [Índice] timestamp: 1569888057 - índice: 10068
				entrada_hash_remover = NULL;
				printf("Exemplo: appliances.sewing_machine -- janome -- 29365\n");
				printf("Exemplo: construction.tools.welding -- magnetta -- 25478\n");
				printf("Digite o category_code do produto:\n");
				if(fgets(category_code_aux, 64, stdin))
					category_code_aux[strcspn(category_code_aux, "\n")] = 0;
				printf("Digite a marca do produto:\n");
				if(fgets(brand_aux, 64, stdin))
					brand_aux[strcspn(brand_aux, "\n")] = 0;
				printf("Digite o preço do produto em centavos:\n");
				scanf("%llu%*c", &price_aux);
				start_time = get_time();
				for(int i=0; i<2; i++){
					timestamp_inicio = -1;
					sucesso = -1;
					skip = 0;
					hash_aux = hash_produto(category_code_aux, brand_aux, price_aux);
					if(i)
						printf("* Hash gerado: %llu *\n", hash_aux);
					while(sucesso != 0){
						timestamp_inicio = procurar_hash(tabela_hash_produtos, hash_aux, skip, i, &entrada_hash_remover);
						if(timestamp_inicio == -1) break;
						sucesso = mostrar_acessos_produtos_intervalo_hash(category_code_aux, brand_aux, price_aux, timestamp_inicio, timestamp_inicio, i);
						skip += 1;
					}
				}
				if(sucesso == -1 || sucesso == 1){
					printf("Não encontrado.");
				} else {
					end_time = get_time();
					printf("* %lf segundos para remover o produto da tabela hash.\n", end_time-start_time);
					entrada_hash_remover->removed = 1;
				}
				break;
			default:
				if(entrada[0] == 'X' || entrada[0] == 'x'){
					printf("Saindo.\n");
					break;
				} else {
					printf("Opção inválida! Tente novamente.\n");
					break;	
				}
		}
	
		printf("Pressione Enter para continuar.\n");
		pause();
	} while (entrada[0] != 'x' && entrada[0] != 'X');
	return 0;
}

