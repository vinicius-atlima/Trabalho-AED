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
	int estreia;
	int atividade;
	int encerramento;
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


//---------------------------------Verificam listas vazias-----------------------------------

int listaGenerosEhVazia (ListaPrincipal *l) {
	// Verifica se ainda não existem gêneros cadastrados
	return l->inicio == NULL;
}

int listaArtistasEhVazia (Genero *g) {
	// Verifica se um gênero ainda não tem artistas cadastrados
	return g->inicioArtistas == NULL;
}


//---------------------Funções de manipulação sobre a lista de gêneros-----------------------

ListaPrincipal *iniciaListaGeneros () {
	// Aloca uma lista de gêneros vazia
	ListaPrincipal *l = (ListaPrincipal *) malloc(sizeof(*l));
	l->inicio = NULL;
	l->fim = NULL;
	l->qtdGeneros = 0;
	return l;
}

Genero *criaGenero (int idGenero, const char nomeGenero[]) {
	// Cria um "nó gênero" (que aponta para uma lista vazia) dado um ID e um nome
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
	novo->ant = NULL;
	novo->prox = NULL;
  
	return novo;
}

void insereGenero (ListaPrincipal *l, Genero *g) {
	// Insere um gênero no fim da lista de gêneros (para manter a ordem do arquivo)
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


//---------------------Funções de manipulação sobre a lista de artistas----------------------

Artista *criaArtista (int idArtista, const char nomeArtista[], int numDeIntegrantes, int numDePremiacoes,
					  const char cidadeNatal[], int anoDeEstreia, int naAtiva, int ultimaMusica,
					  const char melhoresObras[]) {
	// Cria um "nó artista", dadas as informações para preencher seus campos
	Artista *novo = (Artista *) malloc(sizeof(*novo));
	if (novo == NULL) {
		printf("Erro de memória.");
		getchar();
		exit(EXIT_FAILURE);
	}
  
	novo->id = idArtista;
	strcpy(novo->nome, nomeArtista);
    novo->qtdIntegrantes = numDeIntegrantes;
	novo->qtdPremiacoes = numDePremiacoes;
	strcpy(novo->cidadeOrigem, cidadeNatal);
	novo->estreia = anoDeEstreia;
	novo->atividade = naAtiva;
	novo->encerramento = ultimaMusica;
	strcpy(novo->principaisObras, melhoresObras);
	novo->ant = NULL;
	novo->prox = NULL;
  
	return novo;
}

void insereArtistaNoGenero (Genero *g, Artista *a) {
	// Insere um artista no fim da lista de artistas (para manter a ordem de aparição no arquivo)
	if (listaArtistasEhVazia(g)) {
		g->qtdArtistas = 1;
		a->prox = NULL;
		a->ant = NULL;
		g->inicioArtistas = a;
		g->fimArtistas = a;
	}
	
	g->qtdArtistas++;
	a->ant = g->fimArtistas;
	g->fimArtistas->prox = a;
	a->prox = NULL;
	g->fimArtistas = a;
}

/* int totalDeArtistas (ListaPrincipal *l) { */

/* } */


//----------------------------Funções que imprimem informações-------------------------------

void imprimeGenero (Genero *g) {
	// Imprime as informações de um gênero
	printf("Id do gênero: %d\n", g->id);
	printf("Nome do gênero: %s\n", g->nome);
	printf("Quantidade de artistas cadastrados do gênero: %d\n\n", g->qtdArtistas);
}

void imprimeListaGeneros (ListaPrincipal *l) {
	// Imprime as informações de todos os gêneros cadastrados
	if (listaGenerosEhVazia(l)) {
		printf("Lista de gêneros vazia.\n");
		return;
	}
  
	Genero *atual = l->inicio;
	while (atual != NULL) {
		imprimeGenero(atual);
		atual = atual->prox;
	}
}

void imprimeArtista (Artista *a) {
	// Imprime as informações de um artista
	printf("Id do artista: %d\n", a->id);
	printf("Nome do artista/grupo: %s\n", a->nome);
	printf("Cidade de origem: %s\n", a->cidadeOrigem);
	printf("Quantidade de integrantes: %d\n", a->qtdIntegrantes);
	printf("Ano de estreia: %d\n", a->estreia);
	printf("Em atividade: %s\n", (a->atividade) ? "Sim" : "Não");
	printf("Principais obras: %s\n", a->principaisObras);
	printf("Ano de lançamento da música mais recente: %d\n", a->encerramento);
	printf("Quantidade de prêmios obtidos: %d\n\n", a->qtdPremiacoes);
}

void imprimeListaArtistas (Genero *g) {
	// Imprime as informações de todos os artistas cadastrados em um determinado gênero
	if (listaArtistasEhVazia(g)) {
		printf("Lista de artistas vazia.\n");
		return;
	}
  
	Artista *atual = g->inicioArtistas;
	while (atual != NULL) {
		imprimeArtista(atual);
		atual = atual->prox;
	}
}

/* void imprimeRelatorioGeral (ListaPrincipal *l) { */
	
/* } */


//----------------------Funções de manipulação de arquivos (Vinícius)------------------------

FILE *abreArquivoPraLer (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "read" e checa se houve erro; retorna o ponteiro para o arquivo
	fp = fopen(nome, "r");
  
	if (fp == NULL) {
		printf("Erro na abertura do arquivo. Rode o programa novamente.");
		getchar();
		exit(EXIT_FAILURE);
	}

	return fp;
}

FILE *abreArquivoPraEscrever (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "append" e checa se houve erro; retorna o ponteiro para o arquivo
	fp = fopen(nome, "a");
  
	if (fp == NULL) {
		printf("Erro na abertura do arquivo. Rode o programa novamente.");
		getchar();
		exit(EXIT_FAILURE);
	}
  
	return fp;
}

Genero *lerGenero (FILE *fp) {
	// Lê um gênero (de uma linha do arquivo de leitura) e o retorna
	char buffer[MAX_STRING];
	
	if (fgets(buffer, sizeof(buffer), fp) == NULL) return NULL; /* Se o fgets encontra EOF, então ela retorna NULL e
	                                                               a função lerGenero também retorna um ponteiro
	                                                               nulo, o que será a condição de parada da função
	                                                               "carregaGeneros" */
	int idGenero;
	char nomeGenero[MAX_STRING];
	char *ponteiro;
	char *token = strtok_r(buffer, ";", &ponteiro); /* "token" recebe a primeira parte da linha do arquivo (antes do
													   ponto e vírgula), que é o ID do gênero da linha em questão */
	if (token == NULL) {
		printf("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos campos "
			   "corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa novamente.)");
		getchar();
		exit(EXIT_FAILURE);
	}
	sscanf(token, "%d", &idGenero); // Processa a string lida como um número (ID é um int) e atribui a "idGenero"
  
	token = strtok_r(NULL, ";", &ponteiro); /* "token" agora contém o nome do gênero, que vem depois do ponto e
	                                           vírgula (a segunda parte da linha) */
	if (token == NULL) {
		printf("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos campos "
			   "corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa novamente.)");
		getchar();
		exit(EXIT_FAILURE);
	}
	
	token[strcspn(token, "\r\n")] = '\0'; /* Essa linha pega a primeira quebra de linha no final da string e troca
	                                         por '\0' (termina a string automaticamente, ignorando '\r' ou '\n') */

	return criaGenero(idGenero, token); // "token" já contém a string que é o nome do gênero
}

void carregaGeneros (FILE *fp, ListaPrincipal *l) {
	// Carrega todos os gêneros cadastrados em um arquivo ("generos.txt") para a lista principal
	int c;
	while ((c = fgetc(fp)) != '\n'); /* Essa parte (últimas duas linhas) só serve para pularmos a primeira linha,
	                                    que é um cabeçalho */
	Genero *g;
	while ((g = lerGenero(fp)) != NULL) {
		insereGenero(l, g);
	}
}

/* Artista *lerArtista (FILE *fp) { */
	
/* } */

/* void carregaArtistas (FILE *fp, ListaPrincipal *l) { */
	
/* } */


//-------------------------------------------------------------------------------------------
