#ifndef LISTA_H
#define LISTA_H
#define MAX_STRING 200

typedef struct artista Artista;
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

//================================= Funções do Orlando ======================================

ListaPrincipal* criarListaPrincipal(void);
void destruirListaPrincipal(ListaPrincipal* lp);

Genero *criarGenero(ListaPrincipal *, int, const char[]);
int inserirGenero(ListaPrincipal* lp, Genero *g);
Genero* buscarGenero(ListaPrincipal* lp, int id); 
int removerGenero(ListaPrincipal* lp, int id);
int alterarGenero(ListaPrincipal *, int, const char[]);

void exibirGenero(Genero *);
void exibirListaGeneros(ListaPrincipal *);

//================================= Funções do Fernando ====================================

Artista *criarArtista(ListaPrincipal *l, int id, const char nome[], int qtdIntegrantes, int qtdPremiacoes,
					  const char cidadeOrigem[], int estreia, int atividade, int encerramento,
					  const char principaisObras[]);

int inserirArtistaGenero(Artista *a, Genero *genero);

Artista *buscaArtistaGenero(Genero *genero, int id);

int alterarArtistaGenero(Genero *genero, int id, const char nome[], const char cidadeOrigem[],
                         const char principaisObras[], int qtdIntegrantes, int qtdPremiacoes, int estreia,
						 int atividade, int encerramento);

int removerArtistaGenero(Genero *genero, int id);
void listarArtistasGenero(Genero *genero);
void exibirArtista(const Artista *a);
Artista *localizarArtistaGlobal(ListaPrincipal *lista, int id);
void filtrarArtistasPremiacoes(ListaPrincipal *lista, int minimoPremiacoes);

//================================= Funções do Vinícius ====================================

FILE *abreArquivoPraLer(FILE *, const char[]);
void carregarGeneros(FILE *, ListaPrincipal *);
void carregarArtistas(FILE *, ListaPrincipal *);
void exibirListaGeneros(ListaPrincipal *);
void exibirArtistasPorGenero(ListaPrincipal *);

void imprimirMenuPrincipal();
void escolherGeneroArtista();
void preencherInformacoesGenero(int *, char[]);
void preencherInformacoesArtista(int *idArtista, char nomeArtista[], int *qtdIntegrantes, int *qtdPremios,
								 char cidadeOrigem[], int *estreia, int *atividade, int *encerramento,
								 char principaisObras[], int *idGenero);
void imprimirMenuDeBuscas();
void mostraIDdoGenero(ListaPrincipal *, const char[]);
void mostraIDdoArtista(ListaPrincipal *, const char[]);

void menorNumeroDeArtistas(ListaPrincipal *);

#endif
