// ler: https://ava.ucs.br/courses/44997/assignments/289234
// ver: https://www.youtube.com/watch?v=K1a2Bk8NrYQ
// ver: https://www.cs.yale.edu/homes/aspnes/pinewiki/BTrees.html
// testar: https://www.cs.usfca.edu/~galles/visualization/BTree.html

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "btree_products.h"
#include "index.h"

/*
=======================================================
	Árvores B:
==========================================================
*/

BTree_Prod criar_arvore_produtos() {
	BTree_Prod bt;
	bt = malloc(sizeof(*bt));
	assert(bt);

	bt->folha = 1;
	bt->numeroElementos = 0;
	memset(bt->chaves, 0, sizeof(struct registro_indice_produto*) * (ORDEM -1));
	memset(bt->filhos, 0, sizeof(struct btree_products_node*) * ORDEM);

	return bt;
}

void destruir_arvore_produtos(BTree_Prod bt) {
	int i;
	if(!bt->folha) {
		for(i = 0; i < (bt->numeroElementos + 1); i++) {
			destruir_arvore_produtos(bt->filhos[i]);
		}
	}
	free(bt);
}

// achar produto .. dentro das chaves de um nó .. e retornar a posição dele, ou do maior antes dele:
int __procurarLinha(int n, struct registro_indice_produto* chaves[], unsigned long int product_id) {
	unsigned long int lo, mid, hi;
	lo = -1;
	hi = n;
	while(lo+1<hi) {
		mid = (lo+hi)/2;
		if(chaves[mid]->product_id == product_id) {
			return mid;
		} else if(chaves[mid]->product_id < product_id) {
			lo = mid;
		} else {
			hi = mid;
		}
	}

	return hi;
}

// procurar por o nó que tenha o product_id .. dentro da árvore .. para ver se o produto está cadastrado:
int procurarLinha(BTree_Prod bt, unsigned long int product_id) {
	int pos;
	if(bt->numeroElementos == 0) {
		return 0;
	}
	// procurar pela número que seje o product_id, ou venha antes dele:
	pos = __procurarLinha(bt->numeroElementos, bt->chaves, product_id);
	//até aqui ok.
	if(pos < bt->numeroElementos && bt->chaves[pos]->product_id == product_id) {
		return 1;
	} else {
		return(!bt->folha && procurarLinha((BTree_Prod)bt->filhos[pos], product_id));
	}
}

// inserir um novo nó na árvore:
BTree_Prod btInserirInterno(BTree_Prod bt, RegistroIndiceProduto *novoProduto, RegistroIndiceProduto **mediana) {
	int pos;
	int mid;
	BTree_Prod noveNode;

	pos = __procurarLinha(bt->numeroElementos, bt->chaves, novoProduto->product_id);

	// se o product_id já existir:
	if (pos < bt->numeroElementos && bt->chaves[pos]->product_id == novoProduto->product_id) {
		// se o product_id foi ""removido"" (desativado), reativar ele:
		if(bt->chaves[pos]->removed == 1){
			bt->chaves[pos]->removed = 0;
			free(novoProduto); // testar se não dá bug
		}
		return NULL;
	}

	if (bt->folha) {
		memmove(&bt->chaves[pos + 1], &bt->chaves[pos], sizeof(*(bt->chaves)) * (bt->numeroElementos - pos));
		bt->chaves[pos] = novoProduto;
		bt->numeroElementos++;
	} else {
		RegistroIndiceProduto *novaMediana = NULL;
		noveNode = btInserirInterno(bt->filhos[pos], novoProduto, &novaMediana);

		if (noveNode) {
			memmove(&bt->chaves[pos + 1], &bt->chaves[pos], sizeof(*(bt->chaves)) * (bt->numeroElementos - pos));
			memmove(&bt->filhos[pos + 2], &bt->filhos[pos + 1], sizeof(*(bt->filhos)) * (bt->numeroElementos - pos));
			bt->chaves[pos] = novaMediana;
			bt->filhos[pos + 1] = noveNode;
			bt->numeroElementos++;
		}
	}
	if (bt->numeroElementos >= ORDEM - 1) {
		mid = bt->numeroElementos / 2;

		*mediana = bt->chaves[mid];

		noveNode = malloc(sizeof(*noveNode));
		noveNode->numeroElementos = bt->numeroElementos - mid - 1;
		noveNode->folha = bt->folha;

		memmove(noveNode->chaves, &bt->chaves[mid + 1], sizeof(*(bt->chaves)) * noveNode->numeroElementos);
		if (!bt->folha) {
			memmove(noveNode->filhos, &bt->filhos[mid + 1], sizeof(*(bt->filhos)) * (noveNode->numeroElementos + 1));
		}

		bt->numeroElementos = mid;

		return noveNode;
	}

	return NULL;
}

void btInserir(BTree_Prod bt, RegistroIndiceProduto *novoProduto) {
    BTree_Prod b1; // esquerda
    BTree_Prod b2; // direita
    RegistroIndiceProduto *mediana;

    b2 = btInserirInterno(bt, novoProduto, &mediana);

    if (b2) {
        b1 = malloc(sizeof(*b1));
        assert(b1);
        memmove(b1, bt, sizeof(*b1));

        bt->numeroElementos = 1;
        bt->folha = 0;
        bt->chaves[0] = mediana;
        bt->filhos[0] = b1;
        bt->filhos[1] = b2;
    }
}

RegistroIndiceProduto* buscarProductArvore(BTree_Prod bt, unsigned long int product_id) {
    int pos;
    if (bt == NULL || bt->numeroElementos == 0) {
        return NULL;
    }
    pos = __procurarLinha(bt->numeroElementos, bt->chaves, product_id);

    if (pos < bt->numeroElementos && bt->chaves[pos]->product_id == product_id && bt->chaves[pos]->removed == 0) {
    	return bt->chaves[pos];
    }
    if (!bt->folha) {
        return buscarProductArvore(bt->filhos[pos], product_id);
    }
    return NULL;
}

int removerProductArvore(BTree_Prod bt, unsigned long int product_id) {
    int pos;
    if (bt == NULL || bt->numeroElementos == 0) {
        return 0;
    }
    pos = __procurarLinha(bt->numeroElementos, bt->chaves, product_id);

    if (pos < bt->numeroElementos && bt->chaves[pos]->product_id == product_id && bt->chaves[pos]->removed == 0) {
    	bt->chaves[pos]->removed = 1;
        return 1;
    }
    if (!bt->folha) {
        return removerProductArvore(bt->filhos[pos], product_id);
    }
    return 0;
}


/*
=======================================================
	Criação de índices:
==========================================================
*/

BTree_Prod criar_indice_produtos_arvore_b(char filename[], unsigned long long int *linhas_escritas){
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
	BTree_Prod arvore = criar_arvore_produtos();
	
	fseek(file_products, 0, SEEK_SET);
	
	printf("Inserindo registros na árvore B...\n");
	while(fread(line_product_aux, (long)sizeof(struct line_product), 1, file_products)){
		RegistroIndiceProduto *novoProduto = malloc(sizeof(RegistroIndiceProduto));
		novoProduto->product_id = line_product_aux->product_id;
		novoProduto->indice = line_product_aux->indice;
		novoProduto->removed = 0;
		btInserir(arvore, novoProduto);
	}
	fclose(file_products);
	
	printf("Índice de produtos criado na memória.\n");
	*linhas_escritas = registros;
	return arvore;
}

void mostrar_arvore(BTree_Prod bt, int nivel, BT_Prod_Node* pai) {
    if (bt == NULL) {
        return;
    }
    printf("nível: %d\n", nivel);

	// chaves:
    for (int i = 0; i < bt->numeroElementos; i++) {
        printf("[%llu] (removido: %d)", bt->chaves[i]->product_id, bt->chaves[i]->removed);
    }
    printf("\n");

    // filhos:
    if (!bt->folha) {
        for (int i = 0; i <= bt->numeroElementos; i++) {
        	if(pai != NULL && bt->chaves[i] != NULL)
    			printf("\npai: %llu\n", bt->chaves[i]->product_id);
            mostrar_arvore(bt->filhos[i], nivel + 1, bt);
        }
    }
    return;
}
