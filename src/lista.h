#ifndef LISTA_H
#define LISTA_H
#define MAX_STRING 100

typedef struct artista Artista;
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

ListaPrincipal* criarListaPrincipal(void);
void destruirListaPrincipal(ListaPrincipal* lp);
void inserirGenero(ListaPrincipal* lp, int id, const char* nome);
Genero* buscarGenero(ListaPrincipal* lp, int id);

#endif