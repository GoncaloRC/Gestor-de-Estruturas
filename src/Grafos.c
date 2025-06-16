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
 * @brief Cria e inicializa uma nova cidade (grafo).
 * 
 * @note Utiliza `calloc` para garantir que todos os campos são inicializados a 0.
 * 
 * @return Ponteiro para a cidade alocada.
 * @return NULL se a alocação falhar.
 */
Grafo *criarCidade()
{
    // Aloca o espaço na memória para a cidade (e inicializa as variáveis)
    Grafo *novo = calloc(1, sizeof(Grafo));

    return novo;
}

/**
 * @brief Cria e inicializa uma nova antena (vértice) com os parâmetros especificados.
 * 
 * @param frequencia Carácter que identifica a frequência da antena (ex: 'A', 'B').
 * @param x Coordenada horizontal da antena.
 * @param y Coordenada vertical da antena.
 * 
 * @note Campos `visitada`, `primeiraAresta` e `prox` são inicializados a false/NULL.
 * 
 * @return Ponteiro para a antena alocada.
 * @return NULL se a alocação falhar.
 */
Vertice *criarAntena(char frequencia, int x, int y)
{
    // Aloca o espaço na memória para a cidade (e inicializa as variáveis)
    Vertice *novo = malloc(sizeof(Vertice));

    // Verifica se foi possível alocar a memória
    if (novo == NULL) return NULL;

    // Popula as variáveis da estrutura
    (*novo).frequencia = frequencia;
    (*novo).x = x;
    (*novo).y = y;
    (*novo).visitada = false;
    (*novo).primeiraAresta = NULL;
    (*novo).prox = NULL;

    return novo;
}

/**
 * @brief Cria uma nova aresta que liga a um vértice de destino.
 * 
 * @param destino Ponteiro para o vértice de destino da aresta.
 * 
 * @note O campo `prox` é inicializado a NULL.
 * 
 * @return Ponteiro para a aresta alocada.
 * @return NULL se a alocação falhar.
 */
Aresta *criarAresta(Vertice *destino)
{
    // Aloca o espaço na memória para a cidade (e inicializa as variáveis)
    Aresta *novo = malloc(sizeof(Aresta));

    // Verifica se foi possível alocar a memória
    if (novo == NULL) return NULL;

    // Popula as variáveis da estrutura
    (*novo).destino = destino;
    (*novo).prox = NULL;

    return novo;
}

/**
 * @brief Liberta toda a memória associada a uma cidade (grafo), incluindo antenas e arestas.
 * 
 * @param cidade Ponteiro para a cidade a libertar. Se for NULL, a função não tem efeito.
 * 
 * @note Esta função chama "libertarAntenas2()" para libertar todas as antenas e arestas.
 *       O ponteiro da cidade é libertado e os campos são resetados.
 * 
 * @return Sempre retorna NULL, para facilitar a reatribuição do ponteiro original.
 */
Grafo *libertarCidade(Grafo *cidade)
{
    // Verifica se os apontadores são válidos
    if (cidade == NULL) return NULL; /* Return NULL caso não exista a cidade */

    // Liberta as antenas (e, posteriormente, as arestas)
    libertarAntenas2(cidade);

    // Liberta a cidade
    free(cidade);

    return NULL;
}

/**
 * @brief Liberta todas as antenas e arestas de uma cidade, resetando a sua estrutura.
 * 
 * @param cidade Ponteiro para a cidade cujas antenas serão libertadas.
 * 
 * @note Percorre a lista de antenas, libertando cada aresta associada antes de libertar a antena.
 *       No final, reseta o contador de antenas ("numAntenas") e o ponteiro para a primeira antena.
 * 
 * @return Retorna 0 em caso de sucesso
 * @return "ERRO_CIDADE_PONTEIRO_INVALIDO" se "cidade" for NULL.
 */
int libertarAntenas2(Grafo *cidade)
{
    // Verifica se os apontadores são válidos
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    for (Vertice *antenaAtual = (*cidade).primeiraAntena; antenaAtual != NULL; )
    {
        // Liberta as arestas
        for (Aresta *arestaAtual = (*antenaAtual).primeiraAresta; arestaAtual != NULL; )
        {
            Aresta *temp = arestaAtual;
            arestaAtual = (*arestaAtual).prox;
            free(temp);
        }

        // Liberta as antenas
        Vertice *temp = antenaAtual;
        antenaAtual = (*antenaAtual).prox;
        free(temp);
    }

    // Dá reset das variáveis da cidade
    (*cidade).numAntenas = 0;
    (*cidade).primeiraAntena = NULL;

    return 0;
}

/**
 * @brief Liberta todas as arestas das antenas de uma cidade, mas mantém as antenas intactas.
 * 
 * @param cidade Ponteiro para a cidade cujas arestas serão libertadas.
 * 
 * @note Percorre todas as antenas, libertando as suas arestas e definindo `primeiraAresta` como NULL.
 *       As antenas não são libertadas, apenas as suas ligações (arestas).
 * 
 * @return Retorna 0 em caso de sucesso
 * @return "ERRO_CIDADE_PONTEIRO_INVALIDO" se "cidade" for NULL.
 */
int libertarArestas(Grafo *cidade)
{
    // Verifica se os apontadores são válidos
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    for (Vertice *antenaAtual = (*cidade).primeiraAntena; antenaAtual != NULL; antenaAtual = (*antenaAtual).prox)
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
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o ponteiro da cidade for inválido (NULL).
 */
int resetVisitados(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

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
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o ponteiro da cidade for inválido (NULL).  
 * @return int ERRO_ABRIR_FICHEIRO se o ficheiro não puder ser aberto.  
 * @return int -404 se ocorrer um erro de alocação de memória ao adicionar uma antena.
 */
int carregarCidade(Grafo *cidade, const char *localizacaoFicheiro)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return ERRO_ABRIR_FICHEIRO;

    // Variaveis de otimização e retorno de erro
    Vertice *ultimaAntena = NULL;
    int erro;

    // Variáveis necessárias para leitura (com getline)
    char *linha = NULL;
    size_t tamanho = 0;
    ssize_t lidos;

    // Percorre o ficheiro e verifica a existência de antenas
    for (int y = 0; (lidos = getline(&linha, &tamanho, ficheiro)) != -1; y++)
    {
        for (int x = 0; x < lidos; x++)
        {
            if (linha[x] >= 'A' && linha[x] <= 'Z')
            {
                // Cria a nova antena e atualiza a última antena
                ultimaAntena = adicionarAntenaFim(cidade, ultimaAntena, linha[x], x, y, &erro);

                // Verifica se foi possível alocar a memória
                if (ultimaAntena == NULL)
                {
                    free(linha);
                    fclose(ficheiro);
                    return erro;
                }
            }
        }
    }

    free(linha);
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
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o ponteiro da cidade for inválido (NULL).  
 * @return int -404 se ocorrer um erro de alocação de memória ao adicionar uma aresta.
 */
int interligarAntenas(Grafo *cidade, bool FrequenciasIguais, bool FrequenciasDiferentes, bool verificarRepetidas)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    // Cria arestas entre antenas com a mesma frequência, diferentes ou ambos
    for (Vertice *antenaAtual1 = (*cidade).primeiraAntena; antenaAtual1 != NULL; antenaAtual1 = (*antenaAtual1).prox)
    {
        for (Vertice *antenaAtual2 = (*antenaAtual1).prox; antenaAtual2 != NULL; antenaAtual2 = (*antenaAtual2).prox)
        {
            if ((FrequenciasIguais && (*antenaAtual1).frequencia == (*antenaAtual2).frequencia)
              || (FrequenciasDiferentes && (*antenaAtual1).frequencia != (*antenaAtual2).frequencia))
            {
                if (adicionarAresta(antenaAtual1, antenaAtual2, verificarRepetidas) == -404) return ERRO_ALOCACAO_MEMORIA;
                if (adicionarAresta(antenaAtual2, antenaAtual1, verificarRepetidas) == -404) return ERRO_ALOCACAO_MEMORIA;
            }
        }
    }

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
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o ponteiro da cidade for inválido (NULL).  
 * @return int ERRO_OBJETO_JA_EXISTE se já existir uma antena nessa posição.
 * @return int -404 se ocorrer um erro de alocação de memória.
 */
int adicionarAntenaOrdenada(Grafo *cidade, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    // Cria os apontadores para navegar a lista ligada
    Vertice *anterior = NULL;
    Vertice *posterior = (*cidade).primeiraAntena;

    // Insere no início da lista
    if (posterior == NULL || (y < (*posterior).y) || (x <= (*posterior).x && y == (*posterior).y))
    {
        // Verifica se a antena é repetida
        if (x == (*posterior).x && y == (*posterior).y)
        {
            *frequencia = (*posterior).frequencia; /* Guarda a frequência da antena ocupada */
            return ERRO_OBJETO_JA_EXISTE;
        }

        // Cria a nova antena e popula as variáveis da estrutura
        Vertice *nova = criarAntena(*frequencia, x, y);

        // Verifica se foi possível alocar a memória
        if (nova == NULL) return ERRO_ALOCACAO_MEMORIA;

        // Insere a antena na posição correspondente da lista
        (*nova).prox = (*cidade).primeiraAntena;
        (*cidade).primeiraAntena = nova;
    }
    // Insere no meio e fim da lista
    else
    {
        while (posterior != NULL && ((y > (*posterior).y) || (x > (*posterior).x && y == (*posterior).y)))
        {
            anterior = posterior;
            posterior = (*posterior).prox;
        }

        if (posterior != NULL && x == (*posterior).x && y == (*posterior).y)
        {
            *frequencia = (*posterior).frequencia; /* Guarda a frequência da antena ocupada */
            return ERRO_OBJETO_JA_EXISTE;
        }

        // Aloca o espaço na memória para a nova antena
        Vertice *nova = criarAntena(*frequencia, x, y);

        // Verifica se foi possível alocar a memória
        if (nova == NULL) return ERRO_ALOCACAO_MEMORIA;

        // Insere a antena na posição correspondente da lista
        (*anterior).prox = nova;
        (*nova).prox = posterior;
    }

    (*cidade).numAntenas++;

    return 0;
}

/**
 * @brief Adiciona uma nova antena no fim da lista ligada do grafo.
 * 
 * @param cidade Ponteiro para a estrutura do grafo onde a antena será inserida.
 * @param ultimaAntena Ponteiro para a última antena da lista (pode ser NULL se a lista estiver vazia).
 * @param frequencia Carácter que identifica a frequência da antena (ex: 'A', 'B').
 * @param x Coordenada horizontal da antena.
 * @param y Coordenada vertical da antena.
 * @param erro Ponteiro para inteiro onde será armazenado o código de erro, caso ocorra.
 * 
 * @note Esta função:
 *       - Verifica a validade dos parâmetros de entrada.
 *       - Aloca memória para a nova antena.
 *       - Atualiza a lista ligada e o contador de antenas.
 *       - Atribui códigos de erro específicos em caso de falha.
 * 
 * @return ponteiro para a nova antena em caso de sucesso.
 * @return NULL em caso de erro, e o código de erro é armazenado em "*erro".
 */
Vertice *adicionarAntenaFim(Grafo *cidade, Vertice *ultimaAntena, char frequencia, int x, int y, int *erro)
{
    // Verifica se o apontador é válido
    if (cidade == NULL)
    {
        *erro = ERRO_CIDADE_PONTEIRO_INVALIDO;
        return NULL;
    }

    // Cria o espaço na memória para a nova antena
    Vertice *nova = criarAntena(frequencia, x, y);

    // Verifica se foi possível alocar a memória
    if (nova == NULL)
    {
        *erro = ERRO_ALOCACAO_MEMORIA;
        return NULL;
    }

    // Adiciona na posição correta da lista
    if ((*cidade).primeiraAntena == NULL)
    {
        (*cidade).primeiraAntena = nova;
        ultimaAntena = nova;
    }
    else
    {
        (*ultimaAntena).prox = nova;
        ultimaAntena = nova;
    }

    // Incrementa o número de antenas
    (*cidade).numAntenas++;

    return nova;
}

/**
 * @brief Adiciona uma aresta entre duas antenas.
 * 
 * @param inicio Apontador para a antena de origem.
 * @param destino Apontador para a antena de destino.
 * @param verificarRepetidas Booleano que indica se deve verificar arestas repetidas.
 * @return int 0 se for bem-sucedido, ou código de erro:
 * @return int ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS se os vértices forem inválidos,
 * @return int ERRO_ANTENA_INICIO_INVALIDA se o vértice de início for inválido,
 * @return int ERRO_ANTENA_DESTINO_INVALIDA se o vértice de destino for inválido,
 * @return int ERRO_OBJETO_JA_EXISTE se a aresta já existir (quando verificarRepetidas for verdadeiro),
 * @return int -404 se não for possível alocar memória.
 */
int adicionarAresta(Vertice *inicio, Vertice *destino, bool verificarRepetidas)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == NULL)
    {
        if (destino == NULL) return ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS;
        else return ERRO_ANTENA_INICIO_INVALIDA;
    }
    else if (destino == NULL) return ERRO_ANTENA_DESTINO_INVALIDA;

    // Verifica se já existe a aresta (se verificarRepetidas for true)
    if (verificarRepetidas)
    {
        for (Aresta *arestaAtual = (*inicio).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
        {
            if ((*arestaAtual).destino == destino)
            {
                return ERRO_OBJETO_JA_EXISTE;
            }
        }
    }

    // Cria a nova aresta
    Aresta *nova = criarAresta(destino);

    // Verifica se foi possível alocar a memória
    if (nova == NULL) return ERRO_ALOCACAO_MEMORIA;

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
 * @return int 0 se for bem-sucedido.
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o grafo for inválido.
 * @return int ERRO_OBJETO_NAO_EXISTE se a antena não for encontrada.
 */
int removerAntena2(Grafo *cidade, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    Vertice *antenaAnterior = NULL;
    Vertice *antenaAtual = (*cidade).primeiraAntena;

    while (antenaAtual != NULL && y >= (*antenaAtual).y)
    {
        // Verifica se a antena atual é a que queremos remover
        if ((*antenaAtual).x == x && (*antenaAtual).y == y)
        {
            *frequencia = (*antenaAtual).frequencia; /* Guarda a frequência da antena removida */

            // Verifica a posição da antena (início ou meio/fim)
            if (antenaAnterior == NULL)
            {
                (*cidade).primeiraAntena = (*antenaAtual).prox;
            }
            else
            {
                (*antenaAnterior).prox = (*antenaAtual).prox;
            }

            (*cidade).numAntenas--;

            // Liberta a memória
            free(antenaAtual);

            return 0;
        }

        antenaAnterior = antenaAtual;
        antenaAtual = (*antenaAtual).prox;
    }

    return ERRO_OBJETO_NAO_EXISTE;
}

/**
 * @brief Remove uma aresta entre duas antenas.
 * 
 * @param inicio Apontador para a antena de origem.
 * @param destino Apontador para a antena de destino.
 * @return int 0 se for bem-sucedido, ou código de erro:
 * @return int ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS se os vértices forem iguais,
 * @return int ERRO_ANTENA_INICIO_INVALIDA se o vértice de início for inválido,
 * @return int ERRO_ANTENA_DESTINO_INVALIDA se o vértice de destino for inválido,
 * @return int ERRO_OBJETO_NAO_EXISTE se a aresta não existir.
 */
int removerAresta(Vertice *inicio, Vertice *destino)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == NULL)
    {
        if (destino == NULL) return ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS;
        else return ERRO_ANTENA_INICIO_INVALIDA;
    }
    else if (destino == NULL) return ERRO_ANTENA_DESTINO_INVALIDA;

    Aresta *anterior = NULL;
    Aresta *atual = (*inicio).primeiraAresta;

    if ((*atual).destino == destino)
    {
        (*inicio).primeiraAresta = (*atual).prox;
        return 0;
    }
    else
    {
        anterior = atual;
        atual = (*atual).prox;

        while (atual != NULL)
        {
            // Verifica se a aresta atual é a que queremos remover
            if ((*atual).destino == destino)
            {
                // Verifica a posição da aresta (início ou meio/fim)
                (*anterior).prox = (*atual).prox;

                // Liberta a memória
                free(atual);

                return 0;
            }

            anterior = atual;
            atual = (*atual).prox;
        }
    }

    return ERRO_OBJETO_NAO_EXISTE;
}

/**
 * @brief Procura uma antena no grafo com base nas coordenadas (x,y).
 * 
 * @param cidade Ponteiro para a estrutura do grafo onde será realizada a pesquisa.
 * @param x Coordenada horizontal da antena.
 * @param y Coordenada vertical da antena (deve ser não-negativa).
 * @param erro Ponteiro para variável que armazenará o código de erro, caso ocorra.
 * 
 * @note A função assume que as antenas estão ordenadas por coordenada y decrescente,
 *       permitindo otimizar a busca. Caso y seja menor que a coordenada y da antena atual,
 *       a pesquisa é interrompida antecipadamente.
 * 
 * @note Códigos de erro possíveis:
 *       - ERRO_CIDADE_PONTEIRO_INVALIDO se cidade for NULL
 *       - ERRO_OBJETO_NAO_EXISTE se a antena não for encontrada
 * 
 * @return ponteiro para a antena encontrada.
 * @return NULL se não for encontrada ou ocorrer erro, o código de erro é armazenado no parâmetro "*erro"
 */
Vertice *procurarAntena(Grafo *cidade, int x, int y, int *erro)
{
    // Verifica se o apontador é válido
    if (cidade == NULL)
    {
        *erro = ERRO_CIDADE_PONTEIRO_INVALIDO;
        return NULL;
    }

    for (Vertice *antenaAtual = (*cidade).primeiraAntena; antenaAtual != NULL && y >= (*antenaAtual).y; antenaAtual = (*antenaAtual).prox)
    {
        if (x == (*antenaAtual).x && y == (*antenaAtual).y)
        {
            return antenaAtual;
        }
    }

    *erro = ERRO_OBJETO_NAO_EXISTE;
    return NULL;
}

/**
 * @brief Procura uma aresta entre duas antenas (vértices) no grafo.
 * 
 * @param inicio Ponteiro para o vértice de origem da aresta.
 * @param destino Ponteiro para o vértice de destino da aresta.
 * @param erro Ponteiro para armazenar o código de erro em caso de falha.
 * 
 * @note A função:
 *       - Verifica a validade dos vértices de início e destino
 *       - Atribui códigos de erro específicos para cada caso de vértice inválido
 *       - Percorre a lista de arestas do vértice de início
 *       - Compara cada destino de aresta com o vértice destino fornecido
 * 
 * @note Códigos de erro possíveis:
 *       - ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS se ambos vértices forem NULL
 *       - ERRO_ANTENA_INICIO_INVALIDA se apenas o vértice de início for NULL
 *       - ERRO_ANTENA_DESTINO_INVALIDA se apenas o vértice de destino for NULL
 *       - ERRO_OBJETO_NAO_EXISTE se a aresta não for encontrada
 * 
 * @return o ponteiro para a aresta encontrada.
 * @return NULL se a aresta não existir ou em caso de erro, armazenando o código de erro no parâmetro "*erro".
 */
Aresta *procurarAresta(Vertice *inicio, Vertice *destino, int *erro)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == NULL)
    {
        if (destino == NULL) *erro = ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS;
        else *erro = ERRO_ANTENA_INICIO_INVALIDA;
        return NULL;
    }
    else if (destino == NULL)
    {
        *erro = ERRO_ANTENA_DESTINO_INVALIDA;
        return NULL;
    }

    // Percorre as arestas e verifica se existe a aresta especificada
    for (Aresta *arestaAtual = (*inicio).primeiraAresta; arestaAtual != NULL; arestaAtual = (*arestaAtual).prox)
    {
        if ((*arestaAtual).destino == destino)
        {
            return arestaAtual;
        }
    }

    *erro = ERRO_OBJETO_NAO_EXISTE;
    return NULL; /* Retorna NULL caso não tenha encontrado a aresta */
}

/**
 * @brief Realiza uma procura em profundidade (DFS) a partir de uma antena.
 * 
 * @param inicio Apontador para a antena de partida.
 * 
 * @return int 0 se for bem-sucedido, ou -3 se a antena de início for inválida.
 */
int procurarProfundidade(Vertice *inicio)
{
    // Verifica se a antena início é válida
    if (inicio == NULL) return ERRO_ANTENA_INICIO_INVALIDA;

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
 * 
 * @return int 0 se for bem-sucedido,
 * @return int ERRO_ANTENA_INICIO_INVALIDA se a antena de início for inválida,
 * @return int ERRO_OVERFLOW_LISTA se a lista auxiliar exceder o tamanho máximo permitido (overflow).
 */
int procurarLargura(Vertice *inicio, int numeroAntenas)
{
    // Verifica se a antena início é válida
    if (inicio == NULL) return ERRO_ANTENA_INICIO_INVALIDA;

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
                    return ERRO_OVERFLOW_LISTA;
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
 * 
 * @return int 1 se pelo menos um caminho for encontrado e impresso,  
 * @return int 0 se nenhum caminho for encontrado,  
 * @return int ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS se as antenas de início e fim forem iguais (inválidas),  
 * @return int ERRO_ANTENA_INICIO_INVALIDA se a antena de início for inválida (NULL),  
 * @return int ERRO_ANTENA_DESTINO_INVALIDA se a antena de destino for inválida (NULL).
 */
int procurarCaminhos(Vertice *inicio, Vertice *destino, int numeroAntenas)
{
    // Verifica se as antenas de início e destino são válidas
    if (inicio == NULL)
    {
        if (destino == NULL) return ERRO_ANTENAS_INICIO_DESTINO_INVALIDAS;
        else return ERRO_ANTENA_INICIO_INVALIDA;
    }
    else if (destino == NULL) return ERRO_ANTENA_DESTINO_INVALIDA;

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
 * 
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
 * 
 * @return int 1 se forem encontradas e listadas interseções.
 * @return int 0 se não forem encontradas interseções.
 * @return int ERRO_CIDADE_PONTEIRO_INVALIDO se o grafo da cidade for inválido.
 * @return int ERRO_FREQUENCIAS_IGUAIS se as duas frequências forem iguais.
 */
int listarIntersecoes(Grafo *cidade, char frequencia1, char frequencia2)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

    // Verifica se as frequências são diferentes
    if (frequencia1 == frequencia2) return ERRO_FREQUENCIAS_IGUAIS;

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
 * 
 * @return int 0 se for bem-sucedido, ou -1 se o grafo da cidade for inválido.
 */
int listarAntenas(Grafo *cidade)
{
    // Verifica se o apontador é válido
    if (cidade == NULL) return ERRO_CIDADE_PONTEIRO_INVALIDO;

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
 * @return int ERRO_ANTENA_INICIO_INVALIDA se o apontador para a antena for inválido (NULL).
 */
int listarArestasAntena(Vertice *antena)
{
    // Verifica se o apontador antena é valido
    if (antena == NULL) return ERRO_ANTENA_INICIO_INVALIDA;

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