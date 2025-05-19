#pragma once

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @def MAX_LINHAS
 * @brief Número máximo de linhas permitido para a grelha de antenas.
 *
 * Este valor define o limite de linhas da cidade
 * onde estão posicionadas as antenas.
 */
#define MAX_LINHAS 100

/**
 * @def MAX_COLUNAS
 * @brief Número máximo de colunas permitido para a grelha de antenas.
 *
 * Este valor define o limite de colunas da cidade
 * onde estão posicionadas as antenas.
 */
#define MAX_COLUNAS 100

/**
 * @struct Aresta
 * @brief Representa uma ligação entre duas antenas (vértices) no grafo.
 *
 * Cada aresta aponta para o vértice de destino e para a próxima aresta na lista de adjacência,
 * permitindo representar múltiplas conexões a partir de um vértice.
 *
 */
typedef struct Aresta
{
    int destino;  /* (0, 1, 2, 3, ...) */
    struct Aresta* prox;

} Aresta;

/**
 * @struct Vertice
 * @brief Representa uma antena no grafo com os seus atributos e ligações.
 *
 * Cada vértice contém a frequência da antena, a sua posição na grelha,
 * um indicador de visitação (útil para percursos no grafo), e a lista de arestas
 * que representam as suas ligações a outras antenas.
 *
 */
typedef struct Vertice
{
    char frequencia; /* (A - Z) */
    int x;
    int y;
    bool visitada;
    Aresta* adj;

} Vertice;

/**
 * @struct Grafo
 * @brief Representa o grafo de antenas.
 *
 * O grafo é composto por um conjunto de antenas (vértices) e o número total
 * de antenas presentes. É a estrutura para representar a conexão
 * entre antenas com a mesma frequência.
 *
 */
typedef struct Grafo
{
    int numAntenas;
    Vertice* antenas;

} Grafo;

// Declaração das funções
int libertarCidade(Grafo* cidade);
Grafo* adicionarCidade(int numVertices);
int adicionarAresta(Grafo* cidade, int inicio, int destino);
int carregarCidade(Grafo** cidade, const char* localizacaoFicheiro);
int resetVisitados(Grafo* cidade);
int procurarProfundidade(Grafo* cidade, int antena);
int procurarLargura(Grafo* cidade, int inicio);
int procurarCaminhos(Grafo* cidade, int inicio, int destino, int* caminho, int tamanho);
int listarIntersecoes(Grafo *cidade, char frequencia1, char frequencia2);