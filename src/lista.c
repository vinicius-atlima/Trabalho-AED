#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lista.h"

struct artista {
  char nome[MAX_STRING];
  char cidadeOrigem[MAX_STRING];
  char principaisObras[MAX_STRING];

  struct artista *ant;
  struct artista *prox;

  int id;
  int qtdIntegrantes;
  int qtdPremiacoes;
  int estreia;
  int atividade;
  int encerramento;
};

struct genero {
  char nome[MAX_STRING];

  Artista *inicioArtistas;
  Artista *fimArtistas;

  struct genero *ant;
  struct genero *prox;

  int id;
  int qtdArtistas;
};

struct listaPrincipal {
  struct genero *inicio;
  struct genero *fim;
  int qtdGeneros;
};

// Copia uma string para um campo de tamanho limitado, garantindo o terminador.
static void copiarTexto(char destino[MAX_STRING], const char origem[]) {
  snprintf(destino, MAX_STRING, "%s", origem);
}

// Exibe no terminal todos os dados armazenados em um artista.
static void exibirArtista(const Artista *artista) {
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

// Busca um artista pelo ID dentro da lista de um gênero específico.
Artista *buscaArtistaGenero(Genero *genero, int id) {
  Artista *aux;

  if (genero == NULL) {
    return NULL;
  }

  aux = genero->inicioArtistas;
  while (aux != NULL) {
    if (aux->id == id) {
      return aux;
    }
    aux = aux->prox;
  }

  return NULL;
}

// Localiza um artista pelo ID em todos os gêneros do sistema.
static Artista *localizarArtistaGlobal(ListaPrincipal *lista, int id) {
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

// Cria e insere um artista no fim da lista do gênero, exigindo ID global único.
int inserirArtistaGenero(ListaPrincipal *lista, Genero *genero, int id,
                         const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes,
                         int qtdPremiacoes, int estreia, int atividade,
                         int encerramento) {
  Artista *novo;

  if (lista == NULL || genero == NULL || nome == NULL || cidadeOrigem == NULL ||
      principaisObras == NULL) {
    return 0;
  }

  if (localizarArtistaGlobal(lista, id) != NULL) {
    printf("Ja existe um artista com esse ID no sistema\n");
    return 0;
  }

  novo = malloc(sizeof(Artista));
  if (novo == NULL) {
    printf("Falha na alocacao de memoria\n");
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

// Atualiza os dados do artista identificado pelo ID dentro de um gênero.
int alterarArtistaGenero(Genero *genero, int id, const char nome[],
                         const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes,
                         int qtdPremiacoes, int estreia, int atividade,
                         int encerramento) {
  Artista *artista;

  if (genero == NULL || nome == NULL || cidadeOrigem == NULL ||
      principaisObras == NULL) {
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

// Remove do gênero o artista indicado pelo ID e reajusta o encadeamento da lista.
int removerArtistaGenero(Genero *genero, int id) {
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

// Percorre e exibe todos os artistas cadastrados em um gênero.
void listarArtistasGenero(Genero *genero) {
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

// Busca um artista pelo ID no sistema, exibe seus dados e retorna seu endereço.
Artista *buscaGlobalArtista(ListaPrincipal *lista, int id) {
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

// Exibe os artistas do sistema cuja quantidade de premiações atinge o mínimo.
void filtrarArtistasPremiacoes(ListaPrincipal *lista, int minimoPremiacoes) {
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
