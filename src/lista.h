#ifndef LISTA_H
#define LISTA_H
#define MAX_STRING 200

typedef struct artista Artista;
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

//================================= Funções do Orlando ======================================

ListaPrincipal* criarListaPrincipal(void);
void destruirListaPrincipal(ListaPrincipal* lp);
int inserirGenero(ListaPrincipal* lp, Genero *g);
Genero* buscarGenero(ListaPrincipal* lp, int id);

//================================= Funções do Fernando ====================================

Artista *buscaArtistaGenero(Genero *genero, int id);

int inserirArtistaGenero(Artista *a, Genero *genero);

int alterarArtistaGenero(Genero *genero, int id, const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia, int atividade,
                         int encerramento);

int removerArtistaGenero(Genero *genero, int id);
void listarArtistasGenero(Genero *genero);
Artista *buscaGlobalArtista(ListaPrincipal *lista, int id);
void filtrarArtistasPremiacoes(ListaPrincipal *lista, int minimoPremiacoes);

//================================= Funções do Vinícius ====================================

FILE *abreArquivoPraLer(FILE *, const char[]);
void carregarGeneros(FILE *, ListaPrincipal *);
void carregarArtistas(FILE *, ListaPrincipal *);
void exibirListaGeneros(ListaPrincipal *);
void exibirArtistasPorGenero(ListaPrincipal *);

#endif
