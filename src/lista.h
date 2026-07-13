#ifndef LISTA_H
#define LISTA_H
#define MAX_STRING 100

//Estrutura lista secundaria (artista / banda)
typedef struct Artista {
    int id;
    char nome[MAX_STRING];
    char cidadeOrigem[MAX_STRING];
    char periodoAtuacao[MAX_STRING];
    int qtdIntegrantes;
    int qtdPremiacoes;

    struct Artista* anterior;
    struct Artista* proximo;
} Artista;

//Estrutura lista principal (genero)
typedef struct Genero {
    int id;
    char nomeGenero[MAX_STRING];

    Artista* inicioArtistas;
    Artista* fimArtistas;
    int qtdArtistas;

    struct Genero* anterior;
    struct Genero* proximo;
} Genero;

typedef struct {
    Genero* inicio;
    Genero* fim;
    int qtdGeneros;
} ListaPrincipal;

#endif