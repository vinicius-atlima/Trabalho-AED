#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//------------------------------ Definições das estruturas ----------------------------------

struct artista {
	// Estrutura nó da lista secundária (artista / banda)
	char nome[MAX_STRING];
	char cidadeOrigem[MAX_STRING];
	char principaisObras[MAX_STRING];

	struct artista* ant;
	struct artista* prox;

	int id;
	int qtdIntegrantes;
	int qtdPremiacoes;
	int estreia;
	int atividade;
	int encerramento;
};

struct genero {
	// Estrutura nó da lista principal (gênero)
	char nome[MAX_STRING];

	Artista* inicioArtistas;
	Artista* fimArtistas;
	
	struct genero* ant;
	struct genero* prox;

	int id;
	int qtdArtistas;
};

struct listaPrincipal {
	// Estrutura lista principal (de gêneros)
	struct genero* inicio;
	struct genero* fim;
	int qtdGeneros;
};

//===================================================================================================================


ListaPrincipal* criarListaPrincipal(void) {
    ListaPrincipal* lp = (ListaPrincipal*) malloc(sizeof(ListaPrincipal));
    if (lp != NULL) {
        lp->inicio = NULL;
        lp->fim = NULL;
        lp->qtdGeneros = 0;
    }
    return lp;
}

void destruirListaPrincipal(ListaPrincipal* lp) {
    if (lp == NULL) return;

    Genero* atual = lp->inicio;
    while (atual != NULL) {
        Genero* proxGenero = atual->prox;
        
        // Desaloca a lista secundária de artistas associada ao gênero
        Artista* aAtual = atual->inicioArtistas;
        while (aAtual != NULL) {
            Artista* proxArtista = aAtual->prox;
            free(aAtual);
            aAtual = proxArtista;
        }

        free(atual);
        atual = proxGenero;
    }

    free(lp);
}

void inserirGenero(ListaPrincipal* lp, int id, const char* nome) {
    if (lp == NULL || nome == NULL) return;

    if (buscarGenero(lp, id) != NULL) {
        printf("Erro: ID %d ja existe!\n", id);
        return;
    }

    Genero* novo = (Genero*) malloc(sizeof(Genero));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return;
    }

    novo->id = id;
    strncpy(novo->nome, nome, MAX_STRING - 1);
    novo->nome[MAX_STRING - 1] = '\0';

    novo->inicioArtistas = NULL;
    novo->fimArtistas = NULL;
    novo->qtdArtistas = 0;

    novo->ant = lp->fim;
    novo->prox = NULL;

    if (lp->inicio == NULL) {
        lp->inicio = novo;
    } else {
        lp->fim->prox = novo;
    }

    lp->fim = novo;
    lp->qtdGeneros++;
}

Genero* buscarGenero(ListaPrincipal* lp, int id) {
    if (lp == NULL) return NULL;

    Genero* atual = lp->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; 
}