#ifndef LISTA_H
#define LISTA_H
#define MAX_STRING 100

typedef struct artista Artista;
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

Artista *buscaArtistaGenero(Genero *genero, int id);

int inserirArtistaGenero(ListaPrincipal *lista, Genero *genero, int id, const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia, int atividade,
                         int encerramento);

int alterarArtistaGenero(Genero *genero, int id, const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia, int atividade,
                         int encerramento);

int removerArtistaGenero(Genero *genero, int id);
void listarArtistasGenero(Genero *genero);
Artista *buscaGlobalArtista(ListaPrincipal *lista, int id);
void filtrarArtistasPremiacoes(ListaPrincipal *lista, int minimoPremiacoes);

#endif
