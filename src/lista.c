#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//------------------------------ Definições das estruturas ----------------------------------

struct artista {
	// Estrutura nó da lista secundária (artista / banda)
	char nome[MAX_STRING];
	char cidadeOrigem[MAX_STRING]
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


//------------------------ Manipulação da lista de artistas (Fernando) ----------------------

static void copiarTexto(char destino[MAX_STRING], const char origem[]) {
	// Copia uma string para um campo de tamanho limitado, garantindo o terminador.
  snprintf(destino, MAX_STRING, "%s", origem);
}

static void exibirArtista(const Artista *artista) {
	// Exibe no terminal todos os dados armazenados em um artista.
  printf("ID: %d\n", artista->id);
  printf("Nome: %s\n", artista->nome);
  printf("Cidade de origem: %s\n", artista->cidadeOrigem);
  printf("Principais obras: %s\n", artista->principaisObras);
  printf("Quantidade de integrantes: %d\n", artista->qtdIntegrantes);
  printf("Quantidade de premiacoes: %d\n", artista->qtdPremiacoes);
  printf("Estreia: %d\n", artista->estreia);
  printf("Atividade: %d\n", artista->atividade);
  printf("Encerramento: %d\n", artista->encerramento);
  printf("------------------------------\n");
}

Artista *buscaArtistaGenero(Genero *genero, int id) {
	// Busca um artista pelo ID dentro da lista de um gênero específico.
  if (genero == NULL) {
    return NULL;
  }

	Artista *aux;
  aux = genero->inicioArtistas;
  while (aux != NULL) {
    if (aux->id == id) {
      return aux;
    }
    aux = aux->prox;
  }

  return NULL;
}

static Artista *localizarArtistaGlobal(ListaPrincipal *lista, int id) {
	// Localiza um artista pelo ID em todos os gêneros do sistema.
  Genero *genero;

  if (lista == NULL) {
    return NULL;
  }

  genero = lista->inicio;
  while (genero != NULL) {
    Artista *artista = buscaArtistaGenero(genero, id);

    if (artista != NULL) {
      return artista;
    }
    genero = genero->prox;
  }

  return NULL;
}


int inserirArtistaGenero(ListaPrincipal *lista, Genero *genero, int id, const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia,
												 int atividade, int encerramento) {
	// Cria e insere um artista no fim da lista do gênero, exigindo ID global único.
	Artista *novo;

  if (lista == NULL || genero == NULL || nome == NULL || cidadeOrigem == NULL ||
      principaisObras == NULL) {
    return 0;
  }

  if (localizarArtistaGlobal(lista, id) != NULL) {
    printf("Ja existe um artista com esse ID no sistema.\n");
    return 0;
  }

  novo = malloc(sizeof(Artista));
  if (novo == NULL) {
    printf("Falha na alocacao de memória.\n");
    return 0;
  }

  copiarTexto(novo->nome, nome);
  copiarTexto(novo->cidadeOrigem, cidadeOrigem);
  copiarTexto(novo->principaisObras, principaisObras);
  novo->ant = genero->fimArtistas;
  novo->prox = NULL;
  novo->id = id;
  novo->qtdIntegrantes = qtdIntegrantes;
  novo->qtdPremiacoes = qtdPremiacoes;
  novo->estreia = estreia;
  novo->atividade = atividade;
  novo->encerramento = encerramento;

  if (genero->inicioArtistas == NULL) {
    genero->inicioArtistas = novo;
  } else {
    genero->fimArtistas->prox = novo;
  }

  genero->fimArtistas = novo;
  genero->qtdArtistas++;

  return 1;
}

int alterarArtistaGenero(Genero *genero, int id, const char nome[], const char cidadeOrigem[],
												 const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia, int atividade,
                         int encerramento) {
	// Atualiza os dados do artista identificado pelo ID dentro de um gênero.
	Artista *artista;

  if (genero == NULL || nome == NULL || cidadeOrigem == NULL || principaisObras == NULL) {
    return 0;
  }

  artista = buscaArtistaGenero(genero, id);
  if (artista == NULL) {
    return 0;
  }

  copiarTexto(artista->nome, nome);
  copiarTexto(artista->cidadeOrigem, cidadeOrigem);
  copiarTexto(artista->principaisObras, principaisObras);
  artista->qtdIntegrantes = qtdIntegrantes;
  artista->qtdPremiacoes = qtdPremiacoes;
  artista->estreia = estreia;
  artista->atividade = atividade;
  artista->encerramento = encerramento;

  return 1;
}

int removerArtistaGenero(Genero *genero, int id) {
	// Remove do gênero o artista indicado pelo ID e reajusta o encadeamento da lista.
  Artista *artista;

  if (genero == NULL) {
    return 0;
  }

  artista = buscaArtistaGenero(genero, id);
  if (artista == NULL) {
    return 0;
  }

  if (artista->ant == NULL) {
    genero->inicioArtistas = artista->prox;
  } else {
    artista->ant->prox = artista->prox;
  }

  if (artista->prox == NULL) {
    genero->fimArtistas = artista->ant;
  } else {
    artista->prox->ant = artista->ant;
  }

  free(artista);
  genero->qtdArtistas--;

  return 1;
}

void listarArtistasGenero(Genero *genero) {
	// Percorre e exibe todos os artistas cadastrados em um gênero.
  Artista *aux;

  if (genero == NULL) {
    return;
  }

  aux = genero->inicioArtistas;
  while (aux != NULL) {
    exibirArtista(aux);
    aux = aux->prox;
  }
}

Artista *buscaGlobalArtista(ListaPrincipal *lista, int id) {
	// Busca um artista pelo ID no sistema, exibe seus dados e retorna seu endereço.
  Genero *auxGenero;
  Artista *artista;

  if (lista == NULL) {
    return NULL;
  }

  artista = localizarArtistaGlobal(lista, id);
  if (artista == NULL) {
    return NULL;
  }

  auxGenero = lista->inicio;
  while (auxGenero != NULL) {
    if (buscaArtistaGenero(auxGenero, id) == artista) {
      printf("Genero: %s\n", auxGenero->nome);
      exibirArtista(artista);
      return artista;
    }

    auxGenero = auxGenero->prox;
  }

  return NULL;
}

void filtrarArtistasPremiacoes(ListaPrincipal *lista, int minimoPremiacoes) {
	// Exibe os artistas do sistema cuja quantidade de premiações atinge o mínimo.
  Genero *auxGenero;
  int qtdEncontrada = 0;

  if (lista == NULL) {
    return;
  }

  auxGenero = lista->inicio;
  while (auxGenero != NULL) {
    Artista *auxArtista = auxGenero->inicioArtistas;

    while (auxArtista != NULL) {
      if (auxArtista->qtdPremiacoes >= minimoPremiacoes) {
        printf("Genero: %s\n", auxGenero->nome);
        exibirArtista(auxArtista);
        qtdEncontrada++;
      }
      auxArtista = auxArtista->prox;
    }

    auxGenero = auxGenero->prox;
  }

  if (qtdEncontrada == 0) {
    printf("Nenhum artista foi encontrado.\n");
  }
}


//------------------------------- Verificam listas vazias -----------------------------------

int listaGenerosEhVazia (ListaPrincipal *l) {
	// Verifica se ainda não existem gêneros cadastrados
	return l->inicio == NULL;
}

int listaArtistasEhVazia (Genero *g) {
	// Verifica se um gênero ainda não tem artistas cadastrados
	return g->inicioArtistas == NULL;
}


//-------------------- Funções de manipulação sobre a lista de gêneros ----------------------

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


//-------------------- Funções de manipulação sobre a lista de artistas ---------------------

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

/* int totalDeArtistas (ListaPrincipal *l) { */

/* } */


//--------------------------- Funções que imprimem informações ------------------------------

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


//--------------------------------- Funções de remoção --------------------------------------




//---------------------------------- Funções de busca ---------------------------------------




//-------------------------- Consultas e funções de cruzamento ------------------------------




//--------------------------------- Impressão de menus --------------------------------------




//--------------------- Funções de manipulação de arquivos (Vinícius) -----------------------

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


//--------------------------------- Liberação de memória ------------------------------------




//-------------------------------------------------------------------------------------------
