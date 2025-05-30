/**
 * @file Grafos.c
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Implementação da biblioteca de Estruturas de Grafos
 * @version 2.0
 * @date 2025-05-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../include/Grafos.h"

/**
 * @brief Liberta a memória alocada para as estruturas da cidade.
 *
 * Liberta os diferentes componentes da cidade com base no nível especificado:
 * - Nível 1: Liberta as arestas de cada antena.
 * - Nível 2: Liberta também as antenas.
 * - Nível 3: Liberta toda a estrutura, incluindo o grafo.
 *
 * @param cidade Ponteiro para a estrutura do grafo que representa a cidade.
 * @param nivelLibertacao Nível de libertação de memória (1, 2 ou 3).
 *
 * @return int 0 se a libertação for efetuada com sucesso.  
 * @return int -1 se o ponteiro da cidade for inválido (NULL).
 */
int libertarEstruturas(Grafo **cidade, int nivelLibertacao)
{
    // Verifica se os apontadores são válidos
    if (cidade == NULL || *cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Liberta as antenas e arestas
    for (Vertice *antenaAtual = (*(*cidade)).primeiraAntena; antenaAtual != NULL; )
    {
        if (nivelLibertacao >= 1)
        {
            // Liberta as arestas
            for (Aresta *arestaAtual = (*antenaAtual).primeiraAresta; arestaAtual != NULL; )
            {
                Aresta *temp = arestaAtual;
                arestaAtual = (*arestaAtual).prox;
                free(temp);
            }

            // Limpa o ponteiro
            (*antenaAtual).primeiraAresta = NULL;
        }

        if (nivelLibertacao >= 2)
        {
            // Dá reset das variáveis da cidade
            (*(*cidade)).numAntenas = 0;
            (*(*cidade)).primeiraAntena = NULL;

            // Liberta as antenas
            Vertice *temp = antenaAtual;
            antenaAtual = (*antenaAtual).prox;
            free(temp);
        }
        else
        {
            antenaAtual = (*antenaAtual).prox;
        }
    }

    // Liberta a cidade
    if (nivelLibertacao >= 3)
    {
        free(*cidade);
        *cidade = NULL; /* Limpa o ponteiro */
    }

    return 0;
}

/**
 * @brief Reinicia a flag de visitação de todas as antenas da cidade.
 *
 * Percorre todas as antenas do grafo e define a flag `visitada` como falsa,
 * permitindo novas operações de procura ou travessia.
 *
 * @param cidade Ponteiro para a estrutura do grafo que representa a cidade.
 *
 * @return int 0 se a operação for concluída com sucesso.  
 * @return int -1 se o ponteiro da cidade for inválido (NULL).
 */
int resetVisitados(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Dá reset da flag "visitada" das antenas
    for (Vertice *antenaAtual = (*cidade).primeiraAntena; antenaAtual != NULL; antenaAtual = (*antenaAtual).prox)
    {
        (*antenaAtual).visitada = false;
    }

    return 0;
}

/**
 * @brief Carrega a cidade a partir de um ficheiro de texto.
 *
 * Lê o ficheiro linha a linha, identificando antenas através de caracteres entre 'A' e 'Z',
 * e adiciona essas antenas à cidade com as respetivas coordenadas (x, y).
 *
 * @param cidade Ponteiro para a estrutura do grafo que representa a cidade.
 * @param localizacaoFicheiro Caminho para o ficheiro de entrada.
 *
 * @return int 0 se a cidade for carregada com sucesso.  
 * @return int -1 se o ponteiro da cidade for inválido (NULL).  
 * @return int -6 se o ficheiro não puder ser aberto.  
 * @return int -404 se ocorrer um erro de alocação de memória ao adicionar uma antena.
 */
int carregarCidade(Grafo *cidade, const char *localizacaoFicheiro)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return -6; /* Retorna -6 caso não seja possível abrir o ficheiro */

    // Verifica a existência de antenas
    Vertice *ultimaAntena = NULL;
    char linha[MAX_X]; /* Máximo tamanho da linha para leitura */

    for (int y = 0; fgets(linha, sizeof(linha), ficheiro); y++)
    {
        for (int x = 0; linha[x] != '\n' && linha[x] != '\0'; x++)
        {
            if ((linha[x] >= 'A' && linha[x] <= 'Z'))
            {
                // Adiciona a nova antena
                if (adicionarAntenaCidadeCarregar(cidade, &ultimaAntena, linha[x], x, y) == -404)
                {
                    return -404; /* Return -404 caso não tenha sido possível alocar memória */
                }
            }
        }
    }

    fclose(ficheiro); /* Fecha o ficheiro */

    return 0;
}

/**
 * @brief Cria arestas entre antenas da cidade com base nas frequências.
 *
 * Interliga as antenas do grafo conforme os critérios fornecidos:
 * - Frequências iguais: liga antenas com a mesma frequência.
 * - Frequências diferentes: liga antenas com frequências diferentes.
 * - Pode opcionalmente evitar repetições de arestas.
 *
 * @param cidade Ponteiro para a estrutura do grafo da cidade.
 * @param FrequenciasIguais Se verdadeiro, liga antenas com a mesma frequência.
 * @param FrequenciasDiferentes Se verdadeiro, liga antenas com frequências diferentes.
 * @param verificarRepetidas Se verdadeiro, verifica se já existe ligação antes de adicionar.
 *
 * @return int 0 se as interligações forem feitas com sucesso.  
 * @return int -1 se o ponteiro da cidade for inválido (NULL).  
 * @return int -404 se ocorrer um erro de alocação de memória ao adicionar uma aresta.
 */
int interligarAntenas(Grafo *cidade, bool FrequenciasIguais, bool FrequenciasDiferentes, bool verificarRepetidas)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Cria arestas entre antenas com a mesma frequência, diferentes ou ambos
    for (Vertice *antenaAtual1 = (*cidade).primeiraAntena; antenaAtual1 != NULL; antenaAtual1 = (*antenaAtual1).prox)
    {
        for (Vertice *antenaAtual2 = (*antenaAtual1).prox; antenaAtual2 != NULL; antenaAtual2 = (*antenaAtual2).prox)
        {
            if ((FrequenciasIguais && (*antenaAtual1).frequencia == (*antenaAtual2).frequencia)
              || (FrequenciasDiferentes && (*antenaAtual1).frequencia != (*antenaAtual2).frequencia))
            {
                if (adicionarAresta(antenaAtual1, antenaAtual2, verificarRepetidas) == -404) return -404; /* Return -404 caso não tenha sido possível alocar memória */
                if (adicionarAresta(antenaAtual2, antenaAtual1, verificarRepetidas) == -404) return -404; /* Return -404 caso não tenha sido possível alocar memória */
            }
        }
    }

    return 0;
}

/**
 * @brief Cria e inicializa uma nova cidade (grafo).
 *
 * Aloca memória para a estrutura do grafo e inicializa o número de antenas a zero.
 *
 * @param novo Ponteiro duplo onde será armazenado o novo grafo criado.
 *
 * @return int 0 se a cidade for criada com sucesso.  
 * @return int -404 se ocorrer um erro de alocação de memória.
 */
int adicionarCidade(Grafo **novo)
{
    // Aloca o espaço na memória para a cidade
    *novo = malloc(sizeof(Grafo));

    // Verifica se foi possível alocar a memória
    if (*novo == NULL) return -404; /* Return -404 caso não tenha sido possível alocar memória */

    // Popula as variáveis
    (*(*novo)).numAntenas = 0;

    return 0;
}

/**
 * @brief Adiciona uma antena à cidade nas coordenadas especificadas.
 *
 * Verifica se já existe uma antena na posição dada e insere a nova antena na ordem correta.
 * Caso já exista uma antena na mesma posição, a frequência dessa antena é devolvida no parâmetro `frequencia`.
 *
 * @param cidade Ponteiro para a estrutura do grafo da cidade.
 * @param frequencia Ponteiro para a frequência a atribuir à nova antena; também usado para devolver a frequência se já existir uma antena na posição.
 * @param x Coordenada X da nova antena.
 * @param y Coordenada Y da nova antena.
 *
 * @return int 0 se a antena for adicionada com sucesso.  
 * @return int -1 se o ponteiro da cidade for inválido (NULL).  
 * @return int -7 se já existir uma antena nessa posição.  
 * @return int -9 se as coordenadas forem negativas.  
 * @return int -10 se a coordenada X exceder o máximo permitido.  
 * @return int -404 se ocorrer um erro de alocação de memória.
 */
int adicionarAntenaCidade(Grafo *cidade, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Filtra coordenadas negativas
    if (x < 0 || y < 0) return -9; /* Retorna -9 em caso de serem coordenadas negativas */

    // Filtra coordenadas de x acima do permitido
    if (x > MAX_X) return -10; /* Retorna -10 em caso de ultrapassar o máximo de leitura de X */

    // Verifica a posição da nova antena na lista (e se esta é repetida)
    int posicaoNaLista = 0; /* Mais otimizado do que libertar a memória em caso de return -3, mas mais complexo! */

    Vertice *anterior;
    Vertice *posterior = (*cidade).primeiraAntena;

    if (posterior == NULL)
    {
        posicaoNaLista = 1; /* Insere no início e fim (não existem antenas na lista) */
    }
    else if ((y < (*(*cidade).primeiraAntena).y) || (x <= (*(*cidade).primeiraAntena).x && y == (*(*cidade).primeiraAntena).y))
    {
        if (x == (*(*cidade).primeiraAntena).x && y == (*(*cidade).primeiraAntena).y)
        {
            *frequencia = (*(*cidade).primeiraAntena).frequencia; /* Guarda a frequência da antena ocupada */
            return -7; /* Retorna -7 em caso de já existir essa antena */
        }
        else
        {
            posicaoNaLista = 2; /* Insere no início */
        }
    }
    else
    {
        while (posterior != NULL && ((y > (*posterior).y) || (x >= (*posterior).x && y == (*posterior).y)))
        {
            anterior = posterior;
            posterior = (*posterior).prox;
        }

        if (x == (*anterior).x && y == (*anterior).y)
        {
            *frequencia = (*anterior).frequencia; /* Guarda a frequência da antena ocupada */
            return -7; /* Retorna -7 em caso de já existir essa antena */
        }
    }

    // Cria o espaço na memória para a nova antena
    Vertice *nova = malloc(sizeof(Vertice));

    // Verifica se foi possível alocar a memória
    if (nova == NULL) return -404; /* Return -404 caso não tenha sido possível alocar memória */

    // Popula as variáveis da estrutura
    (*nova).frequencia = *frequencia;
    (*nova).x = x;
    (*nova).y = y;
    (*nova).prox = NULL;

    // Insere a antena na posição correspondente
    if (posicaoNaLista == 0)
    {
        (*anterior).prox = nova;
        (*nova).prox = posterior;
    }
    if (posicaoNaLista == 1)
    {
        (*cidade).primeiraAntena = nova;
    }
    if (posicaoNaLista == 2)
    {
        (*nova).prox = (*cidade).primeiraAntena;
        (*cidade).primeiraAntena = nova;
    }

    (*cidade).numAntenas++;

    return 0;
}

/**
 * @brief Adiciona uma antena à cidade durante o carregamento a partir de ficheiro.
 *
 * Cria uma nova antena com os dados fornecidos e adiciona-a ao final da lista ligada.
 *
 * @param cidade Ponteiro para a estrutura do grafo da cidade.
 * @param ultimaAntena Ponteiro para o ponteiro da última antena adicionada.
 * @param frequencia Frequência da antena.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 *
 * @return int 0 se a antena for adicionada com sucesso.  
 * @return int -404 se ocorrer um erro de alocação de memória.
 */
int adicionarAntenaCidadeCarregar(Grafo *cidade, Vertice **ultimaAntena, char frequencia, int x, int y)
{
    // Cria o espaço na memória para a nova antena
    Vertice *nova = malloc(sizeof(Vertice));

    // Verifica se foi possível alocar a memória
    if (nova == NULL) return -404; /* Return -404 caso não tenha sido possível alocar memória */

    // Popula as variáveis da estrutura
    (*nova).frequencia = frequencia;
    (*nova).x = x;
    (*nova).y = y;
    (*nova).prox = NULL;

    // Adiciona na posição correta da lista
    if ((*cidade).primeiraAntena == NULL)
    {
        (*cidade).primeiraAntena = nova;
        *ultimaAntena = nova;
    }
    else
    {
        (*(*ultimaAntena)).prox = nova;
        *ultimaAntena = nova;
    }

    // Incrementa o número de antenas
    (*cidade).numAntenas++;

    return 0;
}

/**
 * @brief Adiciona uma aresta entre duas antenas.
 * 
 * @param inicio Apontador para a antena de origem.
 * @param destino Apontador para a antena de destino.
 * @param verificarRepetidas Booleano que indica se deve verificar arestas repetidas.
 * @return int 0 se for bem-sucedido, ou código de erro:
 * @return int -2 se os vértices forem iguais,
 * @return int -3 se o vértice de início for inválido,
 * @return int -4 se o vértice de destino for inválido,
 * @return int -7 se a aresta já existir (quando verificarRepetidas for verdadeiro),
 * @return int -404 se não for possível alocar memória.
 */
int adicionarAresta(Vertice *inicio, Vertice *destino, bool verificarRepetidas)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == destino) return -2; /* Retorna -2 caso sejam ambas inválidas */
    else if (inicio == NULL) return -3; /* Retorna -3 caso o início seja inválido */
    else if (destino == NULL) return -4; /* Retorna -4 caso o destino seja inválido */

    // Verifica se já existe a aresta (se verificarRepetidas for true)
    if (verificarRepetidas)
    {
        for (Aresta *arestaAtual = (*inicio).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            if ((*arestaAtual).destino == destino)
            {
                return -7; /* Return -7 caso já exista a aresta */
            }
        }
    }

    // Aloca o espaço na memória para a nova aresta
    Aresta *nova = malloc(sizeof(Aresta));

    // Verifica se foi possível alocar a memória
    if (nova == NULL) return -404; /* Return -404 caso não tenha sido possível alocar memória */

    // Atribui o destino
    (*nova).destino = destino;

    // Nova aresta é introduzida no início da lista ligada
    (*nova).prox = (*inicio).primeiraAresta;
    (*inicio).primeiraAresta = nova;

    return 0;
}

/**
 * @brief Remove uma antena da cidade.
 * 
 * @param cidade Apontador para o grafo da cidade.
 * @param frequencia Apontador para guardar a frequência da antena removida.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 * @return int 0 se for bem-sucedido, ou código de erro:
 * @return int -1 se o grafo for inválido,
 * @return int -9 se as coordenadas forem negativas,
 * @return int -10 se a coordenada X exceder o limite,
 * @return int -7 se a antena não for encontrada.
 */
int removerAntenaCidade(Grafo *cidade, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Filtra coordenadas negativas
    if (x < 0 || y < 0) return -9; /* Retorna -9 em caso de serem coordenadas negativas */

    // Filtra coordenadas de x acima do permitido
    if (x > MAX_X) return -10; /* Retorna -10 em caso de ultrapassar o máximo de leitura de X */

    Vertice *anterior = (*cidade).primeiraAntena;
    Vertice *atual = NULL;

    while (atual != NULL)
    {
        // Verifica se a antena atual é a que queremos remover
        if ((*atual).x == x && (*atual).y == y)
        {
            *frequencia = (*atual).frequencia; /* Guarda a frequência da antena removida */

            // Verifica a posição da antena (início ou meio/fim)
            if (anterior == NULL)
            {
                (*cidade).primeiraAntena = (*atual).prox;
            }
            else
            {
                (*anterior).prox = (*atual).prox;
            }

            (*cidade).numAntenas--;

            // Liberta a memória
            free(atual);

            return 0;
        }

        anterior = atual;
        atual = (*atual).prox;
    }

    return -7; /* Retorna -7 caso não tenha encontrado a antena */
}

/**
 * @brief Remove uma aresta entre duas antenas.
 * 
 * @param inicio Apontador para a antena de origem.
 * @param destino Apontador para a antena de destino.
 * @return int 0 se for bem-sucedido, ou código de erro:
 * @return int -2 se os vértices forem iguais,
 * @return int -3 se o vértice de início for inválido,
 * @return int -4 se o vértice de destino for inválido,
 * @return int -7 se a aresta não existir.
 */
int removerAresta(Vertice *inicio, Vertice *destino)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == destino) return -2; /* Retorna -2 caso sejam ambas inválidas */
    else if (inicio == NULL) return -3; /* Retorna -3 caso o início seja inválido */
    else if (destino == NULL) return -4; /* Retorna -4 caso o destino seja inválido */

    Aresta *anterior = NULL;
    Aresta *atual = (*inicio).primeiraAresta;

    while (atual != NULL)
    {
        // Verifica se a aresta atual é a que queremos remover
        if ((*atual).destino == destino)
        {
            // Verifica a posição da aresta (início ou meio/fim)
            if (anterior == NULL)
            {
                (*inicio).primeiraAresta = (*atual).prox;
            }
            else
            {
                (*anterior).prox = (*atual).prox;
            }

            // Liberta a memória
            free(atual);

            return 0;
        }

        anterior = atual;
        atual = (*atual).prox;
    }

    return -7; /* Retorna -7 caso não tenha encontrado a aresta */
}

/**
 * @brief Procura uma antena numa determinada posição.
 * 
 * @param cidade Apontador para o grafo da cidade.
 * @param antena Apontador onde será guardado o endereço da antena encontrada.
 * @param x Coordenada X da antena.
 * @param y Coordenada Y da antena.
 * @return int 0 se for bem-sucedido.
 * @return int -1 se o grafo for inválido.
 * @return int -9 se as coordenadas forem negativas.
 * @return int -10 se a coordenada X exceder o limite.
 * @return int -7 se a antena não for encontrada.
 */
int procurarAntena(Grafo *cidade, Vertice **antena, int x, int y)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return NULL caso não exista a cidade */

    // Filtra coordenadas negativas
    if (x < 0 || y < 0) return -9; /* Retorna -9 em caso de serem coordenadas negativas */

    // Filtra coordenadas de x acima do permitido
    if (x > MAX_X) return -10; /* Retorna -10 em caso de ultrapassar o máximo de leitura de X */

    for (Vertice *antenaAtual = (*cidade).primeiraAntena; antenaAtual != NULL; antenaAtual = (*antenaAtual).prox)
    {
        if (x == (*antenaAtual).x && y == (*antenaAtual).y)
        {
            *antena = antenaAtual;
            return 0;
        }
    }

    return -7; /* Retorna -7 caso não tenha encontrado a antena */
}

/**
 * @brief Realiza uma procura em profundidade (DFS) a partir de uma antena.
 * 
 * @param inicio Apontador para a antena de partida.
 * @return int 0 se for bem-sucedido, ou -3 se a antena de início for inválida.
 */
int procurarProfundidade(Vertice *inicio)
{
    // Verifica se a antena início é válida
    if (inicio == NULL) return -3; /* Return -3 caso não seja válida a antena início */

    // Marca como visitada e imprime
    (*inicio).visitada = true;
    printf("'%c'(%d, %d)\n", (*inicio).frequencia, (*inicio).x, (*inicio).y);

    // Percorre de forma recursiva as antenas adjacentes não visitadas
    for (Aresta *arestaAtual = (*inicio).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
    {
        if (!(*(*arestaAtual).destino).visitada)
        {
            procurarProfundidade((*arestaAtual).destino);
        }
    }

    return 0;
}

/**
 * @brief Realiza uma procura em largura (BFS) a partir de uma antena.
 * 
 * @param inicio Apontador para a antena de partida.
 * @param numeroAntenas Tamanho do array da lista.
 * @return int 0 se for bem-sucedido,
 * @return int -3 se a antena de início for inválida,
 * @return int -8 se a lista auxiliar exceder o tamanho máximo permitido (overflow).
 */
int procurarLargura(Vertice *inicio, int numeroAntenas)
{
    // Verifica se a antena início é válida
    if (inicio == NULL) return -3; /* Return -3 caso não seja válida a antena início */

    // Cria a lista e movimentos de lista
    Vertice *lista[numeroAntenas];
    int anterior = 0, posterior = 0;

    // Inicializa lista com vértice inicial
    lista[anterior++] = inicio;
    (*inicio).visitada = true;

    // Percorre em largura as antenas
    while (anterior > posterior)
    {
        Vertice *antenaAtual = lista[posterior++];
        printf("'%c'(%d, %d)\n", (*antenaAtual).frequencia, (*antenaAtual).x, (*antenaAtual).y);

        // Percorre os adjacentes não visitados
        for (Aresta *arestaAtual = (*antenaAtual).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            Vertice *destino = (*arestaAtual).destino;

            // Adiciona antenas adjacentes não visitadas à fila
            if (!(*destino).visitada)
            {
                (*destino).visitada = true;
                lista[anterior++] = destino;

                // Proteção contra overflow da lista (impossível de acontecer)
                if (anterior >= numeroAntenas)
                {
                    return -8; /* Retorna -8 caso a lista esteja cheia (overflow) */
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Procura e imprime todos os caminhos possíveis entre duas antenas.
 * 
 * Esta função inicializa um array de ponteiros para armazenar o caminho atual
 * e chama uma função recursiva auxiliar que implementa backtracking para encontrar
 * e imprimir todos os caminhos possíveis entre a antena de origem e a antena de destino.
 * 
 * @param inicio Apontador para a antena de origem.
 * @param destino Apontador para a antena de destino.
 * @param numeroAntenas Número total de antenas, utilizado para definir o tamanho máximo do array de caminho.
 * @return int 1 se pelo menos um caminho for encontrado e impresso,  
 * @return int 0 se nenhum caminho for encontrado,  
 * @return int -2 se as antenas de início e fim forem iguais (inválidas),  
 * @return int -3 se a antena de início for inválida (NULL),  
 * @return int -4 se a antena de destino for inválida (NULL).
 */
int procurarCaminhos(Vertice *inicio, Vertice *destino, int numeroAntenas)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == destino) return -2; /* Retorna -2 caso sejam ambas inválidas */
    else if (inicio == NULL) return -3; /* Retorna -3 caso o início seja inválido */
    else if (destino == NULL) return -4; /* Retorna -4 caso o destino seja inválido */

    // Array para guardar o caminho atual
    Vertice *caminho[numeroAntenas];

    // Chama a função recursiva de procurar caminhos
    return procurarCaminhosRecursiva(inicio, destino, caminho, 0);
}

/**
 * @brief Função auxiliar recursiva que procura caminhos entre duas antenas.
 * 
 * Esta função implementa o algoritmo de backtracking para explorar todos os caminhos
 * possíveis entre dois vértices de um grafo. Sempre que o destino é atingido,
 * o caminho atual é impresso no ecrã.
 * 
 * @param inicio Apontador para o vértice atual (antena onde a procura está).
 * @param destino Apontador para o vértice de destino (antena a atingir).
 * @param caminho Array de apontadores para armazenar o caminho atual.
 * @param tamanho Tamanho atual do caminho (número de antenas no caminho até agora).
 * @return int 1 se pelo menos um caminho for encontrado e impresso,  
 * @return int 0 se nenhum caminho for encontrado a partir deste ramo.
 */
int procurarCaminhosRecursiva(Vertice *inicio, Vertice *destino, Vertice **caminho, int tamanho)
{
    int imprimiu = 0;

    // Marca o vértice atual como visitada e adiciona ao caminho
    (*inicio).visitada = true;
    caminho[tamanho++] = inicio;

    // Se o destino for alcançado, imprimime o caminho completo
    if (inicio == destino)
    {
        for (int i = 0; i < tamanho; i++)
        {
            printf("'%c'(%d, %d)", (*caminho[i]).frequencia, (*caminho[i]).x, (*caminho[i]).y);
            if (i == tamanho - 1) printf("\n");
            else printf(" -> ");
        }

        imprimiu = 1;
    }
    else
    {
        // Percorre de forma recursiva as antenas adjacentes não visitadas
        for (Aresta *arestaAtual = (*inicio).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            if (!(*(*arestaAtual).destino).visitada)
            {
                if (procurarCaminhosRecursiva((*arestaAtual).destino, destino, caminho, tamanho) == 1) imprimiu = 1;
            }
        }
        
    }

    // Dá reset da variável "visitada" para poder procurar outro caminho
    (*inicio).visitada = false;

    return imprimiu;
}

/**
 * @brief Lista as interseções entre antenas de duas frequências distintas.
 * 
 * Para cada par de antenas com frequências diferentes, calcula e imprime o ponto médio entre ambas.
 * 
 * @param cidade Apontador para o grafo que representa a cidade.
 * @param frequencia1 Carácter da primeira frequência a considerar.
 * @param frequencia2 Carácter da segunda frequência a considerar.
 * @return int 1 se forem encontradas e listadas interseções,
 * @return int 0 se não forem encontradas interseções,
 * @return int -1 se o grafo da cidade for inválido,
 * @return int -5 se as duas frequências forem iguais.
 */
int listarIntersecoes(Grafo *cidade, char frequencia1, char frequencia2)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Verifica se as frequências são diferentes
    if (frequencia1 == frequencia2) return -5; /* Return -5 caso as frequências sejam iguais */

    int imprimiu = 0;

    // Verifica as interseções
    for (Vertice *antenaAtual1 = (*cidade).primeiraAntena; antenaAtual1 != NULL; antenaAtual1 = (*antenaAtual1).prox)
    {
        // Verifica se corresponde à 1.ª frequência
        if ((*antenaAtual1).frequencia == frequencia1)
        {
            for (Vertice *antenaAtual2 = (*cidade).primeiraAntena; antenaAtual2 != NULL; antenaAtual2 = (*antenaAtual2).prox)
            {
                // Verifica se corresponde à 2.ª frequência
                if ((*antenaAtual2).frequencia == frequencia2)
                {
                    imprimiu = 1;

                    // Calcula a interseção
                    int intersecaoX = ((*antenaAtual1).x + (*antenaAtual2).x) / 2;
                    int intersecaoY = ((*antenaAtual1).y + (*antenaAtual2).y) / 2;

                    // Imprime os pontos de interseção
                    printf("'%c'(%d, %d) -x- '%c'(%d, %d) > (%d, %d)\n",
                      (*antenaAtual1).frequencia, (*antenaAtual1).x, (*antenaAtual1).y,
                      (*antenaAtual2).frequencia, (*antenaAtual2).x, (*antenaAtual2).y,
                      intersecaoX, intersecaoY);
                }
            }
        }
    }

    return imprimiu;
}

/**
 * @brief Lista todas as antenas existentes na cidade.
 * 
 * Imprime todas as antenas (vértices) presentes no grafo, incluindo a sua frequência e coordenadas.
 * 
 * @param cidade Apontador para o grafo que representa a cidade.
 * @return int 0 se for bem-sucedido, ou -1 se o grafo da cidade for inválido.
 */
int listarAntenas(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return -1; /* Return -1 caso não exista a cidade */

    // Imprime as antenas
    for (Vertice *atual = (*cidade).primeiraAntena; atual != NULL; atual = (*atual).prox)
    {
        printf("'%c'(%d, %d)\n", (*atual).frequencia, (*atual).x, (*atual).y);
    }

    return 0;
}

/**
 * @brief Lista todas as arestas de uma antena.
 *
 * Esta função percorre e imprime todas as arestas associadas a uma antena específica,
 * apresentando a ligação entre a antena de origem e as antenas de destino.
 * 
 * @param antena Apontador para a antena cujas arestas se pretende listar.
 * 
 * @return int 1 se existirem arestas e forem impressas com sucesso.  
 * @return int 0 se a antena for válida mas não tiver arestas.  
 * @return int -3 se o apontador para a antena for inválido (NULL).
 */
int listarArestasAntena(Vertice *antena)
{
    // Verifica se o apontador antena é valido
    if (antena == NULL) return -3; /* Retorna -3 caso a antena seja inválida */

    int imprimiu = 0;

    if ((*antena).primeiraAresta != NULL)
    {
        imprimiu = 1;

        for (Aresta *arestaAtual = (*antena).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            printf("'%c'(%d, %d) -> '%c'(%d, %d)\n",
              (*antena).frequencia, (*antena).x, (*antena).y,
              (*(*arestaAtual).destino).frequencia, (*(*arestaAtual).destino).x, (*(*arestaAtual).destino).y);
        }
    }

    return imprimiu;
}