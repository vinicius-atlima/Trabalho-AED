#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lista.h"

//=========================================== Definições das estruturas ================================================

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

//================================================ Funções miscelâneas =================================================
 
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

void mensagemDeErro(const char mensagem[]) {
	// Imprime uma mensagem de erro e fecha o programa imediatamente
	printf("ERRO: %s", mensagem);
	getchar();
	exit(EXIT_FAILURE);
}

FILE *abreArquivoPraLer (FILE *fp, const char nome[]) {
	// Abre o arquivo no modo "read" e checa se houve erro; retorna o ponteiro para o arquivo
	fp = fopen(nome, "r");
	if (fp == NULL) mensagemDeErro("Erro na abertura do arquivo. Rode o programa novamente.");
	return fp;
}


//=================================== Manipulação da lista de gêneros (Orlando) ========================================

// -> Criação/destruição da lista principal

ListaPrincipal* criarListaPrincipal(void) {
	// Inicia a lista principal (de gêneros)
	ListaPrincipal* lp = (ListaPrincipal*) malloc(sizeof(ListaPrincipal));

	if (lp == NULL) {
		mensagemDeErro("Erro de memória. Rode o programa novamente.");
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
		removerGenero(lp, atual->id);

		atual = proxGenero;
	}

	free(lp);
}


// -> Criação/inserção/alteração/remoção de "nós-gênero"
   
Genero *criarGenero(ListaPrincipal *lp, int id, const char nome[]) {
	// Cria um "nó gênero" dado um ID e um nome
	if (lp == NULL || nome == NULL) return NULL;

	if (buscarGenero(lp, id) != NULL) {
		printf("\nJá existe um gênero com esse ID no sistema!\n");
		return NULL;
	}

	Genero* novo = (Genero*) malloc(sizeof(Genero));
	if (novo == NULL) {
		printf("\nErro de alocação de memória!\n");
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
		printf("Gênero inválido.\n");
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

int alterarGenero(ListaPrincipal* lp, int id, const char novoNome[]) {
    if (lp == NULL || novoNome == NULL) return 0;

    // Utiliza a função de busca para reaproveitar o ponteiro do nó
    Genero* g = buscarGenero(lp, id);
    if (g == NULL) return 0; // Retorna 0 (falha) se o gênero não existir

    // Atualiza com segurança a string do nome
	copiarTexto(g->nome, novoNome);

    return 1; // Retorna 1 para sinalizar sucesso
}

int removerGenero(ListaPrincipal* lp, int id) {
	// Desconecta o nó da lista duplamente encadeada e libera sua memória e a de seus artistas.
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


// -> Busca gênero (por ID)

Genero* buscarGenero(ListaPrincipal* lp, int id) {
	// Busca e retorna o ponteiro para um nó gênero; em caso de falha, retorna NULL
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


// -> Exibem informações dos gêneros

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


//==================================== Manipulação da lista de artistas (Fernando) =====================================

// -> Criação, inserção, alteração e remoção de artistas

Artista *criarArtista(ListaPrincipal *lista, int id, const char nome[], int qtdIntegrantes, int qtdPremiacoes,
					  const char cidadeOrigem[], int estreia, int atividade, int encerramento,
					  const char principaisObras[]) {
	// Cria um "nó artista" dadas as informações para preencher seus campos
	Artista *novo = NULL;

	if (lista == NULL || nome == NULL || cidadeOrigem == NULL || principaisObras == NULL) {
		return NULL;
	}

	if (localizarArtistaGlobal(lista, id) != NULL) {
		printf("Já existe um artista com esse ID no sistema.\n");
		return NULL;
	}

	novo = malloc(sizeof(Artista));
	if (novo == NULL) {
		printf("Falha na alocação de memória.\n");
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

	return novo;
}

int inserirArtistaGenero(Artista *a, Genero *genero) {
	// Insere um artista no fim da lista de um determinado gênero
	if (a == NULL) {
		printf("Artista inválido.\n");
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


// -> Busca por artista (dentro de gênero/globalmente)

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

Artista *localizarArtistaGlobal(ListaPrincipal *lista, int id) {
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


// -> Exibem informações dos artistas, por artista, por lista (gênero) ou globalmente

void exibirArtista(const Artista *a) {
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

void exibirArtistasPorGenero(ListaPrincipal *lp) {
	// Exibe todos os artistas cadastrados, de gênero em gênero.
	if (listaGenerosEhVazia(lp)) {
		printf("\nLista de gêneros vazia.\n");
		return;
	}

	printf("\n==== Exibindo todos os artistas cadastrados ====\n\n\n");
	
	Genero *atual = lp->inicio;
	while (atual != NULL) {
		printf("================ Gênero: %s ================\n\n", atual->nome);

		if (listaArtistasEhVazia(atual)) {
			printf("===============================================\n");
			printf("Lista de artistas vazias para este gênero.\n");
			printf("===============================================\n\n");
			atual = atual->prox;
			continue;
		}
		
		Artista *aAtual = atual->inicioArtistas;
		while (aAtual != NULL) {
			exibirArtista(aAtual);
			aAtual = aAtual->prox;
		}
		
		atual = atual->prox;
	}
}


// -> Filtra artistas por uma quantidade mínima de premiações

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
				printf("Gênero: %s\n", auxGenero->nome);
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

// -> Encontra o gênero com o menor número de artistas

void menorNumeroDeArtistas(ListaPrincipal *l) {
	// Retorna o gênero com o menor número de artistas; em caso de empate, retorna o que aparecer primeiro na lista
	if (listaGenerosEhVazia(l)) {
		printf("Lista de gêneros vazia.\n");
		return;
	}

	Genero *menorNumero = l->inicio;
	Genero *atual = menorNumero->prox;
	while (atual != NULL) {
		if (atual->qtdArtistas < menorNumero->qtdArtistas) {
			menorNumero = atual;
		}
		atual = atual->prox;
	}
	
	printf("\nO gênero com o menor número de artistas é o %s! Ele tem %d artistas cadastrados.\n", menorNumero->nome,
		   menorNumero->qtdArtistas);
}


//=================================== Funções de manipulação de arquivos (Vinícius) ====================================

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
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &idGenero); // Processa a string lida como um número (ID é um int) e atribui a "idGenero"

	token = strtok_r(NULL, ";", &ponteiro); /* "token" agora contém o nome do gênero, que vem depois do ponto e vírgula
											   (a segunda parte da linha) */
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
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
		inserirGenero(l, g); // Insere o gênero na lista principal
	}
}

Artista *lerArtista (FILE *fp, ListaPrincipal *lp, int *idGeneroCorrespondente) {
	/* Lê um artista (de uma linha do arquivo de leitura) e o retorna; armazena o ID do seu gênero no ponteiro
	   idGeneroCorrespondente */
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
	char melhoresObras[MAX_STRING];
	int idGenero;
	
	/* Nas linhas abaixo, vamos ler os campos para a criação do artista na ordem em que as variáveis foram declaradas
	   acima (primeiro o ID, depois o nome, depois a quantidade de intergrantes e prêmios, etc.) e armazená-los nas
	   variáveis criadas acima para depois chamar a função criarArtista */

	char *ponteiro;
	char *token = strtok_r(buffer, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &idArtista); // Lê e preenche o ID do artista da linha em questão

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	copiarTexto(nomeArtista, token); // Lê e preenche o nome do artista da linha em questão

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &quantosIntegrantes); /* Lê e preenche quantos integrantes tem a banda, ou se é um artista solo
												 na linha em questão */

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &quantosPremios); // Lê e preenche quantos prêmios (Grammys) tem o artista/banda

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	copiarTexto(cidadeNatal, token); // Lê e preenche o cidade de origem do artista/banda da linha em questão

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &anoDeEstreia); /* Lê e preenche o ano de lançamento da primeira obra do artista da linha em
										   questão */

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &emAtividade); /* Lê e preenche com o valor 0 caso o artista não esteja em atividade e 1 caso
										  contrário */

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &anoUltimaObra); /* Lê e preenche com o ano de lançamento da última obra/encerramento das
											atividades */

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	copiarTexto(melhoresObras, token); // Lê e preenche com as principais obras do artista

	token = strtok_r(NULL, ";", &ponteiro);
	if (token == NULL) {
		mensagemDeErro("Erro na leitura do arquivo: formato não suportado. Talvez você não tenha preenchido um dos"
					   " campos corretamente, use o cabeçalho como referência! (Faça a alteração e rode o programa"
					   " novamente.)");
	}
	sscanf(token, "%d", &idGenero);
	*idGeneroCorrespondente = idGenero; /* Altera o conteúdo do ponteiro idGeneroCorrespondente para podermos passar o
										   ID do gênero correspondente ao artista para a função carregarArtistas */

	return criarArtista(lp, idArtista, nomeArtista, quantosIntegrantes, quantosPremios, cidadeNatal, anoDeEstreia,
						emAtividade, anoUltimaObra, melhoresObras);
}

void carregarArtistas (FILE *fp, ListaPrincipal *l) {
	// Carrega todos os artistas cadastrados em um arquivo ("artistas.txt") para a lista principal
	int c;
	while ((c = fgetc(fp)) != '\n'); /* Essa parte (últimas duas linhas) só serve para pularmos a primeira linha, que é
										um cabeçalho */
	int idGenero;
	Artista *a;
	Genero *g;
	while ((a = lerArtista(fp, l, &idGenero)) != NULL) {
		g = buscarGenero(l, idGenero); // Buscamos o gênero correto para inserir o artista
		if (g == NULL) {
			printf("Aviso: existe um artista em artistas.txt com um gênero inválido (não existe ID correspondente ao"
				   "gênero.)\n");
			continue;
		}
		inserirArtistaGenero(a, g); /* Inserimos o artista no gênero e repetimos o loop até encontrar EOF que retorna
									   NULL */
	}
}

//================================= Impressão de menus e interação com o usuário ======================================

void imprimirMenuPrincipal() {
	// Imprime o menu prinicipal do sistema
	printf("\n1 - Adicionar um gênero/artista\n2 - Buscar pelo ID de um gênero/artista\n3 - Remover um gênero/artista\n"
		   "4 - Alterar os dados de um gênero/artista\n5 - Exibir dados de gêneros/artistas\n6 - Realizar uma consulta "
		   "\n0 - Encerrar o programa\n\n");
}

void escolherGeneroArtista() {
	// Imprime o menu secundário do sistema (escolha entre gênero ou artista)
	printf("\nDigite \"0\" e depois \"enter\" se você quer gênero ou \"1\" e depois \"enter\" se quer artista.\n\n");
}

void preencherInformacoesGenero(int *id, char nome[]) {
	// Coleta as informações para formar um gênero
	printf("Digite um ID para o gênero: ");
	scanf("%d", id);
	while (getchar() != '\n');

	printf("Digite o nome do gênero: ");
	fscanf(stdin, "%199[^\n]", nome);
}

void preencherInformacoesArtista(int *idArtista, char nomeArtista[], int *qtdIntegrantes, int *qtdPremiacoes,
								 char cidadeOrigem[], int *estreia, int *atividade, int *encerramento,
								 char principaisObras[], int *idGeneroCorrespondente) {
	// Coleta as informações para formar um artista
	printf("\nDigite um ID para o artista/banda: ");
	scanf("%d", idArtista);
	while (getchar() != '\n');

	printf("Digite o nome do artista/banda: ");
	fscanf(stdin, "%199[^\n]", nomeArtista);
	while (getchar() != '\n');

	printf("Digite quantos integrantes tem a banda (1 para artistas solo): ");
	scanf("%d", qtdIntegrantes);
	while (getchar() != '\n');

	printf("Digite quantos Grammmys tem o artista/banda (quantidade de prêmios): ");
	scanf("%d", qtdPremiacoes);
	while (getchar() != '\n');

	printf("Digite a cidade de origem do artista/grupo: ");
	fscanf(stdin, "%199[^\n]", cidadeOrigem);
	while (getchar() != '\n');

	printf("Digite o ano de lançamento da primeira obra do artista/grupo: ");
	scanf("%d", estreia);
	while (getchar() != '\n');

	printf("Digite 0 caso o grupo/artista não está mais em ativiade/falecido(s). Digite 1 caso ainda estejam ativos: ");
	scanf("%d", atividade);
	while (getchar() != '\n');

	printf("Digite o ano de lançamento do(a) último(a) música/álbum: ");
	scanf("%d", encerramento);
	while (getchar() != '\n');

	printf("Digite algumas músicas/álbuns notáveis desse artista (use | como separador): ");
	fscanf(stdin, "%199[^\n]", principaisObras);
	while (getchar() != '\n');

	printf("Digite o ID do gênero correspondente a esse artista. Digite -1 caso você queira voltar para consultar: ");
	scanf("%d", idGeneroCorrespondente);
}

void imprimirMenuDeBuscas() {
	printf("\n1 - Buscar pelo ID de um gênero\n2 - Buscar pelo ID de um artista\n\n");
}

void mostraIDdoGenero(ListaPrincipal *l, const char nome[]) {
	if (listaGenerosEhVazia(l)) {
		printf("\nLista de gêneros vazia.\n");
		return;
	}
	
	Genero *atual = l->inicio;
	while (atual != NULL && strcmp(atual->nome, nome) != 0) {
		atual = atual->prox;
	}
	
	if (atual == NULL) {
		printf("\nGênero não encontrado. Adicione-o!\n");
		return;
	}
	
	printf("\nO ID do gênero %s é %d.\n", nome, atual->id);
}

void mostraIDdoArtista(ListaPrincipal *l, const char nome[]) {
	if (listaGenerosEhVazia(l)) {
		printf("\nLista de gêneros vazia.\n");
		return;
	}
	
	Genero *atual = l->inicio;
	Artista *aAtual;
	
	while (atual != NULL) {
		if (listaArtistasEhVazia(atual)) continue;

		aAtual = atual->inicioArtistas;
		while (aAtual != NULL && strcmp(aAtual->nome, nome) != 0) {
			aAtual = aAtual->prox;
		}

		if (aAtual == NULL) atual = atual->prox;
		else break;
	}
	
	if (aAtual == NULL) {
		printf("\nArtista não encontrado. Adicione-o!\n");
		return;
	}
	
	printf("\nO ID do artista %s é %d.\n", nome, aAtual->id);
}


//=====================================================================================================================
