#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

int inserirArtistaGenero (Genero *genero, int id, char nome[], char cidadeOrigem[], char periodoAtuacao[], int qtdIntegrantes, int qtdPremiacoes) {
  if(genero == NULL) {
    return 0;
  }

  if (buscaArtistaGenero(genero, id) != NULL) {
    printf("Artista já presente na lista\n");
    return 0;
  }

  Artista *novo = malloc(sizeof(Artista));

  if(novo == NULL) {
    printf("Falha na alocação de memória\n");
    return 0;
  }

  novo->id = id;
  snprintf(novo->nome, MAX_STRING, "%s", nome);
  snprintf(novo->cidadeOrigem, MAX_STRING, "%s", cidadeOrigem);
  snprintf(novo->periodoAtuacao, MAX_STRING, "%s", periodoAtuacao);
  novo->qtdIntegrantes = qtdIntegrantes;
  novo->qtdPremiacoes = qtdPremiacoes;
  novo->proximo = NULL;

  if(genero->inicioArtistas == NULL) {
    novo->anterior = NULL;
    genero->inicioArtistas = novo;
    genero->fimArtistas = novo;
  } else {
    novo->anterior = genero->fimArtistas;
    genero->fimArtistas->proximo = novo;
    genero->fimArtistas = novo;
  }
  
  genero->qtdArtistas++;

  return 1;

}

Artista *buscaArtistaGenero (Genero *genero, int id) {
  if(genero == NULL) {
    return NULL;
  }
  
  Artista *aux = genero->inicioArtistas;

  while(aux != NULL) {
    if(aux->id == id) {
      return aux;
    }

    aux = aux->proximo;
  }

  return NULL;
}