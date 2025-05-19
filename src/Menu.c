/**
 * @file Menu.c
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Implementação do menu e relacionados de gestão de antenas e efeitos nefastos
 * @version 1.0
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../include/Menu.h"
#include "../include/Listas_Ligadas.h"
#include "../include/Grafos.h"

/**
 * @brief Valor máximo para coordenada X (largura máxima do mapa)
 */
int MAX_X = 4096;

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

/**
 * @brief Menu principal do sistema de gestão de antenas
 * 
 * @note Apresenta um menu interativo com as seguintes opções:
 *       1. Carregar antenas do ficheiro
 *       2. Inserir nova antena
 *       3. Remover antena existente
 *       4. Listar antenas e efeitos nefastos
 *       5. Carregar cidade do ficheiro
 *       6. Listar antenas alcançadas numa procura em profundidade
 *       7. Listar antenas alcançadas numa procura em largura
 *       8. Procurar caminhos possíveis entre antenas
 *       9. Listar pontos de interseção entre antenas com frequências distintas
 *       0. Encerrar programa
 * 
 * @note Utiliza limparEcra() para manter a interface limpa
 */
void menuPrincipal(const char **traducoes)
{
  /*--------------------------------------------------------------------------------------------------------------------------*/

    Antena *primeiraAntena = NULL;
    Nefasto *primeiroNefasto = NULL;

    char frequencia;
    int numAntenas = 0;
    int numNefastos = 0;

    bool nefastosAtualizados = false;

    const char *localizacaoFicheiro[] =
    {
        "./data/antenas.txt",
        "./data/nefastos.txt",
        "./data/cidade.txt"
    };

 /*--------------------------------------------------------------------------------------------------------------------------*/

    Grafo* cidade = NULL;
    int antena;
    int inicio, destino;
    int caminho[MAX_CAMINHO];
    char frequencia1, frequencia2;

 /*--------------------------------------------------------------------------------------------------------------------------*/

    int opcaoMenu = 0;
    int x, y;
    int resultado[2];

    do
    {
        if (opcaoMenu != 0)
        {
            if (!(opcaoMenu >= 6)) printf("\n");
            printf("\nPressione 'Enter' para continuar...");
            getchar(); // Aguarda o utilizador pressionar 'Enter'
        }

        limparEcra();

        printf("|--------------------------------------------------------------------------------------------------------------|\n");
        puts  ("|                                                     MENU                                                     |");
        puts  ("|--------------------------------------------------------------------------------------------------------------|");
        puts  ("| 1. Carregar antenas e efeitos nefastos do ficheiro.                                                          |");
        puts  ("| 2. Inserir uma antena nova na lista.                                                                         |");
        puts  ("| 3. Remover uma antena existente da lista.                                                                    |");
        puts  ("| 4. Listar todas as antenas e efeitos nefastos.                                                               |");
        puts  ("|--------------------------------------------------------------------------------------------------------------|");
        puts  ("| 5. Carregar cidade do ficheiro.                                                                              |");
        puts  ("| 6. Listar antenas alcançadas numa procura em profundidade.                                                   |");
        puts  ("| 7. Listar antenas alcançadas numa procura em largura.                                                        |");
        puts  ("| 8. Procurar caminhos possíveis entre antenas.                                                                |");
        puts  ("| 9. Listar pontos de interseção entre antenas com frequências distintas.                                      |");
        puts  ("|--------------------------------------------------------------------------------------------------------------|");
        puts  ("| 0. Encerrar o programa.                                                                                      |");
        puts  ("|--------------------------------------------------------------------------------------------------------------|");

        printf("\nOpção: ");
        scanf ("%d", &opcaoMenu);
        getchar();

        limparEcra();

        // Introduz a informação da antena
        if (opcaoMenu == 2 || opcaoMenu == 3)
        {
            if (opcaoMenu == 2)
            {
                do
                {
                    limparEcra();

                    printf("Introduza a frequência da antena (de A-Z): ");
                    scanf(" %c", &frequencia);
                    getchar();

                } while (!((frequencia >= 'a' && frequencia <= 'z') || (frequencia >= 'A' && frequencia <= 'Z')));

                frequencia = toupper(frequencia);
            }

            printf("\nIntroduza a posição X da antena: ");
            scanf("%d", &x);
            getchar();

            printf("\nIntroduza a posição Y da antena: ");
            scanf("%d", &y);
            getchar();

            limparEcra();
        }
        else if (cidade != NULL && (*cidade).numAntenas > 0 && (opcaoMenu == 6 || opcaoMenu == 7))
        {
            do
            {
                limparEcra();

                printf("Introduza o número da antena (0 - %d): ", (*cidade).numAntenas - 1);
                scanf("%d", &antena);
                getchar();

            } while (!(antena >= 0 && antena <= (*cidade).numAntenas - 1));

            limparEcra();
        }
        else if (cidade != NULL && (*cidade).numAntenas > 0 && opcaoMenu == 8)
        {
            do
            {
                limparEcra();

                printf("Introduza o número da antena início (0 - %d): ", (*cidade).numAntenas - 1);
                scanf("%d", &inicio);
                getchar();

            } while (!(inicio >= 0 && inicio <= (*cidade).numAntenas - 1));

            do
            {
                limparEcra();

                printf("Introduza o número da antena destino (0 - %d): ", (*cidade).numAntenas - 1);
                scanf("%d", &destino);
                getchar();

            } while (!(destino >= 0 && destino <= (*cidade).numAntenas - 1));

            limparEcra();
        }
        else if (cidade != NULL && opcaoMenu == 9)
        {
            do
            {
                limparEcra();

                printf("Introduza a 1.ª frequencia: ");
                scanf(" %c", &frequencia1);
                getchar();

            } while (!((frequencia1 >= 'a' && frequencia1 <= 'z') || (frequencia1 >= 'A' && frequencia1 <= 'Z')));

            do
            {
                limparEcra();

                printf("Introduza a 2.ª frequencia: ");
                scanf(" %c", &frequencia2);
                getchar();

            } while (!((frequencia2 >= 'a' && frequencia2 <= 'z') || (frequencia2 >= 'A' && frequencia2 <= 'Z')));

            frequencia1 = toupper(frequencia1);
            frequencia2 = toupper(frequencia2);

            limparEcra();
        }

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
                resultado[0] = adicionarAntena(&primeiraAntena, NULL, &numAntenas, &frequencia, x, y, false);
                if (resultado[0] == -1) printf("❌ Coordenadas inválidas (devem ser positivas).");
                else if (resultado[0] == -2) printf("❌ Limite de X atingido (X deve ser menor que %d).", MAX_X);
                else if (resultado[0] == -3) printf("❌ Já existe a antena '%c' em (%d, %d).", frequencia, x, y);
                else
                {
                    printf("✅ Antena '%c' em (%d, %d) adicionada.", frequencia, x, y);
                    nefastosAtualizados = false;
                }
            break;

            case 3:
                resultado[0] = removerAntena(&primeiraAntena, &numAntenas, &frequencia, x, y);
                if (resultado[0] == -1) printf("❌ Não existem antenas na lista.");
                else if (resultado[0] == -2) printf("❌ Antena (%d, %d) não existe.", x, y);
                else
                {
                    printf("✅ Antena '%c' em (%d, %d) removida.", frequencia, x, y);
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
                libertarCidade(cidade);
                if (carregarCidade(&cidade, localizacaoFicheiro[2]) == -1) printf("❌ Não foi possível abrir o ficheiro: (%s)", localizacaoFicheiro[2]);
                else printf("✅ Ficheiro carregado com sucesso.");
            break;

            case 6:
                if (cidade != NULL && (*cidade).numAntenas >= 1) printf("Procura em profundidade da antena (%d):\n\n", antena);
                resultado[0] = procurarProfundidade(cidade, antena);
                if (resultado[0] == -1) printf("❌ Procura falhou, cidade inválida.\n");
                else if (resultado[0] == -2) printf("❌ Procura falhou, antena inválida.\n");
                else if (resultado[0] == -3) printf("❌ Não existem antenas.\n");
                resetVisitados(cidade);
            break;

            case 7:
                if (cidade != NULL && (*cidade).numAntenas >= 1) printf("Procura em largura da antena (%d):\n\n", antena);
                resultado[0] = procurarLargura(cidade, antena);
                if (resultado[0] == -1) printf("❌ Procura falhou, cidade inválida.\n");
                else if (resultado[0] == -2) printf("❌ Procura falhou, antena inválida.\n");
                else if (resultado[0] == -3) printf("❌ Não existem antenas.\n");
                resetVisitados(cidade);
            break;

            case 8:
                if (cidade != NULL && (*cidade).numAntenas >= 2) printf("Caminhos possíveis (de %d a %d):\n\n", inicio, destino);
                resultado[0] = procurarCaminhos(cidade, inicio, destino, caminho, 0);
                if (resultado[0] == -1) printf("❌ Procura falhou, cidade inválida\n");
                else if (resultado[0] == -2) printf("❌ Procura falhou, antena origem inválida.\n");
                else if (resultado[0] == -3) printf("❌ Procura falhou, antena destino inválida.\n");
                else if (resultado[0] == -4) printf("❌ Procura falhou, antena origem e destino inválidas.\n");
                else if (resultado[0] == -5) printf("❌ Não existem antenas suficientes para haver caminhos.\n");
                else if (resultado[0] == 0) printf("❌ Não existem caminhos.\n");
                resetVisitados(cidade);
            break;

            case 9:
                if (cidade != NULL) printf("Pontos de interseção entre as antenas com frequencias '%c' e '%c':\n\n", frequencia1, frequencia2);
                resultado[0] = listarIntersecoes(cidade, frequencia1, frequencia2);
                if (resultado[0] == -1) printf("❌ Procura falhou, cidade inválida.\n");
                else if (resultado[0] == -2) printf("❌ As frequências devem ser diferentes.\n");
                else if (resultado[0] == 0) printf("❌ Não existem pontos de interseção.\n");
            break;

            case 0:
                libertarAntenas(&primeiraAntena, &numAntenas);
                libertarNefastos(&primeiroNefasto, &numNefastos);
                libertarCidade(cidade);
            break;

            default:
                printf("\n❌ Opção inválida.");
            break;
        }

    } while (opcaoMenu != 0);

    printf("Encerrando o programa...\n\n");

    return;
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