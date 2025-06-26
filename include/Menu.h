/**
 * @file Antenas.h
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Cabeçalho com definições e declarações para o menu
 * @version 2.0
 * @date 2025-05-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

// Bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

// Headers
#include "../include/Listas_Ligadas.h"
#include "../include/Grafos.h"

/**
 * @def MAX_CAMINHO
 * @brief Tamanho máximo permitido para caminhos.
 *
 * Este valor é utilizado para limitar o comprimento do caminho
 * encontrado na função procurarCaminhos
 *
 */
#define MAX_CAMINHO 128

#define MAX_QUESTOES 2
#define MAX_RESULTADOS 2

#define QUESTAO_INTERLIGAR_ANTENAS "Pretende interligar as antenas da cidade com a mesma frequência (s/n)?"
#define QUESTAO_ADICIONAR_ARESTA_RETORNO "Pretende adicionar a aresta de retorno (s/n)?"
#define QUESTAO_REMOVER_ARESTA_RETORNO "Pretende remover a aresta de retorno (s/n)?"

typedef struct Dados
{
    char frequencia[MAX_QUESTOES];
    int x[MAX_QUESTOES];
    int y[MAX_QUESTOES];
    Vertice *antena[MAX_QUESTOES];

} Dados;

// Declaração das funções
int limparEcra();
Vertice *procurarAntenaMenu(Grafo *cidade, Dados *dados, int x, int y);
bool requesitarDados(Grafo *cidade, Dados *dados, int numFrequencias, int numCoordenadas, bool procurarAntena);
bool requesitarResposta(const char *questao);
int imprimirErros(Dados *dados, int resultado);
void menuPrincipal();