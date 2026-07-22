# Trabalho Final de AED — Tema 8: Música

Sistema de linha de comando desenvolvido em **C** para o trabalho final da disciplina **FACOM31201 — Algoritmos e Estruturas de Dados I**. O projeto gerencia gêneros musicais e seus respectivos artistas ou bandas por meio de TADs (Tipos Abstratos de Dados) e listas duplamente encadeadas.

## Grupo

- Fernando Rodrigues Costa
- Orlando Luiz de Carvalho Topam
- Vinícius Albino Torres Lima

## Objetivo

Aplicar os conceitos estudados na disciplina na construção de um sistema CLI capaz de cadastrar, consultar, alterar, remover, listar e carregar dados musicais. A solução utiliza duas listas relacionadas:

- **Lista principal — Gêneros Musicais:** armazena os gêneros cadastrados.
- **Lista secundária — Artistas/Bandas:** cada gênero mantém sua própria lista de artistas ou bandas.

As duas listas são duplamente encadeadas, permitindo percorrer os elementos nos dois sentidos. Cada gênero mantém referências para o início e o fim de sua lista de artistas, além da quantidade de artistas associados.

## Modelo de dados

### Gênero musical

- Identificador numérico (`id`)
- Nome do gênero
- Início e fim da lista de artistas
- Quantidade de artistas
- Ponteiros para o gênero anterior e o próximo

### Artista ou banda

- Identificador numérico (`id`)
- Nome
- Cidade de origem
- Período de atuação
- Quantidade de integrantes
- Quantidade de premiações
- Ponteiros para o artista anterior e o próximo

## Funcionalidades obrigatórias

- [ ] Inicializar a lista principal de gêneros.
- [ ] Inserir, buscar, alterar e remover gêneros musicais.
- [ ] Inserir, buscar, alterar e remover artistas ou bandas dentro de um gênero.
- [ ] Listar todos os gêneros cadastrados.
- [ ] Listar os artistas de um gênero específico.
- [ ] Exibir a quantidade total de gêneros.
- [ ] Carregar gêneros e artistas em lote a partir de arquivos de texto.
- [ ] Validar identificadores, dados de entrada e vínculos entre artistas e gêneros.
- [ ] Liberar toda a memória alocada dinamicamente antes de encerrar o programa.
- [ ] Disponibilizar as operações e consultas por meio de um menu no terminal.

## Consultas obrigatórias planejadas

- [ ] Contabilizar quantos artistas existem em cada gênero.
- [ ] Buscar um artista globalmente em todos os gêneros.
- [ ] Filtrar artistas por uma quantidade mínima de premiações.
- [ ] Gerar um relatório sintetizado com todos os gêneros e seus respectivos artistas.
- [ ] Encontrar o gênero com a menor quantidade de artistas.

Em caso de empate na consulta do menor gênero, o sistema deverá apresentar todos os gêneros que possuam a menor quantidade de artistas.

## Divisão de tarefas

### Orlando Luiz de Carvalho Topam — Lista principal: Gêneros

Responsável pelo TAD da lista principal e pelas operações que mantêm os gêneros musicais. Também deverá integrar cada gênero à sua lista secundária e garantir que os contadores permaneçam consistentes após inserções e remoções.

- [ ] Criar a função de inicialização da lista principal.
- [ ] Implementar a inserção de gêneros.
- [ ] Implementar a busca de gêneros.
- [ ] Implementar a alteração de gêneros.
- [ ] Implementar a remoção de gêneros.
- [ ] Implementar a listagem simples de gêneros.
- [ ] Implementar a contagem total de gêneros.
- [ ] Implementar a consulta que contabiliza os artistas de cada gênero.
- [ ] Produzir a seção **Introdução** do relatório.

### Fernando Rodrigues Costa — Lista secundária: Artistas/Bandas

Responsável pelas operações da lista de artistas mantida dentro de cada gênero. As funções devem preservar o encadeamento duplo, atualizar a quantidade de artistas do gênero e permitir consultas locais e globais.

- [x] Implementar a busca de artistas dentro de um gênero.
- [x] Implementar a inserção de artistas dentro de um gênero.
- [ ] Implementar a alteração de artistas.
- [ ] Implementar a remoção de artistas.
- [ ] Implementar a listagem dos artistas de um gênero específico.
- [ ] Implementar a busca global de um artista em todos os gêneros.
- [ ] Implementar o filtro por quantidade mínima de premiações.
- [ ] Produzir a seção **Documentação do Código** do relatório.

### Vinícius Albino Torres Lima — Arquivos e sistema

Responsável pela persistência de entrada, pelo encerramento seguro do sistema e pelas consultas gerais. A carga em lote deverá tratar arquivos inválidos e impedir que artistas sejam associados a gêneros inexistentes.

- [ ] Implementar a leitura e a carga de dados em lote a partir de arquivos de texto.
- [ ] Implementar a liberação de memória de todas as listas.
- [ ] Implementar o relatório sintetizado de gêneros e artistas.
- [ ] Implementar a consulta do gênero com a menor quantidade de artistas.
- [ ] Produzir a seção **Exemplos de Uso** do relatório.
- [ ] Produzir a seção **Conclusão** do relatório.

## Estrutura sugerida de pastas

```text
Trabalho-AED/
├── README.md
├── data/
│   ├── generos.txt
│   └── artistas.txt
├── docs/
│   └── relatorio.pdf
└── src/
    ├── main.c
    ├── lista.h
    └── lista.c
```

> As pastas `data/` e `docs/` representam a organização planejada e podem ser criadas durante o desenvolvimento.

## Formato sugerido dos arquivos de entrada

Os arquivos podem usar ponto e vírgula (`;`) como separador. A primeira linha pode ser um cabeçalho e cada registro deve ocupar uma linha. Para simplificar a leitura em C, os campos textuais não devem conter ponto e vírgula.

### `generos.txt`

Formato:

```text
id;nome_genero
```

Exemplo:

```text
id;nome_genero
1;Rock
2;Samba
3;Jazz
```

### `artistas.txt`

O campo `id_genero` estabelece a associação do artista com um gênero presente em `generos.txt`.

Formato:

```text
id;id_genero;nome;cidade_origem;periodo_atuacao;qtd_integrantes;qtd_premiacoes
```

Exemplo:

```text
id;id_genero;nome;cidade_origem;periodo_atuacao;qtd_integrantes;qtd_premiacoes
1;1;Queen;Londres;1970-presente;4;28
2;2;Cartola;Rio de Janeiro;1928-1980;1;3
3;3;Miles Davis;Alton;1944-1991;1;8
```

Durante a carga, recomenda-se processar primeiro `generos.txt` e depois `artistas.txt`, validar os tipos numéricos, rejeitar identificadores duplicados e informar registros malformados.

## Status

Projeto em desenvolvimento. Os checklists deste documento serão atualizados conforme as funcionalidades e as partes do relatório forem concluídas.
