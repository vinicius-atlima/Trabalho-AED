#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//------------------------------ Definições das Estruturas ----------------------------------

struct artista {
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
    char nome[MAX_STRING];

    Artista* inicioArtistas;
    Artista* fimArtistas;   
    
    struct genero* ant;
    struct genero* prox; 

    int id;
    int qtdArtistas;     
};

struct listaPrincipal {
    struct genero* inicio; 
    struct genero* fim;   
    int qtdGeneros;        
};

//===========================================================================================
// GERENCIAMENTO DE MEMÓRIA DA LISTA
//===========================================================================================

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
        
        // Libera a sublista de artistas vinculada ao gênero
        Artista* aAtual = atual->inicioArtistas;
        while (aAtual != NULL) {
            Artista* proxArtista = aAtual->prox;
            free(aAtual);
            aAtual = proxArtista;
        }

        // Libera o gênero atual
        free(atual);
        atual = proxGenero;
    }

    free(lp);
}

//===========================================================================================
// OPERAÇÕES CRUD DA LISTA PRINCIPAL (GÊNEROS)
//===========================================================================================

void inserirGenero(ListaPrincipal* lp, int id, const char* nome) {
    if (lp == NULL || nome == NULL) return;

    // Impede cadastro com ID duplicado
    if (buscarGenero(lp, id) != NULL) {
        printf("Erro: ID %d ja existe!\n", id);
        return;
    }

    Genero* novo = (Genero*) malloc(sizeof(Genero));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return;
    }

    // Copia dados e inicializa sublista de artistas
    novo->id = id;
    strncpy(novo->nome, nome, MAX_STRING - 1);
    novo->nome[MAX_STRING - 1] = '\0';

    novo->inicioArtistas = NULL;
    novo->fimArtistas = NULL;
    novo->qtdArtistas = 0;

    // Encadeamento no fim da lista duplamente encadeada
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

    // Busca sequencial por ID
    Genero* atual = lp->inicio;
    while (atual != NULL) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL; 
}

int alterarGenero(ListaPrincipal* lp, int id, const char* novoNome) {
    if (lp == NULL || novoNome == NULL) return 0;

    Genero* g = buscarGenero(lp, id);
    if (g == NULL) return 0;

    // Atualiza o nome do gênero localizado
    strncpy(g->nome, novoNome, MAX_STRING - 1);
    g->nome[MAX_STRING - 1] = '\0';

    return 1;
}

int removerGenero(ListaPrincipal* lp, int id) {
    if (lp == NULL) return 0;

    Genero* g = buscarGenero(lp, id);
    if (g == NULL) return 0;

    // Ajusta o ponteiro do nó anterior
    if (g->ant != NULL) {
        g->ant->prox = g->prox;
    } else {
        lp->inicio = g->prox;
    }

    // Ajusta o ponteiro do nó seguinte
    if (g->prox != NULL) {
        g->prox->ant = g->ant;
    } else {
        lp->fim = g->ant;
    }

    // Libera os artistas pertencentes a este gênero
    Artista* aAtual = g->inicioArtistas;
    while (aAtual != NULL) {
        Artista* proxArtista = aAtual->prox;
        free(aAtual);
        aAtual = proxArtista;
    }

    free(g);
    lp->qtdGeneros--;

    return 1;
}

//===========================================================================================
// LISTAGEM E CONSULTAS
//===========================================================================================

void listarGeneros(const ListaPrincipal* lp) {
    if (lp == NULL || lp->inicio == NULL) {
        printf("Nenhum genero cadastrado.\n");
        return;
    }

    printf("\n=== LISTA DE GÊNEROS ===\n");
    Genero* atual = lp->inicio;
    while (atual != NULL) {
        printf("ID: %d | Nome: %s | Total de Artistas: %d\n",
               atual->id, atual->nome, atual->qtdArtistas);
        atual = atual->prox;
    }
    printf("========================\n");
}

int contarGeneros(const ListaPrincipal* lp) {
    if (lp == NULL) return 0;
    return lp->qtdGeneros;
}

void contabilizarArtistasPorGenero(const ListaPrincipal* lp) {
    if (lp == NULL || lp->inicio == NULL) {
        printf("Nenhum genero cadastrado para contabilizar.\n");
        return;
    }

    printf("\n=== QUANTIDADE DE ARTISTAS POR GÊNERO ===\n");
    Genero* atual = lp->inicio;
    while (atual != NULL) {
        printf("Gênero: %-20s | Quantidade de Artistas: %d\n",
               atual->nome, atual->qtdArtistas);
        atual = atual->prox;
    }
    printf("=========================================\n");
}