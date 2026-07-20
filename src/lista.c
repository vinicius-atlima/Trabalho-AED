#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//-------------------------------Definições das estruturas-----------------------------------

struct artista {
    // Estrutura nó da lista secundária (artista / banda)
    int id;
    char nome[MAX_STRING];
    char cidadeOrigem[MAX_STRING];
    char periodoAtuacao[MAX_STRING];
    char principaisObras[MAX_STRING];
    int qtdIntegrantes;
    int qtdPremiacoes;

    struct artista* ant;
    struct artista* prox;
};

struct genero {
    // Estrutura nó da lista principal (gênero)
    int id;
    char nome[MAX_STRING];

    Artista* inicioArtistas;
    Artista* fimArtistas;
    int qtdArtistas;

    struct genero* ant;
    struct genero* prox;
};

struct listaPrincipal {
    // Estrutura lista principal (de gêneros)
    struct genero* inicio;
    struct genero* fim;
    int qtdGeneros;
};


//---------------------------------Funções de verificação------------------------------------

int listaGenerosEhVazia (ListaPrincipal *l) {
	// Verifica se ainda não existem gêneros cadastrados.
	return l->inicio == NULL;
}

int listaArtistasEhVazia (Genero *g) {
	// Verifica se um gênero ainda não tem artistas cadastrados.
	return g->inicioArtistas == NULL;
}


//-----------------------------Funções de manipulação de listas------------------------------

ListaPrincipal *iniciaListaGeneros () {
	// Aloca uma lista de gêneros vazia.
	ListaPrincipal *l = (ListaPrincipal *) malloc(sizeof(*l));
	l->inicio = NULL;
	l->fim = NULL;
	l->qtdGeneros = 0;
	return l;
}

Genero *criaGenero (int idGenero, const char nomeGenero[]) {
	// Cria um "nó gênero" (que aponta para uma lista vazia).
	Genero *novo = (Genero *) malloc(sizeof(*novo));
	if (novo == NULL) {
		printf("Erro de memória.");
		getchar();
		exit(EXIT_FAILURE);
	}
  
	novo->id = idGenero;
	strcpy(novo->nome, nomeGenero);
	novo->inicioArtistas = NULL;
	novo->fimArtistas = NULL;
	novo->qtdArtistas = 0;
  
	return novo;
}

void insereGenero (ListaPrincipal *l, Genero *g) {
	// Insere um gênero no fim da lista de gêneros.
	if (listaGenerosEhVazia(l)) {
		l->qtdGeneros = 1;
		g->prox = NULL;
		g->ant = NULL;
		l->inicio = g;
		l->fim = g; 
	}
	l->qtdGeneros++;
	g->ant = l->fim;
	l->fim->prox = g;
	g->prox = NULL;
	l->fim = g;
}


//---------------------------------Funções de impressão--------------------------------------

void imprimeGenero (Genero *g) {
	// Imprime as informações de um gênero.
	printf("Nome do gênero: %s\n", g->nome);
	printf("Id do gênero: %d\n", g->id);
	printf("Quantidade de artistas cadastrados do gênero: %d\n\n", g->qtdArtistas);
}

void imprimeListaGeneros (ListaPrincipal *l) {
	// Imprime as informações de todos os gêneros cadastrados.
	if (listaGenerosEhVazia(l)) {
		printf("Lista de gêneros vazia.");
		return;
	}
  
	int c;
  
	Genero *atual = l->inicio;
	while (atual != NULL) {
		imprimeGenero(atual);
		atual = atual->prox;
	}
}


//-----------------------Funções de manipulação de arquivos (Vinícius)-----------------------

FILE *abreArquivoPraLer (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "read" e checa se houve erro. Retorna o ponteiro para o arquivo.
	fp = fopen(nome, "r");
  
	if (fp == NULL) {
		printf("Erro na abertura do arquivo. Rode o programa novamente.");
		getchar();
		exit(EXIT_FAILURE);
	}

	return fp;
}

FILE *abreArquivoPraEscrever (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "append" e checa se houve erro. Retorna o ponteiro para o arquivo.
	fp = fopen(nome, "a");
  
	if (fp == NULL) {
		printf("Erro na abertura do arquivo. Rode o programa novamente.");
		getchar();
		exit(EXIT_FAILURE);
	}
  
	return fp;
}

Genero *lerGenero (FILE *fp) {
	// Lê um gênero (do arquivo de leitura) e o retorna.
	char buffer[MAX_STRING];
	if (fgets(buffer, sizeof(buffer), fp) == NULL) {
		return NULL;
	}

	int idGenero;
	char nomeGenero[MAX_STRING];
	char *ponteiro;
	char *token = strtok_r(buffer, ";", &ponteiro);
	if (token == NULL) {
		printf("Erro na leitura do arquivo: formato não suportado.");
		getchar();
		exit(EXIT_FAILURE);
	}
	sscanf(token, "%d", &idGenero);
  
	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		printf("Erro na leitura do arquivo: formato não suportado.");
		getchar();
		exit(EXIT_FAILURE);
	}
	token[strcspn(token, "\r\n")] = '\0';

	return criaGenero(idGenero, token);
}

void carregaGeneros (FILE *fp, ListaPrincipal *l) {
	// Carrega todos os gêneros cadastrados em um arquivo ("generos.txt") para a lista principal.
	Genero *g;
	while ((g = lerGenero(fp)) != NULL) {
		insereGenero(l, g);
	}
}


//-------------------------------------------------------------------------------------------
