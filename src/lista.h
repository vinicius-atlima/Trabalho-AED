#ifndef LISTA_H
#define LISTA_H

#define MAX_STRING 100

// Declaração opaca dos tipos para encapsulamento das estruturas
typedef struct artista Artista;     
typedef struct genero Genero;
typedef struct listaPrincipal ListaPrincipal;

// ---------------- Protótipos das Funções da Lista Principal ----------------

// Cria e aloca dinamicamente a estrutura gerenciadora da lista de gêneros
ListaPrincipal* criarListaPrincipal(void);

// Desaloca toda a memória da lista (tanto gêneros quanto seus artistas)
void destruirListaPrincipal(ListaPrincipal* lp);

// Insere um novo gênero no final da lista principal (retorno void)
void inserirGenero(ListaPrincipal* lp, int id, const char* nome);

// Retorna a referência (ponteiro) do gênero pelo ID ou NULL se não encontrar
Genero* buscarGenero(ListaPrincipal* lp, int id);

// Atualiza o nome de um gênero existente (retorna 1 para sucesso e 0 para falha)
int alterarGenero(ListaPrincipal* lp, int id, const char* novoNome);

// Remove o gênero da lista e libera seus artistas associados (retorna 1 ou 0)
int removerGenero(ListaPrincipal* lp, int id);

#endif