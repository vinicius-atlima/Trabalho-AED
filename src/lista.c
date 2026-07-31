#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//============================== Definições das estruturas ==================================

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

//================================ Funções miscelâneas ======================================

static void copiarTexto(char destino[MAX_STRING], const char origem[]) {
	// Copia uma string para um campo de tamanho limitado, garantindo o terminador
	snprintf(destino, MAX_STRING, "%s", origem);
}

int listaGenerosEhVazia (ListaPrincipal *l) {
	// Verifica se ainda não existem gêneros cadastrados
	return l->inicio == NULL;
}

int listaArtistasEhVazia (Genero *g) {
	// Verifica se um gênero ainda não tem artistas cadastrados
	return g->inicioArtistas == NULL;
}

void mensagemDeErroForte(const char mensagem[]) {
	// Imprime uma mensagem de erro e fecha o programa imediatamente
	printf("%s", mensagem);
	getchar();
	exit(EXIT_FAILURE);
}

void mensagemDeErroFraca(const char mensagem[]) {
	// Imprime uma mensagem de erro, mas não encerra o programa
	printf("%s", mensagem);
	return;
}

FILE *abreArquivoPraLer (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "read" e checa se houve erro; retorna o ponteiro para o arquivo
	fp = fopen(nome, "r");
	if (fp == NULL) mensagemDeErroForte("Erro na abertura do arquivo. Rode o programa novamente.");
	return fp;
}

FILE *abreArquivoPraEscrever (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "append" e checa se houve erro; retorna o ponteiro para o arquivo
	fp = fopen(nome, "a");
	if (fp == NULL) mensagemDeErroForte("Erro na abertura do arquivo. Rode o programa novamente.");
	return fp;
}


//======================= Manipulação da lista de gêneros (Orlando) =========================

ListaPrincipal* criarListaPrincipal(void) {
	// Inicia a lista principal (de gêneros)
	ListaPrincipal* lp = (ListaPrincipal*) malloc(sizeof(ListaPrincipal));

	if (lp == NULL) {
		mensagemDeErroForte("Erro de memória. Rode o programa novamente.");
	}
	
	lp->inicio = NULL;
	lp->fim = NULL;
	lp->qtdGeneros = 0;

	return lp;
}

void destruirListaPrincipal(ListaPrincipal* lp) {
	// Libera a memória alocada para a lista principal
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

/* TODO: função destruirGenero para deixar a função acima mais modular. Também vamos precisar dessa função para remover
   gêneros! */
   
Genero *criarGenero(ListaPrincipal *lp, int id, const char nome[]) {
	// Cria um "nó gênero" dado um ID e um nome
	if (lp == NULL || nome == NULL) return NULL;

	if (buscarGenero(lp, id) != NULL) {
		mensagemDeErroFraca("Erro: já existe um artista com esse ID no sistema!\n");
		return NULL;
	}

	Genero* novo = (Genero*) malloc(sizeof(Genero));
	if (novo == NULL) {
		mensagemDeErroFraca("Erro de alocação de memória!\n");
		return NULL;
	}

	novo->id = id;
	copiarTexto(novo->nome, nome);
	novo->inicioArtistas = NULL;
	novo->fimArtistas = NULL;
	novo->qtdArtistas = 0;

	return novo;
}

int inserirGenero(ListaPrincipal* lp, Genero *g) {
	// Insere um gênero na lista prinicpal
	if (g == NULL) {
		mensagemDeErroFraca("Gênero inválido.\n");
		return 0;
	}
	
	g->ant = lp->fim;
	g->prox = NULL;

	if (listaGenerosEhVazia(lp)) {
		lp->inicio = g;
	} else {
		lp->fim->prox = g;
	}

	lp->fim = g;
	lp->qtdGeneros++;

	return 1;
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


//======================== Manipulação da lista de artistas (Fernando) ======================

static void exibirArtista(const Artista *a) {
	// Exibe no terminal todos os dados armazenados em um artista
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

Artista *buscaArtistaGenero(Genero *genero, int id) {
	// Busca um artista pelo ID dentro da lista de um gênero específico
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
	// Localiza um artista pelo ID em todos os gêneros do sistema
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

Artista *criarArtista(ListaPrincipal *lista, int id, const char nome[], int qtdIntegrantes, int qtdPremiacoes,
					  const char cidadeOrigem[], int estreia, int atividade, int encerramento,
					  const char principaisObras[]) {
	// Cria um "nó artista" dadas as informações para preencher seus campos
	Artista *novo = NULL;

	if (lista == NULL || nome == NULL || cidadeOrigem == NULL || principaisObras == NULL) {
		return NULL;
	}

	if (localizarArtistaGlobal(lista, id) != NULL) {
		mensagemDeErroFraca("Já existe um artista com esse ID no sistema.\n");
		return NULL;
	}

	novo = malloc(sizeof(Artista));
	if (novo == NULL) {
		mensagemDeErroFraca("Falha na alocação de memória.\n");
		return NULL;
	}

	copiarTexto(novo->nome, nome);
	copiarTexto(novo->cidadeOrigem, cidadeOrigem);
	copiarTexto(novo->principaisObras, principaisObras);
	novo->ant = NULL;
	novo->prox = NULL;
	novo->id = id;
	novo->qtdIntegrantes = qtdIntegrantes;
	novo->qtdPremiacoes = qtdPremiacoes;
	novo->estreia = estreia;
	novo->atividade = atividade;
	novo->encerramento = encerramento;
}

int inserirArtistaGenero(Artista *a, Genero *genero) {
	// Insere um artista no fim da lista de um determinado gênero
	if (a == NULL) {
		mensagemDeErroFraca("Artista inválido.\n");
		return 0;
	}

	a->ant = genero->fimArtistas;

	if (listaArtistasEhVazia(genero)) {
		genero->inicioArtistas = a;
	} else {
		genero->fimArtistas->prox = a;
	}

	genero->fimArtistas = a;
	genero->qtdArtistas++;

	return 1;
}

int alterarArtistaGenero(Genero *genero, int id, const char nome[], const char cidadeOrigem[],
						 const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia,
						 int atividade, int encerramento) {
	// Atualiza os dados do artista identificado pelo ID dentro de um gênero

	if (genero == NULL || nome == NULL || cidadeOrigem == NULL || principaisObras == NULL) {
		return 0;
	}

	Artista *artista = buscaArtistaGenero(genero, id);
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
	// Remove do gênero o artista indicado pelo ID e reajusta o encadeamento da lista
	if (genero == NULL) {
		return 0;
	}

	Artista *artista = buscaArtistaGenero(genero, id);
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
	// Percorre e exibe todos os artistas cadastrados em um gênero
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
	// Busca um artista pelo ID no sistema, exibe seus dados e retorna seu endereço
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
	// Exibe os artistas do sistema cuja quantidade de premiações atinge o mínimo
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

//==================== Funções de manipulação sobre a lista de artistas =====================


/* int totalDeArtistas (ListaPrincipal *l) {

/* } */


//=========================== Funções que imprimem informações ==============================

void exibirGenero (Genero *g) {
	// Imprime as informações de um gênero
	printf("===============================================\n");
	printf("Id do gênero: %d\n", g->id);
	printf("Nome do gênero: %s\n", g->nome);
	printf("Quantidade de artistas cadastrados do gênero: %d\n", g->qtdArtistas);
	printf("===============================================\n\n");
}

void exibirListaGeneros (ListaPrincipal *l) {
	// Imprime as informações de todos os gêneros cadastrados
	if (listaGenerosEhVazia(l)) {
		printf("Lista de gêneros vazia.\n");
		return;
	}

	printf("========= Lista de gêneros cadastrados ========\n\n");
	Genero *atual = l->inicio;
	while (atual != NULL) {
		exibirGenero(atual);
		atual = atual->prox;
	}
}

void exibirArtistasPorGenero(ListaPrincipal *lp) {
	// Exibe todos os artistas cadastrados, de gênero em gênero.
	if (listaGenerosEhVazia(lp)) {
		printf("Lista de gêneros vazia.\n");
		return;
	}

	printf("==== Exibindo todos os artistas cadastrados ====\n\n\n");
	
	Genero *atual = lp->inicio;
	while (atual != NULL) {
		printf("================ Gênero: %s ================\n\n", atual->nome);
		listarArtistasGenero(atual);
		atual = atual->prox;
	}
}

/* void imprimeRelatorioGeral (ListaPrincipal *l) { */
	
/* } */


//================================= Funções de remoção ======================================




//================================== Funções de busca =======================================




//========================== Consultas e funções de cruzamento ==============================




//================================= Impressão de menus ======================================




//===================== Funções de manipulação de arquivos (Vinícius) =======================

Genero *lerGenero (FILE *fp, ListaPrincipal *lp) {
	// Lê um gênero (de uma linha do arquivo de leitura) e o retorna
	char buffer[MAX_STRING];
	
	if (fgets(buffer, sizeof(buffer), fp) == NULL) return NULL; /* Se o fgets encontra EOF, então ela retorna NULL e a
																   função lerGenero também retorna um ponteiro nulo,
																   o que será a condição de parada da função
																   "carregarGeneros" */
	int idGenero;
	char nomeGenero[MAX_STRING];
	char *ponteiro;
	char *token = strtok_r(buffer, ";", &ponteiro); /* "token" recebe a primeira parte da linha do arquivo (antes do
													   ponto e vírgula), que é o ID do gênero da linha em questão */
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &idGenero); // Processa a string lida como um número (ID é um int) e atribui a "idGenero"

	token = strtok_r(NULL, ";", &ponteiro); /* "token" agora contém o nome do gênero, que vem depois do ponto e vírgula
											   (a segunda parte da linha) */
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	
	token[strcspn(token, "\r\n")] = '\0'; /* Essa linha pega a primeira quebra de linha no final da string e troca por
											 '\0' (termina a string automaticamente, ignorando '\r' ou '\n') */

	return criarGenero(lp, idGenero, token); // "token" já contém a string que é o nome do gênero
}

void carregarGeneros (FILE *fp, ListaPrincipal *l) {
	// Carrega todos os gêneros cadastrados em um arquivo ("generos.txt") para a lista principal
	int c;
	while ((c = fgetc(fp)) != '\n'); /* Essa parte (últimas duas linhas) só serve para pularmos a primeira linha, que é
										um cabeçalho */
	Genero *g;
	while ((g = lerGenero(fp, l)) != NULL) {
		inserirGenero(l, g);
	}
}

Artista *lerArtista (FILE *fp, ListaPrincipal *lp, int *idGeneroCorrespondente) {
	// Lê um artista (de uma linha do arquivo de leitura) e o retorna
	char buffer[MAX_STRING];
	
	if (fgets(buffer, sizeof(buffer), fp) == NULL) return NULL; /* Se o fgets encontra EOF, então ela retorna NULL e a
																   função lerGenero também retorna um ponteiro nulo,
																   o que será a condição de parada da função
																   "carregarArtistas" */
	int idArtista;
	char nomeArtista[MAX_STRING];
	int quantosIntegrantes;
	int quantosPremios;
	char cidadeNatal[MAX_STRING];
	int anoDeEstreia;
	int emAtividade;
	int anoUltimaObra;
	int idGenero;
	char melhoresObras[MAX_STRING];
	
	/* Nas linhas abaixo, vamos ler os campos para a criação do artista na ordem em que as variáveis foram declaradas
	   acima (primeiro o ID, depois o nome, depois a quantidade de intergrantes e prêmios, etc.) e armazená-los nas
	   variáveis para depois chamar a função criarArtista */

	char *ponteiro;
	char *token = strtok_r(buffer, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &idArtista);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	copiarTexto(nomeArtista, token);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &quantosIntegrantes);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &quantosPremios);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	copiarTexto(cidadeNatal, token);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &anoDeEstreia);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &emAtividade);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &anoUltimaObra);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	copiarTexto(melhoresObras, token);

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErroForte("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
							" campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
							" novamente.)");
	}
	sscanf(token, "%d", &idGenero);
	*idGeneroCorrespondente = idGenero;

	return criarArtista(lp, idArtista, nomeArtista, quantosIntegrantes, quantosPremios, cidadeNatal, anoDeEstreia,
						emAtividade, anoUltimaObra, melhoresObras);
}

void carregarArtistas (FILE *fp, ListaPrincipal *l) {
	// Carrega todos os artistas cadastrados em um arquivo ("artistas.txt") para a lista principal
	char c;
	while ((c = fgetc(fp)) != '\n'); /* Essa parte (últimas duas linhas) só serve para pularmos a primeira linha, que é
										um cabeçalho */
	int idGenero;
	Artista *a;
	Genero *g;
	while ((a = lerArtista(fp, l, &idGenero)) != NULL) {
		g = buscarGenero(l, idGenero);
		inserirArtistaGenero(a, g);
	}
}


//===========================================================================================
