# Trabalho Prático

Licenciatura em Engenharia de Sistemas Informáticos 2024-25 (**Pós-Laboral**)

Estruturas de Dados Avançados

## Alunos
| Número | Nome             |
| 31537  | Gonçalo Carvalho |

## organização

[src/](./src/)  Código da solução desenvolvida 

[doc/](./doc/)  documentação com o relatório

## Introdução
Este projeto foi desenvolvido como parte do programa da disciplina de Estruturas de Dados Avançados (EDA).
A aplicação destina-se a criar funções que manipulem dados de listas ligadas.

## Objetivo
O objetivo principal deste trabalho é desenvolver uma solução em linguagem C que permita a manipulação antenas, efeitos nefastos em listas ligadas.

## Estrutura do Projeto
-   [data/](./data)
    - Diretório com os ficheiros que armanezam dados do programa.
        -   [antenas.txt](./doc/antenas.txt)
            -   Arquivo com dados do mapa das antenas.
        -   [cidade.txt](./doc/cidade.txt)
            -   Arquivo com dados do mapa das antenas da cidade.
        -   [nefastos.txt](./doc/nefastos.txt)
            -   Arquivo com dados do mapa dos efeitos nefastos.

-   [doc/](./doc)
    - Diretório com os relatórios e documentação do projeto.
        -   [Relatório_-_Gestão_de_Estruturas.pdf](./doc/Relatório - Gestão de Estruturas.pdf)
            -   Relatório do projeto da aplicação informática.

-   [include/](./include)
    -   Contém os arquivos de cabeçalho (headers) do projeto.
        -   [Menu.h](./include/Menu.h)
            -   Arquivo header principal que conecta e declara as funções do programa em linguagem C do menu.
        -   [Listas_Ligadas.h](./include/Listas_Ligadas.h)
            -   Arquivo header que conecta e declara as funções do programa em linguagem C das listas ligadas.
        -   [Grafos.h](./include/Grafos.h)
            -   Arquivo header que conecta e declara as funções do programa em linguagem C dos grafos.

-   [src/](./src)
    -   Contém os ficheiros fonte do programa em linguagem C.
        -   [Menu.c](./src/Menu.c)
            -   Ficheiro principal que contém a função do menu interativo.
        -   [Listas_Ligadas.c](./src/Listas_Ligadas.c)
            -   Ficheiro que contem as funções das listas ligadas.
        -   [Grafos.c](./src/Grafos.c)
            -   Ficheiro que contem as funções dos grafos.

-   [Makefile](Makefile)
    -   Arquivo utilizado pelo comando `make` para compilar o projeto de forma automatizada.

-   [README.md](README.md)
    -   Arquivo que contém informações básicas sobre o projeto, a sua descrição, uso e outras instruções.

-   [Gestor_de_Estruturas](Gestor de Estruturas)
    -   Executável do projeto, gerado após a compilação.

### Descrição da Estrutura

Este projeto está organizado de forma a separar claramente os componentes do código-fonte, dados e documentação.
    Os diretórios data/ e doc/ armazenam os ficheiros de dados usados pelo programa, e os relatórios e documentação do projeto.

    O diretório *include/* mantém o ficheiro principal de cabeçalho que interliga todas as funções e algoritmos do projeto.

    O diretório *src/* contém os ficheiros-fonte da aplicação, organizados de acordo com suas funcionalidades específicas.

    O ficheiro *Makefile* automatiza o processo de compilação do projeto, enquanto *README.md* fornece informações básicas sobre o projeto.

    Por fim, *Gestor de Estruturas* é o executável gerado após a compilação do projeto.

## Compilação
Para a compilação e execução do programa, é apenas necessário utilizar o comando *make* a partir do diretório raíz do projeto

Após a compilação do projeto, é gerado um novo executável *Gestor de Estruturas*, na raíz do repositório.

## Execução / Utilização
Para executar esta aplicação, é possível da seguinte forma:
- CLI / Consola:
  - Navegar para o diretório raíz do projeto, e executar "*./Gestor de Estruturas*"

## Distribuição de Tarefas
- **Gonçalo Carvalho:**
    - Participação na organização de dados do projeto e sua verificação.
    - Melhoramentos relacionados com funções e leitura de ficheiros.
    - Troubleshooting.
    - Adição de dados para testes de funcionamento.
    - Contribuição na elaboração do relatório.
    - Colaboração na compilação final e revisão geral do relatório.

## Bibliografia
Durante o desenvolvimento do projeto, foram utilizadas várias referências:
- [Documentação livrarias em C](https://www.tutorialspoint.com/c_standard_library/index.htm)
- [Documentação em Doxygen](https://www.doxygen.nl/manual/docblocks.html)(https://axom.readthedocs.io/en/develop/docs/sphinx/coding_guide/sec04_header_org.html)(https://docs.rtems.org/branches/master/eng/coding-doxygen.html)
- [Fórum StackOverflow](https://stackoverflow.com)
- [Ambiente de desenvolvimento Visual Studio Code](https://code.visualstudio.com)
- Exercícios da disciplina de Programação Imperativa - Moodle IPCA LESIPL
- [Ferramenta MAKE em Windows](https://gnuwin32.sourceforge.net/packages/make.htm)
- [Compilador GCC em Windows](https://sourceforge.net/projects/mingw/files/)
- [Texmaker](https://www.xm1math.net/texmaker/).
- [Overleaf](https://www.overleaf.com/).
- [CTAN](https://ctan.org/).
- [LatexProject](https://www.latex-project.org/)
