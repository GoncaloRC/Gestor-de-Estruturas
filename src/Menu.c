/**
 * @file Menu.c
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Implementação do menu
 * @version 2.0
 * @date 2025-05-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "../include/Menu.h"

/**
 * @brief Limpa o ecrã do terminal de forma multi-plataforma
 * 
 * @return int
 * @return 1 em caso de ocorrer uma limpeza em sistema operativo do windows
 * @return 2 em caso de ocorrer uma limpeza em sistema operativo baseado em Unix
 * @return 2 em caso de ocorrer uma limpeza em sistema operativo alternativo
 * 
 * @note Funciona em Windows, Linux/Unix e macOS
 * @note Para sistemas não suportados, imprime linhas vazias
 */ 
int limparEcra()
{
    #if defined(_WIN32) || defined(_WIN64) // Sistema Operativo - Windows
        system("cls");
        return 1;

    #elif defined(__unix__) || defined(__APPLE__) // Sistemas baseado em Unix (Linux e macOS)
        system("clear");
        return 2;

    #else
        // Para outros sistemas não suportados, imprime linhas vazias de forma a simular a limpeza de ecrã
        for (int i = 0; i < 50; ++i)
        {
            printf("\n");
        }
        return 3;
    #endif

    return 0;
}

/**
 * @brief Estrutura que representa um idioma e as respetivas traduções associadas.
 * 
 * Esta tabela contém os idiomas disponíveis e os textos traduzidos para cada um.
 * O último elemento (com NULL) serve de marcador para o fim do array.
 */
TabelaIdioma idiomas[] =
{
    {"PT", {"Lista de Antenas", "Frequência", "Lista de Efeitos Nefastos"}},
    {"EN", {"Antennas List", "Frequency", "Harmful Effects List"}},
    {NULL, {NULL, NULL, NULL}},
};

/**
 * @brief Altera o idioma da interface com base no código fornecido.
 * 
 * Esta função procura o idioma correspondente ao código recebido (por exemplo, "PT" ou "EN")
 * e devolve o array de strings com as traduções respetivas. Se o idioma não for encontrado,
 * é devolvido o idioma por defeito (inglês).
 * 
 * @param idioma Código do idioma pretendido (por exemplo, "PT" ou "EN").
 * @return const char** Array de strings com as traduções no idioma selecionado.
 */
const char **alterarIdioma(const char *idioma)
{
    for (int i = 0; idiomas[i].idioma != NULL; i++)
    {
        if (strcmp(idiomas[i].idioma, idioma) == 0)
        {
            return idiomas[i].traducoes;
        }
    }

    return alterarIdioma("EN"); // Idioma não encontrado (devolve o default EN)
}

Vertice *procurarAntenaMenu(Grafo *cidade, int x, int y)
{
    int erro = 0;
    Vertice *antena = procurarAntena(cidade, x, y, &erro);

    if (erro == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Cidade inválida\n");
    else if (erro == ERRO_OBJETO_NAO_EXISTE) printf("❌ Antena não existe.\n");

    return antena;
}

/**
 * @brief Menu principal do sistema de gestão de antenas
 * 
 * @note Apresenta um menu interativo
 * 
 * @note Utiliza limparEcra() para manter a interface limpa
 */
void menuPrincipal(const char **traducoes)
{
    const char *localizacaoFicheiro[] =
    {
        "./data/antenas.txt",
        "./data/nefastos.txt",
        "./data/cidade.txt"
    };

  /*---- Antenas/Nefastos ----------------------------------------------------------------------------------------------------*/

    Antena *primeiraAntena = NULL;
    Nefasto *primeiroNefasto = NULL;

    int numAntenas = 0;
    int numNefastos = 0;

 /*---- Grafos ---------------------------------------------------------------------------------------------------------------*/

    Grafo *cidade = criarCidade();

 /*---- Menu -----------------------------------------------------------------------------------------------------------------*/

    int opcaoMenu = 0;

    Vertice *antena1, *antena2;
    char frequencia1, frequencia2;
    int x1, y1, x2, y2;
    int resultado[2];
    char opcaoSN;

    bool nefastosAtualizados = false;

 /*--------------------------------------------------------------------------------------------------------------------------*/

    limparEcra();

    do
    {
        // Dá reset das variáveis do menu
        antena1 = NULL, antena2 = NULL;
        frequencia1 = '.', frequencia2 = '.';
        x1 = -1, x2 = -1, y1 = -1, y2 = -1;
        resultado[0] = 0, resultado[1] = 0;

        // Menu principal
        puts("|--------------------------------------------------------------------------------------------------------------|");
        puts("|                                                     MENU                                                     |");
        puts("|--------------------------------------------------------------------------------------------------------------|");
        puts("| 1. Carregar antenas e efeitos nefastos do ficheiro.                                                          |");
        puts("| 2. Inserir uma antena nova na lista.                                                                         |");
        puts("| 3. Remover uma antena existente da lista.                                                                    |");
        puts("| 4. Listar todas as antenas e efeitos nefastos.                                                               |");
        puts("|--------------------------------------------------------------------------------------------------------------|");
        puts("| 5. Carregar cidade do ficheiro e interligar arestas de frequências iguais.                                   |");
        puts("| 6. Inserir uma antena nova na cidade.                                                                        |");
        puts("| 7. Inserir uma ligação (aresta) nova na cidade.                                                              |");
        puts("| 8. Remover uma antena existente da cidade.                                                                   |");
        puts("| 9. Remover uma ligação (aresta) existente da cidade.                                                         |");
        puts("| 10. Procurar antenas alcançadas numa procura em profundidade.                                                |");
        puts("| 11. Procurar antenas alcançadas numa procura em largura.                                                     |");
        puts("| 12. Procurar caminhos possíveis entre antenas.                                                               |");
        puts("| 13. Procurar pontos de interseção entre antenas com frequências distintas.                                   |");
        puts("| 14. Listar antenas da cidade.                                                                                |");
        puts("| 15. Listar arestas de uma antena da cidade.                                                                  |");
        puts("|--------------------------------------------------------------------------------------------------------------|");
        puts("| 0. Encerrar o programa.                                                                                      |");
        puts("|--------------------------------------------------------------------------------------------------------------|");

        printf("\n> ");
        scanf ("%d", &opcaoMenu);
        getchar();

        if (opcaoMenu == 404) opcaoMenu++;

     /*-----------------------------------------------------------------------------------------------------------------------*/

        // Introduz frequências
        if (opcaoMenu == 2 || (cidade != NULL && (opcaoMenu == 6 || ((*cidade).numAntenas > 0 && opcaoMenu == 13))))
        {
            do
            {
                limparEcra();

                if (opcaoMenu == 13) printf("Introduza a frequência da 1.ª antena (de A-Z): ");
                else printf("Introduza a frequência da antena (de A-Z): ");
                scanf(" %c", &frequencia1);
                getchar();

            } while (!((frequencia1 >= 'a' && frequencia1 <= 'z') || (frequencia1 >= 'A' && frequencia1 <= 'Z')));

            frequencia1 = toupper(frequencia1);

            if (opcaoMenu == 13)
            {
                do
                {
                    limparEcra();

                    printf("Introduza a frequência da 2.ª antena (de A-Z): ");
                    scanf(" %c", &frequencia2);
                    getchar();

                } while (!((frequencia2 >= 'a' && frequencia2 <= 'z') || (frequencia2 >= 'A' && frequencia2 <= 'Z')));

                frequencia2 = toupper(frequencia2);
            }
        }

        // Introduz coordenadas (X e Y)
        if (opcaoMenu == 2 || opcaoMenu == 3 || (cidade != NULL && ((opcaoMenu == 6 || opcaoMenu == 7)
              || ((*cidade).numAntenas >0 && (opcaoMenu == 8 || opcaoMenu == 9 || opcaoMenu == 10 || opcaoMenu == 11 || opcaoMenu == 12 || opcaoMenu == 15)))))
        {
            do
            {
                limparEcra();

                if (opcaoMenu == 7 || opcaoMenu == 9 || opcaoMenu == 12) printf("Introduza a posição X da 1.ª antena: ");
                else printf("Introduza a posição X da antena: ");
                scanf("%d", &x1);
                getchar();

                if (opcaoMenu == 7 || opcaoMenu == 9 || opcaoMenu == 12) printf("\nIntroduza a posição Y da 1.ª antena: ");
                else printf("\nIntroduza a posição Y da antena: ");
                scanf("%d", &y1);
                getchar();

            } while (x1 < 0 || y1 < 0);

            if (opcaoMenu != 6)
            {
                antena1 = procurarAntenaMenu(cidade, x1, y1);
                if (antena1 == NULL)
                {
                    printf("❌ Antena não existe\n");
                    opcaoMenu = 404;
                }
            }

            if (opcaoMenu == 7 || opcaoMenu == 9 || opcaoMenu == 12)
            {
                do
                {
                    limparEcra();

                    printf("Introduza a posição X da 2.ª antena: ");
                    scanf("%d", &x2);
                    getchar();

                    printf("\nIntroduza a posição Y da 2.ª antena: ");
                    scanf("%d", &y2);
                    getchar();

                } while (x2 < 0 || y2 < 0);

                antena2 = procurarAntenaMenu(cidade, x2, y2);
                if (antena2 == NULL)
                {
                    printf("❌ Antena não existe\n");
                    opcaoMenu = 404;
                }
            }
        }

        // Questiona o usuário (S ou N)
        if (opcaoMenu == 5)
        {
            do
            {
                limparEcra();

                if (opcaoMenu == 7) puts("Pretende remover a aresta de retorno?");
                else if (opcaoMenu == 9) puts("Pretende adicionar a aresta de retorno?");
                else
                {
                    puts("Pretende interligar as antenas da cidade (s/n)?");
                    printf("\n> ");
                }
                scanf(" %c", &opcaoSN);
                getchar();

                if (opcaoSN == 'Y') opcaoSN = 'S';
                else if (opcaoSN == 'y') opcaoSN = 's';

            } while (!(opcaoSN == 'S' || opcaoSN == 's' || opcaoSN == 'N' || opcaoSN == 'n'));
        }

        limparEcra();

     /*-----------------------------------------------------------------------------------------------------------------------*/

        switch (opcaoMenu)
        {
            case 1:
                nefastosAtualizados = false;
                libertarAntenas(&primeiraAntena, &numAntenas);
                libertarNefastos(&primeiroNefasto, &numNefastos);

                resultado[0] = carregarAntenas(&primeiraAntena, &numAntenas, localizacaoFicheiro[0]);
                resultado[1] = carregarNefastos(&primeiroNefasto, &numNefastos, localizacaoFicheiro[1]);

                if (resultado[0] == -1 || resultado[1] == -1)
                {
                    printf("❌ Não foi possível abrir o ficheiro:");
                    if (resultado[0] == -1) printf("\n> Antenas (%s)", localizacaoFicheiro[0]);
                    if (resultado[1] == -1) printf("\n> Nefastos (%s)", localizacaoFicheiro[1]);

                    printf("\n");
                    opcaoMenu = 5;
                }
                else
                {
                    printf("✅ Ficheiros carregados com sucesso.");
                    if (numNefastos > 0) nefastosAtualizados = true; /* Ficheiro nefastos deve estar atualizado ou vazio! */
                }
            break;

            case 2:
                resultado[0] = adicionarAntena(&primeiraAntena, NULL, &numAntenas, &frequencia1, x1, y1, false);
                if (resultado[0] == -2) printf("❌ Já existe a antena '%c' em (%d, %d).", frequencia1, x1, y1);
                else
                {
                    printf("✅ Antena '%c' em (%d, %d) adicionada.", frequencia1, x1, y1);
                    nefastosAtualizados = false;
                }
            break;

            case 3:
                resultado[0] = removerAntena(&primeiraAntena, &numAntenas, &frequencia1, x1, y1);
                if (resultado[0] == -1) printf("❌ Não existem antenas na lista.");
                else if (resultado[0] == -2) printf("❌ Antena (%d, %d) não existe.", x1, y1);
                else
                {
                    printf("✅ Antena '%c' em (%d, %d) removida.", frequencia1, x1, y1);
                    nefastosAtualizados = false;
                }
            break;

            case 4:
                if (nefastosAtualizados == false)
                {
                    libertarNefastos(&primeiroNefasto, &numNefastos);
                    calcularNefastos(primeiraAntena, &primeiroNefasto, &numNefastos); /* Calcula os efeitos nefastos para apresentar */
                    nefastosAtualizados = true;
                }
                apresentarTabela(primeiraAntena, primeiroNefasto, numAntenas, numNefastos, traducoes);
            break;

            case 5:
                libertarArestas(cidade);
                resultado[0] = carregarCidade(cidade, localizacaoFicheiro[2]);
                if (resultado[0] == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Cidade inválida.\n");
                else if (resultado[0] == ERRO_ABRIR_FICHEIRO) printf("❌ Não foi possível abrir o ficheiro: (%s)", localizacaoFicheiro[2]);
                else if (resultado[0] == ERRO_ALOCACAO_MEMORIA) printf("❌ Não foi possível alocar memória!");
                else
                {
                    puts("✅ Ficheiro carregado com sucesso.");
                    if (opcaoSN == 'S' || opcaoSN == 's')
                    {
                        resultado[1] = interligarAntenas(cidade, true, false, false);
                        if (resultado[1] == ERRO_ALOCACAO_MEMORIA) printf("❌ Não foi possível alocar memória!");
                        else printf("✅ Antenas com frequências iguais interligadas com sucesso.\n");
                    }
                }
            break;

            case 6:
                resultado[0] = adicionarAntenaOrdenada(cidade, &frequencia1, x1, y1);
                if (resultado[0] == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Cidade inválida.\n");
                else if (resultado[0] == ERRO_OBJETO_JA_EXISTE) printf("❌ Já existe a antena '%c' em (%d, %d).\n", frequencia1, x1, y1);
                else printf("✅ Antena '%c' em (%d, %d) adicionada.\n", frequencia1, x1, y1);
            break;

            case 7:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    resultado[0] = adicionarAresta(antena1, antena2, true);
                    if (resultado[0] == ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS) printf("❌ Procura falhou, antena origem e destino inválidas.\n");
                    else if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Procura falhou, antena origem inválida.\n");
                    else if (resultado[0] == ERRO_ANTENA_DESTINO_INVALIDA) printf("❌ Procura falhou, antena destino inválida.\n");
                    else if (resultado[0] == ERRO_OBJETO_JA_EXISTE) printf("❌ A aresta especificada já existe.\n");
                    else if (resultado[0] == ERRO_ALOCACAO_MEMORIA) printf("❌ Não foi possível alocar memória!\n");
                    else printf("✅ Aresta (%d, %d) -> (%d, %d) adicionada.\n", x1, y1, x2, y2);
                }
            break;

            case 8:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    resultado[0] = removerAntena2(cidade, &frequencia1, x1, y1);
                    if (resultado[0] == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Cidade inválida.\n");
                    else if (resultado[0] == ERRO_OBJETO_NAO_EXISTE) printf("❌ Não existe a antena (%d, %d).\n", x1, y1);
                    else printf("✅ Antena '%c' em (%d, %d) removida.\n", frequencia1, x1, y1);
                }
            break;

            case 9:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    resultado[0] = removerAresta(antena1, antena2);
                    if (resultado[0] == ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS) printf("❌ Procura falhou, antena origem e destino inválidas.\n");
                    else if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Procura falhou, antena origem inválida.\n");
                    else if (resultado[0] == ERRO_ANTENA_DESTINO_INVALIDA) printf("❌ Procura falhou, antena destino inválida.\n");
                    else if (resultado[0] == ERRO_OBJETO_NAO_EXISTE) printf("❌ A aresta especificada não existe.\n");
                    else printf("✅ Aresta (%d, %d) -> (%d, %d) removida.", x1, y1, x2, y2);
                }
            break;

            case 10:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    printf("Procura em profundidade da antena '%c'(%d, %d):\n\n", (*antena1).frequencia, x1, y1);
                    resultado[0] = procurarProfundidade(antena1);
                    if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Procura falhou, antena inválida.\n");
                    resetVisitados(cidade);
                }
            break;

            case 11:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    printf("Procura em largura da antena '%c'(%d, %d):\n\n", (*antena1).frequencia, x1, y1);
                    resultado[0] = procurarLargura(antena1, (*cidade).numAntenas);
                    if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Procura falhou, antena inválida.\n");
                    else if (resultado[0] == ERRO_OVERFLOW_LISTA) printf("❌ Procura falhou, ocorreu overflow da lista.\n");
                    resetVisitados(cidade);
                }
            break;

            case 12:
                if (cidade != NULL && (*cidade).numAntenas < 2) printf("❌ Não existem antenas suficientes para haver caminhos.\n");
                else
                {
                    printf("Caminhos possíveis de '%c'(%d, %d) a '%c'(%d, %d):\n\n", (*antena1).frequencia, x1, y1, (*antena2).frequencia, x2, y2);
                    resultado[0] = procurarCaminhos(antena1, antena2, (*cidade).numAntenas);
                    if (resultado[0] == 0) printf("❌ Não existem caminhos.\n");
                    else if (resultado[0] == ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS) printf("❌ Procura falhou, antena origem e destino inválidas.\n");
                    else if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Procura falhou, antena origem inválida.\n");
                    else if (resultado[0] == ERRO_ANTENA_DESTINO_INVALIDA) printf("❌ Procura falhou, antena destino inválida.\n");
                    resetVisitados(cidade);
                }
            break;

            case 13:
                if (cidade != NULL && (*cidade).numAntenas < 2) printf("❌ Não existem antenas suficientes para haver interseções.\n");
                else
                {
                    if (cidade != NULL && frequencia1 != frequencia2) printf("Pontos de interseção entre as antenas com frequencias '%c' e '%c':\n\n", frequencia1, frequencia2);
                    resultado[0] = listarIntersecoes(cidade, frequencia1, frequencia2);
                    if (resultado[0] == 0) printf("❌ Não existem pontos de interseção.\n");
                    else if (resultado[0] == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Procura falhou, cidade inválida.\n");
                    else if (resultado[0] == ERRO_FREQUENCIAS_IGUAIS) printf("❌ Procura falhou, as frequências devem ser diferentes.\n");
                }
            break;

            case 14:
                if (cidade != NULL && (*cidade).numAntenas > 0)
                {
                    printf("Lista de Antenas da cidade (%d):\n\n", (*cidade).numAntenas);
                    listarAntenas(cidade);
                }
                else printf("❌ Não existem antenas na cidade.\n");
            break;

            case 15:
                if (cidade != NULL && (*cidade).numAntenas > 0)
                {
                    printf("Lista de Arestas da antena '%c'(%d, %d):\n\n", (*antena1).frequencia, x1, y1);
                    resultado[0] = listarArestasAntena(antena1);
                    if (resultado[0] == 0) printf("❌ A antena não tem arestas.\n");
                    else if (resultado[0] == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Antena inválida.\n");
                }
                else printf("❌ Não existem antenas na cidade.\n");
            break;

            case 0:
                libertarAntenas(&primeiraAntena, &numAntenas);
                libertarNefastos(&primeiroNefasto, &numNefastos);
                cidade = libertarCidade(cidade);
                printf("Encerrando o programa...\n");
                return;
            break;

            case 404:
            break;

            default:
                printf("❌ Opção inválida.\n");
            break;
        }

     /*-----------------------------------------------------------------------------------------------------------------------*/

        if (resultado[0] == ERRO_ALOCACAO_MEMORIA || resultado[1] == ERRO_ALOCACAO_MEMORIA)
        {
            printf("\nPressione 'Enter' para sair...");
            getchar(); // Aguarda o utilizador pressionar 'Enter'
            libertarAntenas(&primeiraAntena, &numAntenas);
            libertarNefastos(&primeiroNefasto, &numNefastos);
            cidade = libertarCidade(cidade);

            limparEcra();
            printf("Encerrando o programa...\n");
            return;
        }

        if (opcaoMenu <= 4) puts("");
        printf("\nPressione \"Enter\" para continuar...");
        getchar(); // Aguarda o utilizador pressionar 'Enter'

        limparEcra();

    } while (7);
}

/**
 * @brief Função principal do programa
 * @return Retorna int 0
 * 
 * @note Inicia o programa ao fazer a chamanda do menu principal
 */
int main()
{
    menuPrincipal(alterarIdioma("PT"));

    return 0;
}