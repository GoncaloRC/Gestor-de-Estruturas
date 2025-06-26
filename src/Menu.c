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

// Função de ler inteiros extraída de uma biblioteca
bool lerInteiroSeguro(int *resultado)
{
    char buffer[12]; // Suficiente para "-2147483648\0"
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) return false;  // Erro na leitura

    // Verifica se o buffer estava cheio e não contém \n
    bool buffer_cheio = !strchr(buffer, '\n');

    // Remove \n e verifica caracteres inválidos
    for (char *p = buffer; *p != '\0'; p++)
    {
        if (*p == '\n')
        {
            *p = '\0';
            buffer_cheio = false;
            break;
        }
        else if (!isdigit((unsigned char)*p) && *p != '-' && *p != '+')
        {
            // Limpa todo o buffer de entrada se houver caracteres inválidos
            while (getchar() != '\n');
            return false;
        }
    }

    // Limpa o buffer se estava cheio
    if (buffer_cheio) while (getchar() != '\n');

    // Verifica se o buffer está vazio
    if (buffer[0] == '\0') return false;

    // Conversão e verificação de overflow
    char *endptr;
    long num = strtol(buffer, &endptr, 10);
    
    if (*endptr != '\0' || num < -2147483648 || num > 2147483648) return false;

    *resultado = (int)num;
    return true;
}

Vertice *procurarAntenaMenu(Grafo *cidade, Dados *dados, int x, int y)
{
    int erro = 0;
    Vertice *antena = procurarAntena(cidade, x, y, &erro);

    if (erro < 0) imprimirErros(dados, erro);

    return antena;
}

bool requesitarDados(Grafo *cidade, Dados *dados, int numFrequencias, int numCoordenadas, bool procurarAntena)
{
    // Limita a requesitação de dados a 2 iterações
    for (int i = 0; i < MAX_QUESTOES; i++)
    {
        // Requesita a frequencia (A - Z)
        if (numFrequencias > 0)
        {
            do
            {
                limparEcra();

                if (i > 0 || numFrequencias > 1) printf("Introduza a frequência da %d.ª antena (de A-Z): ", i + 1);
                else printf("Introduza a frequência da antena (de A-Z): ");
                scanf(" %c", &(*dados).frequencia[i]);
                getchar();

                (*dados).frequencia[i] = toupper((*dados).frequencia[i]);

            } while (!((*dados).frequencia[i] >= 'A' && (*dados).frequencia[i] <= 'Z'));
        
            numFrequencias--;
        }

        // Requesita as coordenadas (X e Y)
        if (numCoordenadas > 0)
        {
            do
            {
                limparEcra();

                if (i > 0 || numCoordenadas > 1) printf("Introduza a posição X da %d.ª antena: ", i + 1);
                else printf("Introduza a posição X da antena: ");
                scanf("%d", &(*dados).x[i]);
                getchar();

                if (i > 0 || numCoordenadas > 1) printf("\nIntroduza a posição Y da 1.ª antena: ");
                else printf("\nIntroduza a posição Y da antena: ");
                scanf("%d", &(*dados).y[i]);
                getchar();

            } while (&(*dados).x[i] < 0 || &(*dados).y[i] < 0);

            if (procurarAntena)
            {
                limparEcra();

                (*dados).antena[i] = procurarAntenaMenu(cidade, dados, (*dados).x[i], (*dados).y[i]);
                if ((*dados).antena[i] == NULL) return false;  /* ERRO! Antena não encontrada */
            }

            numCoordenadas--;
        }
    }

    limparEcra();

    return true; /* Sucesso */
}

bool requesitarResposta(const char *questao)
{
    // Armazena uma fração do input do usuário (2 caracteres)
    char buffer[4];

    // Requesita resposta a questão (S ou N)
    do
    {
        limparEcra();

        // Imprime a questão
        printf("%s\n", questao);
        printf("\n> ");

        // Lê no máximo 3 caracteres (2 + \n)
        if (!fgets(buffer, sizeof(buffer), stdin)) return false; // Erro ou EOF
        
        // Limpa o buffer se o usuário digitou mais que 2 chars
        if (strchr(buffer, '\n') == NULL)
        {
            int resultado;
            while ((resultado = getchar()) != '\n' && resultado != EOF); // Limpeza rápida
        }
        
        // Remove '\n', se existir
        buffer[strcspn(buffer, "\n")] = '\0';
        
        // Verifica se foi introduzido apenas 1 caracter
        if (strlen(buffer) != 1) continue;

        buffer[0] = tolower(buffer[0]);

    } while (!(buffer[0] == 's' || buffer[0] == 'y' || buffer[0] == 'n'));

    limparEcra();

    // Retorna true ou false dependendo da resposta
    return (buffer[0] != 'n');
}

int imprimirErros(Dados *dados, int resultado)
{
    if (resultado >= 0) return resultado;
    else if (resultado == LL_ERRO_REDE_PONTEIRO_INVALIDO) printf("❌ Rede inválida.\n");
    else if (resultado == ERRO_CIDADE_PONTEIRO_INVALIDO) printf("❌ Cidade inválida.\n");
    else if (resultado == ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS) printf("❌ Antenas origem e destino inválidas.\n");
    else if (resultado == ERRO_ANTENA_INICIO_INVALIDA) printf("❌ Antena origem inválida.\n");
    else if (resultado == ERRO_ANTENA_DESTINO_INVALIDA) printf("❌ Antena destino inválida.\n");

    else if (resultado == ERRO_ANTENA_JA_EXISTE) /* LL_ERRO_ANTENA_JA_EXISTE */ printf("❌ Já existe a antena '%c'(%d, %d).\n", (*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
    else if (resultado == ERRO_ANTENA_NAO_EXISTE) /* LL_ERRO_ANTENA_NAO_EXISTE */ printf("❌ Não existe a antena (%d, %d).\n", (*dados).x[0], (*dados).y[0]);
    else if (resultado == LL_ERRO_NEFASTO_JA_EXISTE) printf("❌ Já existe o nefasto (%d, %d).\n", (*dados).x[0], (*dados).y[0]);
    else if (resultado == LL_ERRO_NEFASTO_NAO_EXISTE) printf("❌ Não existe o nefasto (%d, %d).\n", (*dados).x[0], (*dados).y[0]);
    else if (resultado == ERRO_ARESTA_JA_EXISTE) printf("❌ Já existe a aresta (%d, %d) -> (%d, %d).\n", (*dados).x[0], (*dados).y[0], (*dados).x[1], (*dados).y[1]);
    else if (resultado == ERRO_ARESTA_NAO_EXISTE) printf("❌ Não existe a aresta (%d, %d) -> (%d, %d).\n", (*dados).x[0], (*dados).y[0], (*dados).x[1], (*dados).y[1]);

    else if (resultado == ERRO_OVERFLOW_LISTA) printf("❌ Ocorreu overflow da lista.\n");
    else if (resultado == ERRO_ABRIR_FICHEIRO) /* == LL_ERRO_ABRIR_FICHEIRO */ printf("❌ Não foi possível abrir o ficheiro.\n");
    else if (resultado == ERRO_ALOCACAO_MEMORIA) /* == LL_ERRO_ALOCACAO_MEMORIA */ printf("❌ Não foi possível alocar memória!\n");

    return resultado;
}

/**
 * @brief Menu principal do sistema de gestão de antenas
 * 
 * @note Apresenta um menu interativo
 * 
 * @note Utiliza limparEcra() para manter a interface limpa
 */
void menuPrincipal()
{
  /*---- Ficheiros -----------------------------------------------------------------------------------------------------------*/
    const char *localizacaoFicheiro[] =
    {
        "./data/antenas.txt", /* Ficheiro com o mapa das antenas */
        "./data/nefastos.txt", /* Ficheiro com o mapa dos nefastos */
        "./data/cidade.txt" /* Ficheiro com o mapa dos vértices */
    };

  /*---- Antenas/Nefastos ----------------------------------------------------------------------------------------------------*/

    Rede *rede = LL_criarRede();

 /*---- Grafos ---------------------------------------------------------------------------------------------------------------*/

    Grafo *cidade = criarCidade();

 /*---- Menu -----------------------------------------------------------------------------------------------------------------*/

    int opcaoMenu = 337357357;

    Dados *dados = calloc(1, sizeof(Dados));
    int resultado[MAX_RESULTADOS];
    bool nefastosAtualizados = false;

 /*---------------------------------------------------------------------------------------------------------------------------*/

    do
    {
        // Dá reset dos resultados
        for (int i = 0; i < MAX_RESULTADOS; i++) resultado[i] = 0;

        // Menu principal
        do
        {
            limparEcra();

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

        } while (lerInteiroSeguro(&opcaoMenu) == false);

        limparEcra();

     /*-----------------------------------------------------------------------------------------------------------------------*/

        switch (opcaoMenu)
        {
            case 1:
                nefastosAtualizados = false;
                LL_libertarAntenas(rede);
                LL_libertarNefastos(rede);

                resultado[0] = LL_carregarAntenas(rede, localizacaoFicheiro[0]);
                resultado[1] = LL_carregarNefastos(rede, localizacaoFicheiro[1]);

                if (resultado[0] == LL_ERRO_ABRIR_FICHEIRO || resultado[1] == LL_ERRO_ABRIR_FICHEIRO)
                {
                    printf("❌ Não foi possível abrir o ficheiro:");
                    if (resultado[0] == LL_ERRO_ABRIR_FICHEIRO) printf("\n> Antenas (%s)", localizacaoFicheiro[0]);
                    if (resultado[1] == LL_ERRO_ABRIR_FICHEIRO) printf("\n> Nefastos (%s)", localizacaoFicheiro[1]);

                    LL_libertarAntenas(rede);
                    LL_libertarNefastos(rede);

                    printf("\n");
                }
                else
                {
                    printf("✅ Ficheiros carregados com sucesso.");
                    if ((*rede).numNefastos > 0) nefastosAtualizados = true; /* Ficheiro nefastos deve estar atualizado ou vazio! */
                }
            break;

            case 2:
                requesitarDados(NULL, dados, 1, 1, false);
                resultado[0] = LL_adicionarAntenaOrdenada(rede, &(*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                if (imprimirErros(dados, resultado[0]) >= 0)
                {
                    printf("✅ Antena '%c' em (%d, %d) adicionada.", (*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                    nefastosAtualizados = false;
                }
            break;

            case 3:
                if (rede == NULL || (*rede).numAntenas == 0) printf("❌ Não existem antenas na lista.");
                else
                {
                    requesitarDados(NULL, dados, 0, 1, false);
                    resultado[0] = LL_removerAntena(rede, &(*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                    if (imprimirErros(dados, resultado[0]) >= 0)
                    {
                        printf("✅ Antena '%c' em (%d, %d) removida.", (*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                        nefastosAtualizados = false;
                    }
                }
            break;

            case 4:
                if ((*rede).numAntenas == 0) printf("❌ Não existem antenas.");
                else
                {
                    if (nefastosAtualizados == false)
                    {
                        LL_libertarNefastos(rede);
                        LL_calcularNefastos(rede, false); /* Calcula os efeitos nefastos para apresentar */
                        nefastosAtualizados = true;
                    }
                    printf("Lista de Antenas (%d):\n\n", (*rede).numAntenas);
                    LL_apresentarAntenas(rede);
                    puts("\n");
                    printf("Lista de Nefastos (%d):\n\n", (*rede).numNefastos);
                    LL_apresentarNefastos(rede);
                }
            break;

            case 5:
                libertarAntenas(cidade);
                resultado[0] = carregarCidade(cidade, localizacaoFicheiro[2]);
                if (imprimirErros(dados, resultado[0]) >= 0)
                {
                    puts("✅ Ficheiro carregado com sucesso.");
                    printf("\nPressione \"Enter\" para continuar...");
                    getchar(); /* Aguarda o utilizador pressionar "Enter" */

                    if (requesitarResposta(QUESTAO_INTERLIGAR_ANTENAS))
                    {
                        resultado[1] = interligarAntenas(cidade, true, false, false);
                        if (imprimirErros(dados, resultado[1]) >= 0) printf("✅ Antenas com frequências iguais interligadas com sucesso.\n");
                    }
                }
            break;

            case 6:
                requesitarDados(cidade, dados, 1, 1, false);
                resultado[0] = adicionarAntenaOrdenada(cidade, &(*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                if (imprimirErros(dados, resultado[0]) >= 0) printf("✅ Antena '%c' em (%d, %d) adicionada.\n", (*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
            break;

            case 7:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    if (requesitarDados(cidade, dados, 0, 2, true) == false) break;
                    resultado[0] = adicionarAresta((*dados).antena[0], (*dados).antena[1], true);
                    if (imprimirErros(dados, resultado[0]) >= 0) printf("✅ Aresta (%d, %d) -> (%d, %d) adicionada.\n", (*dados).x[0], (*dados).y[0], (*dados).x[1], (*dados).y[1]);
                }
            break;

            case 8:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    requesitarDados(cidade, dados, 0, 1, false);
                    resultado[0] = removerAntena(cidade, &(*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                    if (imprimirErros(dados, resultado[0]) >= 0) printf("✅ Antena '%c' em (%d, %d) removida.\n", (*dados).frequencia[0], (*dados).x[0], (*dados).y[0]);
                }
            break;

            case 9:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    if (requesitarDados(cidade, dados, 0, 2, true) == false) break;
                    resultado[0] = removerAresta((*dados).antena[0], (*dados).antena[1]);
                    if (imprimirErros(dados, resultado[0]) >= 0) printf("✅ Aresta (%d, %d) -> (%d, %d) removida.", (*dados).x[0], (*dados).y[0], (*dados).x[1], (*dados).y[1]);
                }
            break;

            case 10:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    if (requesitarDados(cidade, dados, 0, 1, true) == false) break;
                    printf("Procura em profundidade da antena '%c'(%d, %d):\n\n", (*(*dados).antena[0]).frequencia, (*dados).x[0], (*dados).y[0]);
                    resultado[0] = procurarProfundidade((*dados).antena[0]);
                    imprimirErros(dados, resultado[0]);
                    resetVisitados(cidade);
                }
            break;

            case 11:
                if (cidade != NULL && (*cidade).numAntenas == 0) printf("❌ Não existem antenas.\n");
                else
                {
                    if (requesitarDados(cidade, dados, 0, 1, true) == false) break;
                    printf("Procura em largura da antena '%c'(%d, %d):\n\n", (*(*dados).antena[0]).frequencia, (*dados).x[0], (*dados).y[0]);
                    resultado[0] = procurarLargura((*dados).antena[0], (*cidade).numAntenas);
                    imprimirErros(dados, resultado[0]);
                    resetVisitados(cidade);
                }
            break;

            case 12:
                if (cidade != NULL && (*cidade).numAntenas < 2) printf("❌ Não existem antenas suficientes para haver caminhos.\n");
                else
                {
                    if (requesitarDados(cidade, dados, 0, 2, true) == false) break;
                    printf("Caminhos possíveis de '%c'(%d, %d) a '%c'(%d, %d):\n\n", (*(*dados).antena[0]).frequencia, (*dados).x[0], (*dados).y[0], (*(*dados).antena[1]).frequencia, (*dados).x[1], (*dados).y[1]);
                    resultado[0] = procurarCaminhos((*dados).antena[0], (*dados).antena[1], (*cidade).numAntenas);
                    if (resultado[0] == 0) printf("❌ Não existem caminhos.\n");
                    else imprimirErros(dados, resultado[0]);
                    resetVisitados(cidade);
                }
            break;

            case 13:
                if (cidade != NULL && (*cidade).numAntenas < 2) printf("❌ Não existem antenas suficientes para haver interseções.\n");
                else
                {
                    requesitarDados(cidade, dados, 2, 0, false);
                    if (cidade != NULL && (*dados).frequencia[0] != (*dados).frequencia[1]) printf("Pontos de interseção entre as antenas com frequencias '%c' e '%c':\n\n", (*dados).frequencia[0], (*dados).frequencia[1]);
                    resultado[0] = listarIntersecoes(cidade, (*dados).frequencia[0], (*dados).frequencia[1]);
                    if (resultado[0] == 0) printf("❌ Não existem pontos de interseção.\n");
                    else imprimirErros(dados, resultado[0]);
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
                    if (requesitarDados(cidade, dados, 0, 1, true) == false) break;
                    printf("Lista de Arestas da antena '%c'(%d, %d):\n\n", (*(*dados).antena[0]).frequencia, (*dados).x[0], (*dados).y[0]);
                    resultado[0] = listarArestasAntena((*dados).antena[0]);
                    if (resultado[0] == 0) printf("❌ A antena não tem arestas.\n");
                    else imprimirErros(dados, resultado[0]);
                }
                else printf("❌ Não existem antenas na cidade.\n");
            break;

            case 0:
                rede = LL_libertarRede(rede);
                cidade = libertarCidade(cidade);
                printf("Encerrando o programa...\n");
                return;
            break;

            default:
                printf("❌ Opção inválida.\n");
            break;
        }

     /*-----------------------------------------------------------------------------------------------------------------------*/

        // Encerra o programa caso ocorra erro de alocação de memória
        if (resultado[0] == ERRO_ALOCACAO_MEMORIA || resultado[1] == ERRO_ALOCACAO_MEMORIA) /* LL_ERRO_ALOCACAO_MEMORIA */
        {
            printf("\nPressione \"Enter\" para sair...");
            getchar(); /* Aguarda o utilizador pressionar "Enter" */
            rede = LL_libertarRede(rede);
            cidade = libertarCidade(cidade);

            limparEcra();
            printf("Encerrando o programa...\n");
            return;
        }

        if (opcaoMenu <= 4) puts("");
        printf("\nPressione \"Enter\" para continuar...");
        getchar(); /* Aguarda o utilizador pressionar "Enter" */

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
    menuPrincipal();

    return 0;
}