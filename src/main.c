#include <stdio.h>
#include "lista.h"

int main () {

	FILE *f1, *f2;
	f1 = abreArquivoPraLer(f1, "../dados/generos.txt");
	f2 = abreArquivoPraLer(f2, "../dados/artistas.txt");
	ListaPrincipal *l = criarListaPrincipal();

	printf("\nSeja bem-vindo ao seu catálogo de artistas e gêneros musicais!\n\n");
	
	carregarGeneros(f1, l);
	printf("O catálogo pré-definido em generos.txt foi carregado com sucesso!\n");
	carregarArtistas(f2, l);
	printf("O catálogo pré-definido em artistas.txt foi carregado com sucesso!\n\n");

	Artista *a;
	Genero *g;
	int escolha = -1;
	int escolha2 = -1;
	int idRemovidoAlteradoExibido;

	int id;
	char nome[MAX_STRING];

	int idArtista;
	char nomeArtista[MAX_STRING];
	int qtdIntegrantes;
	int qtdPremios;
	char cidadeOrigem[MAX_STRING];
	int estreia;
	int atividade;
	int encerramento;
	char principaisObras[MAX_STRING];
	int idGenero;
	
	printf("O que você deseja fazer?\n");
	
	while (escolha != 0) {
		imprimirMenuPrincipal();
		scanf("%d", &escolha);
		
		switch (escolha) {
		case 1:
			escolherGeneroArtista();
			scanf("%d", &escolha2);

			switch (escolha2) {
			case 0: 
				preencherInformacoesGenero(&id, nome);
				g = criarGenero(l, id, nome);
				if (g == NULL) break;
				
				inserirGenero(l, g);
				break;

			case 1: {
				preencherInformacoesArtista(&idArtista, nomeArtista, &qtdIntegrantes, &qtdPremios, cidadeOrigem,
											&estreia, &atividade, &encerramento, principaisObras, &idGenero);

				if (idGenero == -1) break;

				g = buscarGenero(l, idGenero);
				if (g == NULL) {
					printf("\nNão existe um gênero com o ID informado. Caso você não se lembre do ID do gênero, pode "
						   "realizar a busca no próprio terminal. Tente novamente.\n");
					break;
				}

				a = criarArtista(l, idArtista, nomeArtista, qtdIntegrantes, qtdPremios, cidadeOrigem, estreia,
								 atividade, encerramento, principaisObras);
				if (a == NULL) break;

				inserirArtistaGenero(a, g);
				break;
			}

			default:
				printf("\nOpção inválida.\n");
				break;
			}
			
			break;

		case 2:
			imprimirMenuDeBuscas();
			scanf("%d", &escolha2);

			if (escolha2 == 1) {
				char generoBuscaID[MAX_STRING];
				while (getchar() != '\n');
				printf("\nDigite o nome do gênero do qual quer saber o ID: ");
				fscanf(stdin, "%199[^\n]", generoBuscaID);
				mostraIDdoGenero(l, generoBuscaID);
			} else if (escolha2 == 2) {
				char artistaBuscaID[MAX_STRING];
				while (getchar() != '\n');
				printf("\nDigite o nome do artista do qual quer saber o ID: ");
				fscanf(stdin, "%199[^\n]", artistaBuscaID);
				mostraIDdoArtista(l, artistaBuscaID);
			} else {
				printf("\nOpção inválida.\n");
			}
			
			break;

		case 3:
			escolherGeneroArtista();
			scanf("%d", &escolha2);

			switch (escolha2) {
			case 0:
				printf("\nDigite o ID do gênero que quer remover: ");
				scanf("%d", &idRemovidoAlteradoExibido);

				if (removerGenero(l, idRemovidoAlteradoExibido) == 0) {
					printf("\nID não encontrado.\n");
					break;
				}

				printf("\nGênero e seus respectivos artistas removidos com sucesso.\n");
				break;

			case 1:
				printf("\nDigite o ID do gênero correspondente ao artista: ");
				scanf("%d", &idRemovidoAlteradoExibido);

				g = buscarGenero(l, idRemovidoAlteradoExibido);
				if (g == NULL) {
					printf("\nGênero não encontrado.\n");
				}

				printf("Digite o ID do artista que quer remover: ");
				scanf("%d", &idRemovidoAlteradoExibido);

				a = localizarArtistaGlobal(l, idRemovidoAlteradoExibido);
				if (a == NULL) {
					printf("\nArtista não encontrado!\n");
					break;
				}

				if (removerArtistaGenero(g, idRemovidoAlteradoExibido) == 0) {
					printf("\nFalha na remoção do artista.\n");
					break;
				}
				printf("\nArtista removido com sucesso.\n");

				break;

			default:
				printf("\nOpção inválida.\n");
				break;
				
			}
			
			break;

		case 4:
			escolherGeneroArtista();
			scanf("%d", &escolha2);

			switch (escolha2) {
			case 0: {
				char nomeGenero[MAX_STRING];

				printf("\nDigite o ID do gênero que deseja alterar: ");
				scanf("%d", &idRemovidoAlteradoExibido);
				g = buscarGenero(l, idRemovidoAlteradoExibido);
				if (g == NULL) {
					printf("\nGênero não encontrado.\n");
					break;
				}

				while (getchar() != '\n');
				printf("Digite o novo nome do gênero: ");
				fscanf(stdin, "%199[^\n]", nomeGenero);

				if (alterarGenero(l, idRemovidoAlteradoExibido, nomeGenero) == 0)
					printf("\nFalha na alteração do gênero.\n");
				
				break;
			}	

			case 1: {
				printf("\nDigite o ID do gênero correspondente ao artista que quer alterar: ");
				scanf("%d", &idRemovidoAlteradoExibido);
				
				g = buscarGenero(l, idRemovidoAlteradoExibido);
				if (g == NULL) {
					printf("\nGênero não encontrado.\n");
					break;
				}

				preencherInformacoesArtista(&idArtista, nomeArtista, &qtdIntegrantes, &qtdPremios, cidadeOrigem,
											&estreia, &atividade, &encerramento, principaisObras, &idGenero);
				if (idGenero == -1) break;

				if (alterarArtistaGenero(g, idArtista, nomeArtista, cidadeOrigem, principaisObras, qtdIntegrantes,
										 qtdPremios, estreia, atividade, encerramento) == 0) {
					printf("\nErro na alteração do artista (não encontrado).\n");
					break;
				}
				
				break;

			}

			default:
				printf("\nOpção inválida.\n");
				break;
				
			}

			break;

		case 5:
			printf("\n1 - Exibir um gênero por ID\n2 - Exibir todos os gêneros\n3 - Exibir um artista por ID\n4 - "
				   "Exibir todos os artistas de um gênero\n5 - Exibir todos os artistas, por gênero\n\n");
			scanf("%d", &escolha2);
			
			switch (escolha2) {
			case 1:
				printf("\nDigite o ID do gênero: ");
				scanf("%d", &idRemovidoAlteradoExibido);
				g = buscarGenero(l, idRemovidoAlteradoExibido);

				if (g == NULL) {
					printf("\nGênero não encontrado.\n");
					break;
				}
				
				exibirGenero(g);
				break;

			case 2:
				exibirListaGeneros(l);
				break;

			case 3: 
				printf("\nDigite o ID do artista: ");
				scanf("%d", &idRemovidoAlteradoExibido);
				a = localizarArtistaGlobal(l, idRemovidoAlteradoExibido);
				if (a == NULL) {
					printf("\nArtista não encontrado.\n");
					break;
				}
				exibirArtista(a);
				break;

			case 4:
				printf("\nDigite o ID do gênero: ");
				scanf("%d", &idRemovidoAlteradoExibido);
				g = buscarGenero(l, idRemovidoAlteradoExibido);
				if (g == NULL) {
					printf("\nGênero não encontrado.\n");
					break;
				}
				listarArtistasGenero(g);
				break;

			case 5:
				exibirArtistasPorGenero(l);
				break;

			}

			break;

		case 6:
			printf("\n1 - Filtrar artistas por um número mínimo de premiações (Grammys)\n2 - Encontrar o gênero com o "
				   "menor número de artistas (em caso de empate, consideramos o que vier primeiro na lista)\n\n");
			scanf("%d", &escolha2);

			switch (escolha2) {
			case 1: {
				int quantosGrammys;
				printf("\nPor quantos Grammys vamos filtrar? ");
				scanf("%d", &quantosGrammys);
				filtrarArtistasPremiacoes(l, quantosGrammys);
				break;
			}
				
			case 2:
				menorNumeroDeArtistas(l);
				break;
			
			}
			
			break;

		case 0:
			break;

		default:
			printf("\nOpção inválida.\n");
			break;

		}
		
	}

	destruirListaPrincipal(l);
	
	return 0;
}
