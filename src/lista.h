#ifndef LISTA_H
#define LISTA_H

#define MAX_STRING 100

// Tipos opacos para encapsulamento das estruturas
typedef struct artista Artista;     
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

// Gerenciamento da Lista Principal
ListaPrincipal* criarListaPrincipal(void);
void destruirListaPrincipal(ListaPrincipal* lp);

// Operações CRUD de Gêneros
void inserirGenero(ListaPrincipal* lp, int id, const char* nome);
Genero* buscarGenero(ListaPrincipal* lp, int id);
int alterarGenero(ListaPrincipal* lp, int id, const char* novoNome);
int removerGenero(ListaPrincipal* lp, int id);

// Relatórios e Consultas
void listarGeneros(const ListaPrincipal* lp);
int contarGeneros(const ListaPrincipal* lp);
void contabilizarArtistasPorGenero(const ListaPrincipal* lp);

#endif