#include <stdio.h>
#include "lista.h"

int main () {

	FILE *f1, *f2;
	f1 = abreArquivoPraLer(f1, "../dados/generos.txt");
	f2 = abreArquivoPraLer(f2, "../dados/artistas.txt");
	ListaPrincipal *l = criarListaPrincipal();

	printf("Seja bem-vindo ao seu catálogo de artistas e gêneros musicais!\n\n");
	
	carregarGeneros(f1, l);
	carregarArtistas(f2, l);
	
	exibirListaGeneros(l);
	printf("\n");
	exibirArtistasPorGenero(l);

	destruirListaPrincipal(l);

	return 0;
}
