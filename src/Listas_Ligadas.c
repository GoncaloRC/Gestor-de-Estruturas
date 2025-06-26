/**
 * @file Antenas.c
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Implementação das operações de gestão de antenas e efeitos nefastos
 * @version 1.0
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../include/Listas_Ligadas.h"

/**
 * @brief Cria e inicializa uma nova rede na memória.
 * 
 * Esta função aloca memória para uma estrutura do tipo Rede e inicializa todos os seus campos.
 * 
 * @return Apontador para a rede criada.
 * @return NULL em caso de falha na alocação de memória.
 */
Rede *LL_criarRede()
{
    // Aloca o espaço na memória para a rede (e inicializa as variáveis)
    return calloc(1, sizeof(Rede));
}

/**
 * @brief Cria e inicializa uma nova antena na memória.
 * 
 * Esta função aloca memória para uma estrutura do tipo Antena e inicializa os seus campos
 * com os valores fornecidos.
 * 
 * @param frequencia A frequência de operação da antena.
 * @param x A coordenada x da antena.
 * @param y A coordenada y da antena.
 * 
 * @return Apontador para a antena criada.
 * @return NULL em caso de falha na alocação de memória.
 */
Antena *LL_criarAntena(char frequencia, int x, int y)
{
    // Aloca o espaço na memória para a antena (e inicializa as variáveis)
    Antena *novaAntena = malloc(sizeof(Antena));

    // Verifica se foi possível alocar a memória
    if (novaAntena == NULL) return NULL;

    // Popula as variáveis da estrutura
    (*novaAntena).frequencia = frequencia;
    (*novaAntena).x = x;
    (*novaAntena).y = y;
    (*novaAntena).prox = NULL;    

    return novaAntena;
}

/**
 * @brief Cria e inicializa um novo nefasto na memória.
 * 
 * Esta função aloca memória para uma estrutura do tipo Nefasto e inicializa os seus campos
 * com os valores fornecidos.
 * 
 * @param x A coordenada x do nefasto.
 * @param y A coordenada y do nefasto.
 * 
 * @return Apontador para o nefasto criado.
 * @return NULL em caso de falha na alocação de memória.
 */
Nefasto *LL_criarNefasto(int x, int y)
{
    // Aloca o espaço na memória para o nefasto (e inicializa as variáveis)
    Nefasto *novoNefasto = malloc(sizeof(Nefasto));

    // Verifica se foi possível alocar a memória
    if (novoNefasto == NULL) return NULL;

    // Popula as variáveis da estrutura
    (*novoNefasto).x = x;
    (*novoNefasto).y = y;
    (*novoNefasto).prox = NULL;

    return novoNefasto;
}

/**
 * @brief Liberta a memória alocada para uma rede e todos os seus elementos.
 * 
 * Esta função liberta a memória alocada para as antenas, nefastos e finalmente para a própria rede.
 * 
 * @param rede Apontador para a rede a ser libertada.
 * 
 * @return NULL para indicar que a rede foi libertada com sucesso.
 */
Rede *LL_libertarRede(Rede *rede)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return NULL;

    // Liberta as antenas e os nefastos
    LL_libertarAntenas(rede);
    LL_libertarNefastos(rede);

    // Liberta a cidade
    free(rede);

    return NULL;
}

/**
 * @brief Liberta a memória ocupada pelas antenas na lista.
 * 
 * Esta função percorre a lista de antenas, libertando a memória de cada antena, e redefine o ponteiro da primeira antena para NULL.
 * 
 * @param rede Ponteiro para a rede com a lista de antenas.
 * 
 * @return 0 Caso a operação seja bem sucedida.
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" Caso o apontador seja inválido.
 */
int LL_libertarAntenas(Rede *rede)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Liberta a lista das antenas
    for (Antena *antenaAtual = (*rede).primeiraAntena; antenaAtual != NULL; )
    {
        Antena *temp = antenaAtual;
        antenaAtual = (*antenaAtual).prox;
        free(temp);
    }

    // Dá reset das variáveis da rede
    (*rede).primeiraAntena = NULL;
    (*rede).numAntenas = 0;

    return 0;
}

/**
 * @brief Liberta a memória ocupada pelos efeitos nefastos na lista.
 * 
 * Esta função percorre a lista de efeitos nefastos, libertando a memória de cada elemento, e redefine o ponteiro do primeiro efeito nefasto para NULL.
 * 
 * @param rede Ponteiro para a rede com a lista de efeitos nefastos.
 * 
 * @return 0 Caso a operação seja bem sucedida.
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" Caso o apontador seja inválido.
 */
int LL_libertarNefastos(Rede *rede)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Liberta a lista dos efeitos nefastos
    for (Nefasto *nefastoAtual = (*rede).primeiroNefasto; nefastoAtual != NULL; )
    {
        Nefasto *temp = nefastoAtual;
        nefastoAtual = (*nefastoAtual).prox;
        free(temp);
    }

    // Dá reset das variáveis da rede
    (*rede).primeiroNefasto = NULL;
    (*rede).numNefastos = 0;

    return 0;
}

/**
 * @brief Adiciona uma nova antena à rede de forma ordenada
 * 
 * Insere uma nova antena na lista ligada de antenas, mantendo a ordenação por coordenadas (y crescente, x crescente para y igual).
 * Verifica se já existe uma antena nas mesmas coordenadas antes de inserir.
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param frequencia Ponteiro para a frequência da antena (recebe a frequência existente se a antena já existir)
 * @param x Coordenada x da nova antena
 * @param y Coordenada y da nova antena
 * 
 * @return Código de erro:
 * @return 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se o ponteiro da rede for inválido
 * @return "LL_ERRO_ANTENA_JA_EXISTE" se já existir uma antena nas mesmas coordenadas
 * @return "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória
 */
int LL_adicionarAntenaOrdenada(Rede *rede, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Cria o 1.º apontador para navegar a lista ligada
    Antena *antenaAtual = (*rede).primeiraAntena;

    // Insere no início da lista
    if (antenaAtual == NULL || (y < (*antenaAtual).y) || (x <= (*antenaAtual).x && y == (*antenaAtual).y))
    {
        // Verifica se a antena é repetida
        if (antenaAtual != NULL && x == (*antenaAtual).x && y == (*antenaAtual).y)
        {
            *frequencia = (*antenaAtual).frequencia; /* Guarda a frequência da antena ocupada */
            return LL_ERRO_ANTENA_JA_EXISTE;
        }

        // Cria a nova antena e popula as variáveis da estrutura
        Antena *nova = LL_criarAntena(*frequencia, x, y);

        // Verifica se foi possível alocar a memória
        if (nova == NULL) return LL_ERRO_ALOCACAO_MEMORIA;

        // Insere a antena na posição correspondente da lista
        (*nova).prox = (*rede).primeiraAntena;
        (*rede).primeiraAntena = nova;
    }
    // Insere no meio e fim da lista
    else
    {
        // Cria o 2.º apontador para navegar a lista ligada
        Antena *antenaAnterior = NULL;

        while (antenaAtual != NULL && ((y > (*antenaAtual).y) || (x > (*antenaAtual).x && y == (*antenaAtual).y)))
        {
            antenaAnterior = antenaAtual;
            antenaAtual = (*antenaAtual).prox;
        }

        if (antenaAtual != NULL && x == (*antenaAtual).x && y == (*antenaAtual).y)
        {
            *frequencia = (*antenaAtual).frequencia; /* Guarda a frequência da antena ocupada */
            return LL_ERRO_ANTENA_JA_EXISTE;
        }

        // Aloca o espaço na memória para a nova antena
        Antena *nova = LL_criarAntena(*frequencia, x, y);

        // Verifica se foi possível alocar a memória
        if (nova == NULL) return LL_ERRO_ALOCACAO_MEMORIA;

        // Insere a antena na posição correspondente da lista
        (*antenaAnterior).prox = nova;
        (*nova).prox = antenaAtual;
    }

    (*rede).numAntenas++;

    return 0;
}

/**
 * @brief Adiciona uma nova antena no final da lista de antenas
 * 
 * Insere uma nova antena no final da lista ligada de antenas, sem verificar ordenação ou duplicados.
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param ultimaAntena Ponteiro para a última antena da lista (atualizado com a nova antena)
 * @param frequencia Frequência da nova antena
 * @param x Coordenada x da nova antena
 * @param y Coordenada y da nova antena
 * @param erro Ponteiro para armazenar código de erro:
 * 
 * @return 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se o ponteiro da rede for inválido
 * @return "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória
 * @return Ponteiro para a nova antena criada.
 * @return NULL em caso de erro
 */
Antena *LL_adicionarAntenaFim(Rede *rede, Antena *ultimaAntena, char frequencia, int x, int y, int *erro)
{
    // Verifica se o apontador é válido
    if (rede == NULL)
    {
        *erro = LL_ERRO_REDE_PONTEIRO_INVALIDO;
        return NULL;
    }

    // Cria o espaço na memória para a nova antena
    Antena *nova = LL_criarAntena(frequencia, x, y);

    // Verifica se foi possível alocar a memória
    if (nova == NULL)
    {
        *erro = LL_ERRO_ALOCACAO_MEMORIA;
        return NULL;
    }

    // Adiciona na posição correta da lista
    if ((*rede).primeiraAntena == NULL)
    {
        (*rede).primeiraAntena = nova;
        ultimaAntena = nova;
    }
    else
    {
        (*ultimaAntena).prox = nova;
        ultimaAntena = nova;
    }

    // Incrementa o número de antenas
    (*rede).numAntenas++;

    return nova;
}

/**
 * @brief Adiciona um nefasto à rede de forma ordenada
 * 
 * Insere um novo nefasto na lista ligada de nefastos, mantendo a ordenação
 * por coordenadas (y crescente, x crescente para y igual)
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param x Coordenada x do nefasto a inserir
 * @param y Coordenada y do nefasto a inserir
 * 
 * @return Código de erro:
 * @return 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se rede for NULL
 * @return "LL_ERRO_NEFASTO_JA_EXISTE" se já existir nefasto com essas coordenadas
 * @return "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória
 */
int LL_adicionarNefastoOrdenado(Rede *rede, int x, int y)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Cria o 1.º apontador para navegar a lista ligada
    Nefasto *nefastoAtual = (*rede).primeiroNefasto;

    // Insere no início da lista
    if (nefastoAtual == NULL || (y < (*nefastoAtual).y) || (x <= (*nefastoAtual).x && y == (*nefastoAtual).y))
    {
        // Verifica se o nefasto é repetido
        if (nefastoAtual != NULL && x == (*nefastoAtual).x && y == (*nefastoAtual).y) return LL_ERRO_NEFASTO_JA_EXISTE;

        // Cria o novo nefasto e popula as variáveis da estrutura
        Nefasto *novo = LL_criarNefasto(x, y);

        // Verifica se foi possível alocar a memória
        if (novo == NULL) return LL_ERRO_ALOCACAO_MEMORIA;

        // Insere o nefasto na posição correspondente da lista
        (*novo).prox = (*rede).primeiroNefasto;
        (*rede).primeiroNefasto = novo;
    }
    // Insere no meio e fim da lista
    else
    {
        // Cria o 2.º apontador para navegar a lista ligada
        Nefasto *nefastoAnterior = NULL;

        while (nefastoAtual != NULL && ((y > (*nefastoAtual).y) || (x > (*nefastoAtual).x && y == (*nefastoAtual).y)))
        {
            nefastoAnterior = nefastoAtual;
            nefastoAtual = (*nefastoAtual).prox;
        }

        if (nefastoAtual != NULL && x == (*nefastoAtual).x && y == (*nefastoAtual).y) return LL_ERRO_NEFASTO_JA_EXISTE;

        // Aloca o espaço na memória para o novo nefasto
        Nefasto *novo = LL_criarNefasto(x, y);

        // Verifica se foi possível alocar a memória
        if (novo == NULL) return LL_ERRO_ALOCACAO_MEMORIA;

        // Insere a antena na posição correspondente da lista
        (*nefastoAnterior).prox = novo;
        (*novo).prox = nefastoAtual;
    }

    // Incrementa o número de nefastos
    (*rede).numNefastos++;

    return 0;
}

/**
 * @brief Adiciona um nefasto no fim da lista ligada
 * 
 * Insere um novo nefasto no final da lista ligada de nefastos,
 * atualizando o ponteiro para o último elemento
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param ultimoNefasto Ponteiro para o último nefasto da lista
 * @param x Coordenada x do nefasto a inserir
 * @param y Coordenada y do nefasto a inserir
 * @param erro Ponteiro para variável que receberá o código de erro
 * 
 * @return Ponteiro para o novo nefasto inserido.
 * @return NULL em caso de erro.
 */
Nefasto *LL_adicionarNefastoFim(Rede *rede, Nefasto *ultimoNefasto, int x, int y, int *erro)
{
    // Verifica se o apontador é válido
    if (rede == NULL)
    {
        *erro = LL_ERRO_REDE_PONTEIRO_INVALIDO;
        return NULL;
    }

    // Cria o espaço na memória para o novo nefasto
    Nefasto *novo = LL_criarNefasto(x, y);

    // Verifica se foi possível alocar a memória
    if (novo == NULL)
    {
        *erro = LL_ERRO_ALOCACAO_MEMORIA;
        return NULL;
    }

    // Adiciona na posição correta da lista
    if ((*rede).primeiroNefasto == NULL)
    {
        (*rede).primeiroNefasto = novo;
        ultimoNefasto = novo;
    }
    else
    {
        (*ultimoNefasto).prox = novo;
        ultimoNefasto = novo;
    }

    // Incrementa o número de nefastos
    (*rede).numNefastos++;

    return novo;
}

/**
 * @brief Remove uma antena da lista de antenas da rede.
 * 
 * Esta função procura e remove uma antena com as coordenadas especificadas,
 * libertando a memória alocada para a mesma.
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param frequencia Ponteiro para guardar a frequência da antena removida
 * @param x Coordenada x da antena a remover
 * @param y Coordenada y da antena a remover
 * 
 * @return Código de erro:
 * @return 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se rede for NULL
 * @return "LL_ERRO_ANTENA_NAO_EXISTE" se a antena não for encontrada
 */
int LL_removerAntena(Rede *rede, char *frequencia, int x, int y)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    Antena *antenaAnterior = NULL;
    Antena *antenaAtual = (*rede).primeiraAntena;

    while (antenaAtual != NULL && y >= (*antenaAtual).y)
    {
        // Verifica se a antena atual é a que queremos remover
        if ((*antenaAtual).x == x && (*antenaAtual).y == y)
        {
            *frequencia = (*antenaAtual).frequencia; /* Guarda a frequência da antena removida */

            // Verifica a posição da antena (início ou meio/fim)
            if (antenaAnterior == NULL)
            {
                (*rede).primeiraAntena = (*antenaAtual).prox;
            }
            else
            {
                (*antenaAnterior).prox = (*antenaAtual).prox;
            }

            (*rede).numAntenas--;

            // Liberta a memória
            free(antenaAtual);

            return 0;
        }

        antenaAnterior = antenaAtual;
        antenaAtual = (*antenaAtual).prox;
    }

    return LL_ERRO_ANTENA_NAO_EXISTE;
}

/**
 * @brief Calcula e adiciona os efeitos nefastos entre antenas.
 * 
 * Identifica pares de antenas com a mesma frequência e calcula as posições
 * dos efeitos nefastos resultantes, adicionando-os à lista ordenada.
 * 
 * @param rede Ponteiro para a estrutura da rede
 * @param coordenadasNegativas Indica se devem ser consideradas coordenadas negativas
 * 
 * @return Código de erro:
 * @return 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se rede for NULL
 * @return "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória
 */
int LL_calcularNefastos(Rede *rede, bool coordenadasNegativas)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Percorre todas as combinações de antenas
    for (Antena *antenaAtual = (*rede).primeiraAntena; antenaAtual != NULL; antenaAtual = (*antenaAtual).prox)
    {
        for (Antena *comparar = (*antenaAtual).prox; comparar != NULL; comparar = (*comparar).prox)
        {
            // Verifica se são antenas diferentes com mesma frequência
            if ((*antenaAtual).frequencia == (*comparar).frequencia)
            {
                // Calcula as distâncias entre as antenas
                int distanciaX = (*comparar).x - (*antenaAtual).x;
                int distanciaY = (*comparar).y - (*antenaAtual).y;

                // Calcula efeitos nefastos estendendo a linha em ambas as direções
                int nefasto1X = (*antenaAtual).x - distanciaX;
                int nefasto1Y = (*antenaAtual).y - distanciaY;
                int nefasto2X = (*comparar).x + distanciaX;
                int nefasto2Y = (*comparar).y + distanciaY;

                // Adiciona os efeitos nefastos à lista (evitando os duplicados) e bloqueia coordenadas negativas se "coordenadasNegativas" for "false"
                if (coordenadasNegativas || (nefasto1X >= 0 && nefasto1Y >= 0))
                {
                    if (LL_adicionarNefastoOrdenado(rede, nefasto1X, nefasto1Y) == LL_ERRO_ALOCACAO_MEMORIA) return LL_ERRO_ALOCACAO_MEMORIA;
                }
                if (coordenadasNegativas || (nefasto2X >= 0 && nefasto2Y >= 0))
                {
                    if (LL_adicionarNefastoOrdenado(rede, nefasto2X, nefasto2Y) == LL_ERRO_ALOCACAO_MEMORIA) return LL_ERRO_ALOCACAO_MEMORIA;
                }
            }
        }
    }

    return 0;
}

/**
 * @brief Carrega as antenas a partir de um ficheiro.
 * 
 * Esta função lê um ficheiro de texto contendo informações sobre as antenas e adiciona-as à rede,
 * uma por uma, com base nas coordenadas (x, y) e na frequência (representada por um caractere).
 * Cada letra (maiúscula ou minúscula) no ficheiro é interpretada como uma antena.
 * 
 * @param rede Ponteiro para a estrutura da rede onde serão adicionadas as antenas
 * @param localizacaoFicheiro Caminho para o ficheiro que contém as informações das antenas
 * 
 * @return Retorna 0 em caso de sucesso
 * @return Retorna "LL_ERRO_REDE_PONTEIRO_INVALIDO" se o ponteiro para a rede for inválido
 * @return Retorna "LL_ERRO_ABRIR_FICHEIRO" se ocorrer um erro ao abrir o ficheiro
 * @return Retorna "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória para uma antena
 */
int LL_carregarAntenas(Rede *rede, const char *localizacaoFicheiro)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return LL_ERRO_ABRIR_FICHEIRO;

    // Variaveis de otimização e retorno de erro
    Antena *ultimaAntena = NULL;
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
            // Verifica se o caracter é uma letra (com early exit)
            if (linha[x] >= 'A' && (linha[x] <= 'Z' || (linha[x] >= 'a' && linha[x] <= 'z')))
            {
                // Adiciona a nova antena
                ultimaAntena = LL_adicionarAntenaFim(rede, ultimaAntena, linha[x], x, y, &erro);

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

    return 0; /* Retorna 0 em caso de sucesso */
}

/**
 * @brief Carrega os efeitos nefastos a partir de um ficheiro.
 * 
 * Esta função lê um ficheiro de texto contendo informações sobre os efeitos nefastos e adiciona-os
 * à rede, um por um, com base nas coordenadas (x, y). Cada símbolo '#' no ficheiro é interpretado
 * como um efeito nefasto.
 * 
 * @param rede Ponteiro para a estrutura da rede onde serão adicionados os efeitos nefastos
 * @param localizacaoFicheiro Caminho para o ficheiro que contém as informações dos efeitos nefastos
 * 
 * @return Retorna 0 em caso de sucesso
 * @return "LL_ERRO_REDE_PONTEIRO_INVALIDO" se o ponteiro para a rede for inválido
 * @return "LL_ERRO_ABRIR_FICHEIRO" se ocorrer um erro ao abrir o ficheiro
 * @return "LL_ERRO_ALOCACAO_MEMORIA" se falhar a alocação de memória para um efeito nefasto
 */
int LL_carregarNefastos(Rede *rede, const char *localizacaoFicheiro)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return LL_ERRO_ABRIR_FICHEIRO;

    // Variaveis de otimização e retorno de erro
    Nefasto *ultimoNefasto = NULL;
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
            if (linha[x] == '#')
            {
                // Cria um novo nefasto
                ultimoNefasto = LL_adicionarNefastoFim(rede, ultimoNefasto, x, y, &erro);

                // Verifica se foi possível alocar a memória
                if (ultimoNefasto == NULL)
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

    return 0; /* Retorna 0 em caso de sucesso */
}

/**
 * @brief Apresenta uma tabela com informações das antenas.
 * 
 * Esta função apresenta uma tabela com as antenas e as suas localizações (x, y).
 * As tabelas são formatadas e apresentadas no terminal.
 * 
 * @param rede Ponteiro para a estrutura que contem a primeira antena da lista e o número de antenas.
 * 
 * @return bool "true" caso tenha imprimido antenas.
 * @return bool "false" caso tenha imprimido uma tabela vazia.
 */
bool LL_apresentarAntenas(Rede *rede)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Imprime a tabela das antenas
    printf("----------------------------------------");
    printf("\n| 📶         | X          | Y          |");
    printf("\n----------------------------------------");

    if ((*rede).numAntenas > 0)
    {
        Antena *antenaAtual = (*rede).primeiraAntena;

        while (antenaAtual != NULL)
        {
            printf("\n| %-11c| %-11d| %-11d|",
                (*antenaAtual).frequencia,
                (*antenaAtual).x,
                (*antenaAtual).y);
            antenaAtual = (*antenaAtual).prox;
        }
        printf("\n----------------------------------------");

        return true;
    }
    else
    {
        printf("\n|            |            |            |");
        printf("\n----------------------------------------");

        return false;
    }
}

/**
 * @brief Apresenta uma tabela com informações dos efeitos nefastos.
 * 
 * Esta função apresenta uma tabela com os efeitos nefastos e as suas localizações (x, y).
 * As tabelas são formatadas e apresentadas no terminal.
 * 
 * @param rede Ponteiro para a estrutura que contem o primeiro nefasto da lista e o número de nefastos.
 * 
 * @return bool "true" caso tenha imprimido antenas.
 * @return bool "false" caso tenha imprimido uma tabela vazia.
 */
bool LL_apresentarNefastos(Rede *rede)
{
    // Verifica se o apontador é válido
    if (rede == NULL) return LL_ERRO_REDE_PONTEIRO_INVALIDO;

    // Imprime a tabela das localizações nefastas
    printf("---------------------------");
    printf("\n| X          | Y          |");
    printf("\n---------------------------");

    if ((*rede).numNefastos > 0)
    {
        Nefasto *nefastoAtual = (*rede).primeiroNefasto;

        while (nefastoAtual != NULL)
        {
            printf("\n| %-11d| %-11d|",
                (*nefastoAtual).x,
                (*nefastoAtual).y);
            nefastoAtual = (*nefastoAtual).prox;
        }
        printf("\n---------------------------");

        return true;
    }
    else
    {
        printf("\n|            |            |");
        printf("\n---------------------------");

        return false;
    }    
}