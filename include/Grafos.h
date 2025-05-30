/**
 * @file Antenas.h
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Cabeçalho com definições e declarações para as estruturas de grafos
 * @version 2.0
 * @date 2025-05-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @def MAX_X
 * @brief Valor máximo para coordenada X.
 *
 * Este valor define o limite de X (linhas) da cidade
 * onde estão posicionadas as antenas.
 */
extern const int MAX_X;

/**
 * @struct Aresta
 * @brief Representa uma ligação entre duas antenas (vértices) no grafo.
 *
 * Cada aresta aponta para o vértice de destino e para a próxima aresta na lista de adjacência,
 * permitindo representar múltiplas conexões a partir de um vértice.
 */
typedef struct Aresta
{
    struct Aresta *prox;
    struct Vertice *destino;

} Aresta;

/**
 * @struct Vertice
 * @brief Representa uma antena no grafo com os seus atributos e ligações.
 *
 * Cada vértice contém a frequência da antena, a sua posição na grelha,
 * um indicador de visitação (útil para percursos no grafo), e a lista de arestas
 * que representam as suas ligações a outras antenas.
 */
typedef struct Vertice
{
    char frequencia; /* (A - Z) */
    int x;
    int y;
    bool visitada;
    Aresta *primeiraAresta;
    struct Vertice *prox;

} Vertice;

/**
 * @struct Grafo
 * @brief Representa o grafo de antenas.
 *
 * O grafo é composto por um conjunto de antenas (vértices) e o número total
 * de antenas presentes. É a estrutura para representar a conexão
 * entre antenas com a mesma frequência.
 */
typedef struct Grafo
{
    int numAntenas;
    Vertice *primeiraAntena;

} Grafo;

// Declaração das funções
int libertarEstruturas(Grafo **cidade, int nivelLibertacao);
int resetVisitados(Grafo *cidade);

int carregarCidade(Grafo *cidade, const char *localizacaoFicheiro);
int interligarAntenas(Grafo *cidade, bool FrequenciasIguais, bool FrequenciasDiferentes, bool verificarRepetidas);

int adicionarCidade(Grafo **novo);
int adicionarAntenaCidade(Grafo *cidade, char *frequencia, int x, int y);
int adicionarAntenaCidadeCarregar(Grafo *cidade, Vertice **ultimaAntena, char frequencia, int x, int y);
int adicionarAresta(Vertice *inicio, Vertice *destino, bool verificarRepetidas);

int removerAntenaCidade(Grafo *cidade, char *frequencia, int x, int y);
int removerAresta(Vertice *inicio, Vertice *destino);

int procurarAntena(Grafo *cidade, Vertice **antena, int x, int y);
int procurarProfundidade(Vertice *inicio);
int procurarLargura(Vertice *inicio, int numeroAntenas);
int procurarCaminhos(Vertice *inicio, Vertice *destino, int numeroAntenas);
int procurarCaminhosRecursiva(Vertice *inicio, Vertice *destino, Vertice **caminho, int tamanho);

int listarIntersecoes(Grafo *cidade, char frequencia1, char frequencia2);
int listarAntenas(Grafo *cidade);
int listarArestasAntena(Vertice *antena);