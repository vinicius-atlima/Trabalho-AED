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
// FUNÇÕES DE GERENCIAMENTO DE MEMÓRIA DA LISTA
//===========================================================================================

/**
 * Aloca a estrutura gerenciadora e inicializa os ponteiros de controle.
 */
ListaPrincipal* criarListaPrincipal(void) {
    // Aloca a memória necessária para guardar os ponteiros inicio, fim e o contador qtdGeneros
    ListaPrincipal* lp = (ListaPrincipal*) malloc(sizeof(ListaPrincipal));
    
    // Garante que a alocação foi bem-sucedida antes de atribuir os valores iniciais
    if (lp != NULL) {
        lp->inicio = NULL;     // A lista começa vazia
        lp->fim = NULL;        // Sem elementos no final
        lp->qtdGeneros = 0;    // Quantidade zerada
    }
    
    // Retorna o ponteiro alocado (ou NULL em caso de falha de memória)
    return lp;
}

/**
 * Percorre recursivamente a estrutura encadeada liberando a memória do nível mais interno 
 * (artistas) para o nível mais externo (gêneros e a lista principal).
 */
void destruirListaPrincipal(ListaPrincipal* lp) {
    // Validação defensiva: se a lista não existe na memória, encerra
    if (lp == NULL) return;

    // Ponteiro auxiliar para navegar pela lista principal de gêneros
    Genero* atual = lp->inicio;
    
    while (atual != NULL) {
        // Salva o próximo gênero antes de apagar o atual, evitando perda de referência
        Genero* proxGenero = atual->prox;
        
        // --- LIBERAÇÃO DA SUBLISTA DE ARTISTAS DO GÊNERO ATUAL ---
        Artista* aAtual = atual->inicioArtistas;
        while (aAtual != NULL) {
            Artista* proxArtista = aAtual->prox; // Salva o próximo artista
            free(aAtual);                         // Libera o artista atual
            aAtual = proxArtista;                 // Avança para o próximo
        }

        // Após limpar todos os artistas do gênero, libera a memória do nó do gênero
        free(atual);
        
        // Avança para o próximo gênero salvo anteriormente
        atual = proxGenero;
    }

    // Por fim, libera a estrutura gerenciadora principal
    free(lp);
}

//===========================================================================================
// OPERAÇÕES CRUD DA LISTA PRINCIPAL (GÊNEROS)
//===========================================================================================

/**
 * Insere um novo gênero no final da lista duplamente encadeada.
 */
void inserirGenero(ListaPrincipal* lp, int id, const char* nome) {
    // Validação básica de ponteiros nulos
    if (lp == NULL || nome == NULL) return;

    // Impede a inserção de dois gêneros com o mesmo ID para manter a unicidade
    if (buscarGenero(lp, id) != NULL) {
        printf("Erro: ID %d ja existe!\n", id);
        return;
    }

    // Aloca a memória para o novo nó do gênero
    Genero* novo = (Genero*) malloc(sizeof(Genero));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        return;
    }

    // Atribui os dados do gênero
    novo->id = id;
    strncpy(novo->nome, nome, MAX_STRING - 1);
    novo->nome[MAX_STRING - 1] = '\0'; // Garante o caractere nulo no final da string

    // Inicializa a sublista de artistas vazia para esse novo gênero
    novo->inicioArtistas = NULL;
    novo->fimArtistas = NULL;
    novo->qtdArtistas = 0;

    // Como é inserido no FIM, o anterior do novo será o antigo fim da lista
    novo->ant = lp->fim;
    novo->prox = NULL; // Não há ninguém depois dele

    // Atualiza o encadeamento da lista principal
    if (lp->inicio == NULL) {
        // Se a lista estava vazia, o novo nó também é o primeiro elemento
        lp->inicio = novo;
    } else {
        // Se já havia elementos, o antigo último nó passa a apontar o 'prox' para o novo
        lp->fim->prox = novo;
    }

    // O ponteiro de fim da lista principal passa a apontar para o nó recém-criado
    lp->fim = novo;
    lp->qtdGeneros++; // Incrementa o contador total de gêneros
}

/**
 * Busca sequencial por ID. Retorna o ponteiro para alteração/uso direto ou NULL.
 */
Genero* buscarGenero(ListaPrincipal* lp, int id) {
    if (lp == NULL) return NULL;

    // Inicia a navegação a partir do primeiro nó da lista
    Genero* atual = lp->inicio;
    
    while (atual != NULL) {
        // Se encontrar o ID procurado, retorna o endereço de memória desse nó
        if (atual->id == id) {
            return atual;
        }
        // Avança para o próximo nó encadeado
        atual = atual->prox;
    }
    
    // Retorna NULL caso chegue ao fim da lista sem encontrar o ID
    return NULL; 
}

/**
 * Altera o nome de um gênero buscando seu nó pelo ID.
 */
int alterarGenero(ListaPrincipal* lp, int id, const char* novoNome) {
    if (lp == NULL || novoNome == NULL) return 0;

    // Utiliza a função de busca para reaproveitar o ponteiro do nó
    Genero* g = buscarGenero(lp, id);
    if (g == NULL) return 0; // Retorna 0 (falha) se o gênero não existir

    // Atualiza com segurança a string do nome
    strncpy(g->nome, novoNome, MAX_STRING - 1);
    g->nome[MAX_STRING - 1] = '\0';

    return 1; // Retorna 1 para sinalizar sucesso
}

/**
 * Desconecta o nó da lista duplamente encadeada e libera sua memória e a de seus artistas.
 */
int removerGenero(ListaPrincipal* lp, int id) {
    if (lp == NULL) return 0;

    // Localiza o nó a ser removido
    Genero* g = buscarGenero(lp, id);
    if (g == NULL) return 0; // Não encontrado

    // --- REARRANJO DOS PONTEIROS (DESCONEXÃO DO NÓ) ---

    // 1. Ajuste da ponta ANTERIOR
    if (g->ant != NULL) {
        // Se g não era o primeiro, faz o nó anterior apontar para o sucessor de g
        g->ant->prox = g->prox;
    } else {
        // Se g era o primeiro, o início da lista passa a ser o próximo de g
        lp->inicio = g->prox;
    }

    // 2. Ajuste da ponta PRÓXIMA
    if (g->prox != NULL) {
        // Se g não era o último, faz o nó seguinte apontar para o antecessor de g
        g->prox->ant = g->ant;
    } else {
        // Se g era o último, o fim da lista passa a ser o anterior de g
        lp->fim = g->ant;
    }

    // --- LIBERAÇÃO DA SUBLISTA DE ARTISTAS VINCULADA ---
    Artista* aAtual = g->inicioArtistas;
    while (aAtual != NULL) {
        Artista* proxArtista = aAtual->prox;
        free(aAtual);
        aAtual = proxArtista;
    }

    // Libera a memória do nó do gênero e reduz a contagem
    free(g);
    lp->qtdGeneros--;

    return 1; // Sucesso na remoção
}