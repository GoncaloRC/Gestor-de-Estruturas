#include "../include/Grafos.h"
#include "../include/Menu.h"

/**
 * @brief Liberta toda a memória associada a uma cidade.
 *
 * Esta função remove todas as arestas, antenas e a própria cidade da memória.
 * Deve ser chamada quando já não for necessária a estrutura para evitar fugas de memória.
 *
 * @param cidade Apontador para a cidade a libertar.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o apontador for inválido.
 */
int libertarCidade(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Liberta as arestas
    for (int i = 0; i < (*cidade).numAntenas; i++)
    {
        Aresta *atual = (*cidade).antenas[i].adj;

        while (atual != NULL)
        {
            Aresta *temp = atual;
            atual = (*atual).prox;

            free(temp);
        }
    }

    // Liberta as antenas
    free((*cidade).antenas);

    // Liberta a cidade
    free(cidade);

    return 0;
}

/**
 * @brief Cria uma nova cidade com um determinado número de antenas.
 *
 * Aloca a memória necessária para representar a cidade e as suas antenas, inicializando-as.
 *
 * @param numAntenas Número total de antenas que a cidade deve conter.
 * @return Grafo* Apontador para a nova cidade criada.
 */
Grafo *adicionarCidade(int numAntenas)
{
    // Aloca o espaço na memória para a cidade e as suas antenas
    Grafo *novo = malloc(sizeof(Grafo));

    // Atribui o número de antenas e aloca o espaço na memória com inicialização
    (*novo).numAntenas = numAntenas;
    (*novo).antenas = calloc(numAntenas, sizeof(Vertice));

    return novo; /* Retorna o apontador dessa memória */
}

/**
 * @brief Adiciona uma aresta entre duas antenas.
 *
 * Cria uma ligação (aresta) entre duas antenas da cidade, do vértice de início para o de destino.
 *
 * @param cidade Apontador para a cidade onde se encontram as antenas.
 * @param inicio Índice da antena de origem.
 * @param destino Índice da antena de destino.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o apontador da cidade for inválido.
 */
int adicionarAresta(Grafo *cidade, int inicio, int destino)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Aloca o espaço na memória para a nova aresta
    Aresta *nova = malloc(sizeof(Aresta));

    // Atribui o destino
    (*nova).destino = destino;

    // Nova aresta é introduzida no início da lista ligada
    (*nova).prox = (*cidade).antenas[inicio].adj;
    (*cidade).antenas[inicio].adj = nova;

    return 0;
}

/**
 * @brief Carrega uma cidade a partir de um ficheiro de texto.
 *
 * Lê as antenas de um ficheiro, identifica as suas posições e frequências, 
 * cria a cidade correspondente e estabelece arestas entre antenas com a mesma frequência.
 *
 * @param cidade Apontador duplo para onde será guardada a cidade carregada.
 * @param localizacaoFicheiro Caminho para o ficheiro com os dados da cidade.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o ficheiro não puder ser aberto.
 */
int carregarCidade(Grafo **cidade, const char *localizacaoFicheiro)
{
    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return -1; /* Retorna -1 caso não seja possível abrir o ficheiro */

    int numAntenas = 0;
    char linha[MAX_COLUNAS];

    Vertice *antenasTemp = malloc(MAX_LINHAS * MAX_COLUNAS * sizeof(Vertice)); /* Evita overflow, mas pode tornar-se excessivamente grande! O uso de uma lista dinâmica para cidades maiores seria beneficial */

    // Lê o ficheiro e grava a informação das antenas no apontador temporário
    for (int y = 0; fgets(linha, sizeof(linha), ficheiro); y++)
    {
        for (int x = 0; x < strlen(linha); x++)
        {
            if (linha[x] >= 'A' && linha[x] <= 'Z')
            {
                Vertice antena;
                antena.frequencia = linha[x];
                antena.x = x;
                antena.y = y;
                antena.adj = NULL;
                antena.visitada = false;
                antenasTemp[numAntenas++] = antena;
            }
        }
    }

    fclose(ficheiro); /* Fecha o ficheiro */

    // Copia as antenas temporárias para a cidade e liberta a memória delas
    *cidade = adicionarCidade(numAntenas);
    memcpy((*(*cidade)).antenas, antenasTemp, numAntenas * sizeof(Vertice));

    free(antenasTemp);

    // Cria arestas entre antenas com a mesma frequência
    for (int i = 0; i < (*(*cidade)).numAntenas; i++)
    {
        for (int j = i + 1; j < (*(*cidade)).numAntenas; j++)
        {
            if ((*(*cidade)).antenas[i].frequencia == (*(*cidade)).antenas[j].frequencia)
            {
                adicionarAresta(*cidade, i, j);
                adicionarAresta(*cidade, j, i);
            }
        }
    }

    return 0;
}

/**
 * @brief Reinicia a flag de "visitado" em todas as antenas da cidade.
 *
 * Útil para preparar a cidade para uma nova pesquisa em profundidade ou largura.
 *
 * @param cidade Apontador para a cidade a reiniciar.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o apontador for inválido.
 */
int resetVisitados(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Dá reset da flag "visitada" da antena
    for (int i = 0; i < (*cidade).numAntenas; i++)
    {
        (*cidade).antenas[i].visitada = false;
    }

    return 0;
}

/**
 * @brief Realiza uma pesquisa em profundidade (DFS) a partir de uma antena.
 *
 * Percorre recursivamente todas as antenas adjacentes que ainda não foram visitadas,
 * imprimindo as suas coordenadas.
 *
 * @param cidade Apontador para a cidade onde será feita a pesquisa.
 * @param antena Índice da antena por onde iniciar a pesquisa.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o apontador for inválido.
 * @return int Retorna -2 se a antena for inválida.
 * @return int Retorna -3 se não existirem antenas.
 *
 */
int procurarProfundidade(Grafo *cidade, int antena)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Verifica se a antena é válida
    if (antena < 0 || antena > (*cidade).numAntenas) return -2; /* Return -1 caso não seja válida a antena */

    // Verifica se há antenas
    if ((*cidade).numAntenas < 1) return -3; /* Return -1 caso não haja antenas */

    // Marca como visitada e imprime
    (*cidade).antenas[antena].visitada = true;
    printf("(%d, %d)\n", (*cidade).antenas[antena].x, (*cidade).antenas[antena].y);

    // Percorre de forma recursiva as antenas adjacentes não visitadas
    for (Aresta *arestaAtual = (*cidade).antenas[antena].adj; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
    {
        if (!(*cidade).antenas[(*arestaAtual).destino].visitada)
        {
            procurarProfundidade(cidade, (*arestaAtual).destino);
        }
    }

    return 0;
}

/**
 * @brief Realiza uma pesquisa em largura (BFS) a partir de uma antena.
 *
 * Explora todas as antenas acessíveis a partir da antena inicial de forma iterativa,
 * imprimindo as coordenadas à medida que são visitadas.
 *
 * @param cidade Apontador para a cidade onde será feita a pesquisa.
 * @param inicio Índice da antena por onde iniciar a pesquisa.
 * @return int Retorna 0 em caso de sucesso.
 * @return int Retorna -1 se o apontador for inválido.
 * @return int Retorna -2 se a antena for inválida.
 * @return int Retorna -3 se não existirem antenas.
 *
 */
int procurarLargura(Grafo *cidade, int inicio)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Verifica se a antena início é válida
    if (inicio < 0 || inicio > (*cidade).numAntenas) return -2; /* Return -1 caso não seja válida a antena início */

    // Verifica se há antenas
    if ((*cidade).numAntenas < 1) return -3; /* Return -1 caso não haja antenas */

    int fila[MAX_LINHAS * MAX_COLUNAS]; /* Evita overflow, mas pode tornar-se excessivamente grande! O uso de uma lista dinâmica para cidades maiores seria beneficial */
    int anterior = 0, posterior = 0;

    // Coloca a antena inicial na fila e marca como visitada
    fila[anterior++] = inicio;
    (*cidade).antenas[inicio].visitada = true;

    // Percorre em largura as antenas
    while (posterior < anterior)
    {
        int atual = fila[posterior++];
        printf("(%d, %d)\n", (*cidade).antenas[atual].x, (*cidade).antenas[atual].y);

        // Adiciona antenas adjacentes não visitadas à fila
        for (Aresta *arestaAtual = (*cidade).antenas[atual].adj; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            if (!(*cidade).antenas[(*arestaAtual).destino].visitada)
            {
                fila[anterior++] = (*arestaAtual).destino;
                (*cidade).antenas[(*arestaAtual).destino].visitada = true;
            }
        }
    }

    return 0;
}

/**
 * @brief Procura e imprime todos os caminhos entre duas antenas.
 *
 * Utiliza pesquisa em profundidade para encontrar e imprimir todos os caminhos possíveis
 * entre duas antenas distintas, mostrando as suas coordenadas.
 *
 * @param cidade Apontador para a cidade onde será feita a pesquisa.
 * @param inicio Índice da antena de partida.
 * @param destino Índice da antena de chegada.
 * @param caminho Array onde será guardado o caminho atual.
 * @param tamanho Comprimento atual do caminho (normalmente começa a 0).
 * @return int Retorna 1 (imprimiu) se pelo menos um caminho for encontrado.
 * @return int Retorna 0 (imprimiu) se não houver caminho.
 * @return int Retorna -1 se não existir a cidade.
 * @return int Retorna -2 se não for válida a antena início.
 * @return int Retorna -3 se não for válida a antena destino.
 * @return int Retorna -4 se não for válida a antena início e destino.
 * @return int Retorna -5 se não houver antenas.
 *
 */
int procurarCaminhos(Grafo *cidade, int inicio, int destino, int* caminho, int tamanho)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Verifica se a antena início é válida (e, possivelmente, a antena destino)
    if (inicio < 0 || inicio > (*cidade).numAntenas)
    {
        if (destino < 0 || destino > (*cidade).numAntenas) return -4; /* Return -4 caso não seja válida a antena início e destino */
        else return -2; /* Return -2 caso não seja válida a antena início */
    }

    // Verifica se a antena destino é válida
    if (destino < 0 || destino > (*cidade).numAntenas) return -3; /* Return -3 caso não seja válida a antena destino */

    // Verifica se há antenas suficientes para haver caminhos
    if ((*cidade).numAntenas < 1) return -5; /* Return -5 caso não haja antenas */

    int imprimiu = 0;

    // Marca o vértice atual como visitada e adiciona ao caminho
    (*cidade).antenas[inicio].visitada = true;
    caminho[tamanho++] = inicio;

    // Se o destino for alcançado, imprimime o caminho completo
    if (inicio == destino)
    {
        for (int i = 0; i < tamanho; i++)
        {
            printf("(%d, %d)", (*cidade).antenas[caminho[i]].x, (*cidade).antenas[caminho[i]].y);

            if (i == tamanho - 1) printf("\n");
            else printf(" -> ");
        }

        imprimiu = 1;
    }

    // Percorre de forma recursiva as antenas adjacentes não visitadas
    else
    {
        for (Aresta *arestaAtual = (*cidade).antenas[inicio].adj; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            if (!(*cidade).antenas[(*arestaAtual).destino].visitada)
            {
                imprimiu = procurarCaminhos(cidade, (*arestaAtual).destino, destino, caminho, tamanho);
            }
        }
    }

    // Dá reset da variável "visitada" para poder procurar outro caminho
    (*cidade).antenas[inicio].visitada = false;

    return imprimiu;
}

/**
 * @brief Lista todos os pontos de interseção entre antenas de duas frequências distintas.
 *
 * Para cada par de antenas com frequências diferentes, calcula e imprime o ponto médio
 * (interseção virtual) entre elas.
 *
 * @param cidade Apontador para a cidade com as antenas.
 * @param frequencia1 Primeira frequência a considerar.
 * @param frequencia2 Segunda frequência a considerar.
 * @return int Retorna 1 (imprimiu) se foram impressas interseções.
 * @return int Retorna 0 (imprimiu) se não foram impressas interseções.
 * @return int Retorna -1 se não existir a cidade.
 * @return int Retorna -2 se as frequências forem iguais.
 * 
 */
int listarIntersecoes(Grafo *cidade, char frequencia1, char frequencia2)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Verifica se as frequências são diferentes
    if (frequencia1 == frequencia2) return -2; /* Return -2 caso as frequências sejam iguais */

    int imprimiu = 0;

    // Verifica as interseções
    for (int i = 0; i < (*cidade).numAntenas; i++)
    {
        if ((*cidade).antenas[i].frequencia == frequencia1)
        {
            for (int j = 0; j < (*cidade).numAntenas; j++)
            {
                if ((*cidade).antenas[j].frequencia == frequencia2)
                {
                    // Calcula a interseção
                    int intersecaoX = ((*cidade).antenas[i].x + (*cidade).antenas[j].x) / 2;
                    int intersecaoY = ((*cidade).antenas[i].y + (*cidade).antenas[j].y) / 2;

                    // Imprime os pontos de interseção
                    printf("(%d, %d) - (%d, %d) -> (%d, %d)\n",
                     (*cidade).antenas[i].x, (*cidade).antenas[i].y,
                     (*cidade).antenas[j].x, (*cidade).antenas[j].y,
                     intersecaoX, intersecaoY);

                    imprimiu = 1;
                }
            }
        }
    }

    return imprimiu;
}