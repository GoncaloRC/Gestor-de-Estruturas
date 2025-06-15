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
 * @brief Liberta a memória ocupada pelas antenas na lista.
 * 
 * Esta função percorre a lista de antenas, libertando a memória de cada antena, e redefine o ponteiro da primeira antena para NULL.
 * 
 * @param primeiraAntena Ponteiro para o início da lista de antenas.
 * @param numAntenas Ponteiro para o número total de antenas.
 * @return 0 Caso a operação seja bem sucedida.
 * @return -1 Caso o apontador seja inválido.
 */
int libertarAntenas(Antena **primeiraAntena, int *numAntenas)
{
    // Verifica se o apontador é válido
    if (*primeiraAntena == NULL) return -1; /* Return -1 caso não exista a primeira antena */

    // Liberta a lista das antenas
    *numAntenas = 0;

    Antena *antenaAtual = *primeiraAntena;

    while (antenaAtual != NULL)
    {
        Antena *temp = antenaAtual;
        antenaAtual = (*antenaAtual).prox;
        free(temp);
    }

    *primeiraAntena = NULL;

    return 0;
}

/**
 * @brief Liberta a memória ocupada pelos efeitos nefastos na lista.
 * 
 * Esta função percorre a lista de efeitos nefastos, libertando a memória de cada elemento, e redefine o ponteiro do primeiro efeito nefasto para NULL.
 * 
 * @param primeiroNefasto Ponteiro para o início da lista de efeitos nefastos.
 * @param numNefastos Ponteiro para o número total de efeitos nefastos.
 * @return 0 Caso a operação seja bem sucedida.
 * @return -1 Caso o apontador seja inválido.
 */
int libertarNefastos(Nefasto **primeiroNefasto, int *numNefastos)
{
    // Verifica se o apontador é válido
    if (*primeiroNefasto == NULL) return -1; /* Return -1 caso não exista o primeiro nefasto */

    // Liberta a lista dos efeitos nefastos
    *numNefastos = 0;

    Nefasto *nefastoAtual = *primeiroNefasto;

    while (nefastoAtual != NULL)
    {
        Nefasto *temp = nefastoAtual;
        nefastoAtual = (*nefastoAtual).prox;
        free(temp);
    }

    *primeiroNefasto = NULL;

    return 0;
}

/**
 * @brief Adiciona uma nova antena à lista.
 * 
 * Esta função adiciona uma nova antena à lista de antenas, verificando previamente se as coordenadas são válidas e se a antena já existe na posição desejada. 
 * O posicionamento da antena pode ser no início, no meio ou no fim da lista.
 * 
 * @param primeiraAntena Ponteiro para o início da lista de antenas.
 * @param ultimaAntena Ponteiro para o fim da lista de antenas.
 * @param numAntenas Ponteiro para o número total de antenas.
 * @param frequencia Frequência da nova antena.
 * @param x Coordenada X da nova antena.
 * @param y Coordenada Y da nova antena.
 * @param inserirNoFim Flag que indica se a antena deve ser inserida no fim da lista.
 * @return 0 Caso a operação seja bem sucedida.
 * @return -1 Caso as coordenadas de X ou Y sejam inválidas.
 * @return -2 Caso a antena já exista na posição solicitada.
 */
int adicionarAntena(Antena **primeiraAntena, Antena **ultimaAntena, int *numAntenas, char *frequencia, int x, int y, bool inserirNoFim)
{
    // Filtra coordenadas negativas
    if (x < 0 || y < 0) return -1; /* Retorna -1 em caso de serem coordenadas negativas */

    // Verifica a posição da nova antena na lista (e se esta é repetida)
    int posicaoNaLista = 0; /* Mais otimizado do que libertar a memória em caso de return -3, mas mais complexo! */

    Antena *anterior;
    Antena *posterior = *primeiraAntena;

    if (inserirNoFim == true)
    {
        posicaoNaLista = 1; /* Insere no fim */
    }
    else if (posterior == NULL)
    {
        posicaoNaLista = 2; /* Insere no início e fim (não existem antenas na lista) */
    }
    else if ((y < (*(*primeiraAntena)).y) || (x <= (*(*primeiraAntena)).x && y == (*(*primeiraAntena)).y))
    {
        if (x == (*(*primeiraAntena)).x && y == (*(*primeiraAntena)).y)
        {
            *frequencia = (*(*primeiraAntena)).frequencia; /* Guarda a frequência da antena ocupada */
            return -2; /* Retorna -2 em caso de já existir essa antena */
        }
        else
        {
            posicaoNaLista = 3; /* Insere no início */
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
            return -2; /* Retorna -2 em caso de já existir essa antena */
        }
    }

    // Cria o espaço na memória para a nova antena e popula as variáveis
    Antena *nova = malloc(sizeof(Antena));
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
    else if (posicaoNaLista == 1)
    {
        if (*primeiraAntena == NULL)
        {
            *primeiraAntena = nova;
            *ultimaAntena = nova;
        }
        else
        {
            (*(*ultimaAntena)).prox = nova;
            *ultimaAntena = nova;
        }
    }
    if (posicaoNaLista == 2)
    {
        *primeiraAntena = nova;
    }
    if (posicaoNaLista == 3)
    {
        (*nova).prox = *primeiraAntena;
        *primeiraAntena = nova;
    }

    (*numAntenas)++;

    return 0;
}

/**
 * @brief Adiciona um novo efeito nefasto à lista.
 * 
 * Esta função adiciona um novo efeito nefasto à lista, verificando previamente se as coordenadas são válidas e se o efeito nefasto já existe na posição desejada. 
 * O posicionamento do efeito pode ser no início, no meio ou no fim da lista.
 * 
 * @param primeiroNefasto Ponteiro para o início da lista de efeitos nefastos.
 * @param ultimoNefasto Ponteiro para o fim da lista de efeitos nefastos.
 * @param numNefastos Ponteiro para o número total de efeitos nefastos.
 * @param x Coordenada X do novo efeito nefasto.
 * @param y Coordenada Y do novo efeito nefasto.
 * @param inserirNoFim Flag que indica se o efeito nefasto deve ser inserido no fim da lista.
 * @return 0 Caso a operação seja bem sucedida.
 * @return -1 Caso as coordenadas de X ou Y sejam inválidas.
 * @return -2 Caso o efeito nefasto já exista na posição solicitada.
 */
int adicionarNefasto(Nefasto **primeiroNefasto, Nefasto **ultimoNefasto, int *numNefastos, int x, int y, bool inserirNoFim)
{
    // Filtra coordenadas negativas
    if (x < 0 || y < 0) return -1; /* Retorna -1 em caso de serem coordenadas negativas */
    
    // Verifica a posição do novo nefasto na lista (e se este é repetido)
    int posicaoNaLista = 0; /* Mais otimizado do que libertar a memória em caso de return -3, mas mais complexo! */

    Nefasto *anterior;
    Nefasto *posterior = *primeiroNefasto;

    if (inserirNoFim == true)
    {
        posicaoNaLista = 1; /* Insere no fim */
    }
    else if (posterior == NULL)
    {
        posicaoNaLista = 2; /* Insere no início e fim (não existem nefastos na lista) */
    }
    else if ((y < (*(*primeiroNefasto)).y) || (x <= (*(*primeiroNefasto)).x && y == (*(*primeiroNefasto)).y))
    {
        if (x == (*(*primeiroNefasto)).x && y == (*(*primeiroNefasto)).y)
        {
            return -2; /* Retorna -2 em caso de já existir esse nefasto */
        }
        else
        {
            posicaoNaLista = 3; /* Insere no início */
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
            return -2; /* Retorna -3 em caso de já existir esse nefasto */
        }
    }

    // Cria o espaço na memória para a nova nefasto e popula as variáveis
    Nefasto *novo = malloc(sizeof(Nefasto));
    (*novo).x = x;
    (*novo).y = y;
    (*novo).prox = NULL;

    // Insere a antena na posição correspondente
    if (posicaoNaLista == 0)
    {
        (*anterior).prox = novo;
        (*novo).prox = posterior;
    }
    else if (posicaoNaLista == 1)
    {
        if (*primeiroNefasto == NULL)
        {
            *primeiroNefasto = novo;
            *ultimoNefasto = novo;
        }
        else
        {
            (*(*ultimoNefasto)).prox = novo;
            *ultimoNefasto = novo;
        }
    }
    if (posicaoNaLista == 2)
    {
        *primeiroNefasto = novo;
    }
    if (posicaoNaLista == 3)
    {
        (*novo).prox = *primeiroNefasto;
        *primeiroNefasto = novo;
    }

    (*numNefastos)++;

    return 0;
}

/**
 * @brief Remove uma antena da lista de antenas.
 * 
 * Esta função percorre a lista de antenas para encontrar a antena que deve ser removida com base nas coordenadas.
 * A memória alocada para a antena removida é libertada.
 * 
 * @param primeiraAntena Ponteiro para o ponteiro da primeira antena da lista.
 * @param numAntenas Número atual de antenas na lista. Será decrementado.
 * @param x Coordenada X da antena a ser removida.
 * @param y Coordenada Y da antena a ser removida.
 * 
 * @return Retorna 0 em caso de sucesso.
 * @return Retorna -1 caso não existam antenas.
 * @return Retorna -2 caso a antena não seja encontrada.
 */
int removerAntena(Antena **primeiraAntena, int *numAntenas, char *frequencia, int x, int y)
{
    // Verifica se existem antenas na lista
    if (*numAntenas == 0) return -1;   // Retorna -1 em caso de não existirem antenas na lista

    Antena *atual = *primeiraAntena;
    Antena *anterior = NULL;

    while (atual != NULL)
    {
        // Verifica se a antena atual é a que queremos remover
        if ((*atual).x == x && (*atual).y == y)
        {
            *frequencia = (*atual).frequencia; /* Guarda a frequência da antena removida */

            // Verifica a posição da antena (início ou meio/fim)
            if (anterior == NULL)
            {
                *primeiraAntena = (*atual).prox;
            }
            else
            {
                (*anterior).prox = (*atual).prox;
            }

            (*numAntenas)--;

            // Liberta a memória
            free(atual);

            return 0;   // Retorna 0 em caso de sucesso
        }

        anterior = atual;
        atual = (*atual).prox;
    }

    return -2;  // Retorna -2 em caso de não existir a antena
}

/**
 * @brief Calcula os efeitos nefastos causados pelas antenas.
 * 
 * Esta função calcula os efeitos nefastos entre antenas com a mesma frequência,
 * com base na distância entre elas, e adiciona esses efeitos à lista de efeitos nefastos.
 * 
 * @param primeiraAntena Ponteiro para a primeira antena da lista.
 * @param primeiroNefasto Ponteiro para o ponteiro do primeiro efeito nefasto da lista.
 * @param numNefastos Número atual de efeitos nefastos na lista. Será incrementado.
 * 
 * @return Retorna 0 em caso de sucesso (existem nefastos).
 * @return Retorna -1 em caso de não haver nefastos ou nefastos suficientes para calcular (0 - 1).
 */
int calcularNefastos(Antena *primeiraAntena, Nefasto **primeiroNefasto, int *numNefastos)
{
    // Verifica se existem nefastos
    if (*numNefastos > 0) return -1;

    // Percorre todas as combinações de antenas (Bubble Sort)
    for (Antena *antenaAtual = primeiraAntena; antenaAtual != NULL; antenaAtual = (*antenaAtual).prox)
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

                // Adiciona os efeitos nefastos à lista (evitando os duplicados)
                adicionarNefasto(primeiroNefasto, NULL, numNefastos, nefasto1X, nefasto1Y, false);
                adicionarNefasto(primeiroNefasto, NULL, numNefastos, nefasto2X, nefasto2Y, false);
            }
        }
    }

    return 0;
}

/**
 * @brief Carrega as antenas a partir de um ficheiro.
 * 
 * Esta função lê um ficheiro de texto contendo informações sobre as antenas e adiciona as antenas à lista, 
 * uma por uma, com base nas coordenadas (x, y) e na frequência.
 * Cada letra no ficheiro é interpretada como uma antena.
 * 
 * @param primeiraAntena Ponteiro para o ponteiro da primeira antena da lista.
 * @param numAntenas Número de antenas na lista, que será incrementado.
 * @param localizacaoFicheiro Caminho para o ficheiro que contém as informações das antenas.
 * 
 * @return Retorna 0 em caso de sucesso.
 * @return Retorna -1 caso ocorra um erro ao abrir o ficheiro.
 */
int carregarAntenas(Antena **primeiraAntena, int *numAntenas, const char *localizacaoFicheiro)
{
    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return -1;

    // Variaveis de otimização e retorno de erro
    Antena *ultimaAntena = NULL;

    // Variáveis necessárias para leitura (getline)
    char *linha = NULL;
    size_t tamanho = 0;
    ssize_t lidos;

    // Percorre o ficheiro e verifica a existência de antenas
    for (int y = 0; (lidos = getline(&linha, &tamanho, ficheiro)) != -1; y++)
    {
        for (int x = 0; linha[x] != '\n' && linha[x] != '\0'; x++)
        {
            if (linha[x] >= 'A' && linha[x] <= 'Z')
            {
                // Adiciona a nova antena
                adicionarAntena(primeiraAntena, &ultimaAntena, numAntenas, &linha[x], x, y, true);
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
 * Esta função lê um ficheiro de texto contendo informações sobre os efeitos nefastos e adiciona os efeitos 
 * à lista, um por um, com base nas coordenadas (x, y). Cada símbolo '#' no ficheiro é interpretado como 
 * um efeito nefasto.
 * 
 * @param primeiroNefasto Ponteiro para o ponteiro do primeiro efeito nefasto da lista.
 * @param numNefastos Número de efeitos nefastos na lista, que será incrementado.
 * @param localizacaoFicheiro Caminho para o ficheiro que contém as informações dos efeitos nefastos.
 * 
 * @return Retorna 0 em caso de sucesso.
 * @return Retorna -1 se ocorrer um erro ao abrir o ficheiro.
 */
int carregarNefastos(Nefasto **primeiroNefasto, int *numNefastos, const char *localizacaoFicheiro)
{
    // Abre o ficheiro para leitura
    FILE *ficheiro = fopen(localizacaoFicheiro, "r");

    // Verifica se foi possível abrir o ficheiro
    if (ficheiro == NULL) return -1;

    // Variaveis de otimização e retorno de erro
    Nefasto *ultimoNefasto = NULL;

    // Variáveis necessárias para leitura (getline)
    char *linha = NULL;
    size_t tamanho = 0;
    ssize_t lidos;

    // Percorre o ficheiro e verifica a existência de antenas
    for (int y = 0; (lidos = getline(&linha, &tamanho, ficheiro)) != -1; y++)
    {
        for (int x = 0; linha[x] != '\n' && linha[x] != '\0'; x++)
        {
            if (linha[x] == '#')
            {
                // Cria um novo nefasto
                adicionarNefasto(primeiroNefasto, &ultimoNefasto, numNefastos, x, y, true);
            }
        }
    }

    free(linha);
    fclose(ficheiro); /* Fecha o ficheiro */

    return 0; /* Retorna 0 em caso de sucesso */
}

/**
 * @brief Apresenta as tabelas com informações das antenas e dos efeitos nefastos.
 * 
 * Esta função apresenta duas tabelas: uma com as antenas e as suas localizações (x, y), e outra com os efeitos 
 * nefastos e as suas localizações (x, y). As tabelas são formatadas e apresentadas no terminal.
 * 
 * @param primeiraAntena Ponteiro para a primeira antena da lista.
 * @param primeiroNefasto Ponteiro para o primeiro efeito nefasto da lista.
 * @param numAntenas Número de antenas na lista.
 * @param numNefastos Número de efeitos nefastos na lista.
 * @param traducoes Array de strings com as traduções para os títulos das tabelas.
 * 
 * @return Retorna o número de tabelas que foram impressas com valores (0-2).
 */
int apresentarTabela(Antena *primeiraAntena, Nefasto *primeiroNefasto, int numAntenas, int numNefastos, const char *traducoes[3])
{
    int impressoes = 0;

    // Imprime a tabela das antenas
    printf("%s (%d):\n", traducoes[0], numAntenas);

    printf("\n--------------------------------------------");
    printf("\n| %-15s | X          | Y          |", traducoes[1]);
    printf("\n--------------------------------------------");

    if (numAntenas > 0)
    {
        Antena *atual = primeiraAntena;

        while (atual != NULL)
        {
            printf("\n| %-15c| %-11d| %-11d|",
                (*atual).frequencia,
                (*atual).x,
                (*atual).y);
            atual = (*atual).prox;
        }

        impressoes++;
    }
    else
    {
        printf("\n|                |            |            |");
    }
        
    printf("\n--------------------------------------------");

    // Imprime a tabela das localizações nefastas
    printf("\n\n%s (%d):\n", traducoes[2], numNefastos);

    printf("\n---------------------------");
    printf("\n| X          | Y          |");
    printf("\n---------------------------");

    if (numNefastos > 0)
    {
        Nefasto *atual = primeiroNefasto;

        while (atual != NULL)
        {
            printf("\n| %-11d| %-11d|",
                (*atual).x,
                (*atual).y);
            atual = (*atual).prox;
        }

        impressoes++;
    }
    else
    {
        printf("\n|            |            |");
    }

    printf("\n---------------------------");

    return impressoes; /* Retorna o número de tabelas com valores imprimidas */
}