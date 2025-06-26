/**
 * @file Antenas.h
 * @author Gonçalo Carvalho (a31537@alunos.ipca.pt)
 * @brief Cabeçalho com definições e declarações para gestão de antenas e efeitos nefastos
 * @version 2.0
 * @date 2025-03-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#pragma once

// Bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

// Lista de Erros
#define LL_ERRO_REDE_PONTEIRO_INVALIDO -11
#define LL_ERRO_ANTENA_JA_EXISTE -5
#define LL_ERRO_ANTENA_NAO_EXISTE -6
#define LL_ERRO_NEFASTO_JA_EXISTE -17
#define LL_ERRO_NEFASTO_NAO_EXISTE -18
#define LL_ERRO_ABRIR_FICHEIRO -100
#define LL_ERRO_ALOCACAO_MEMORIA -404

/**  
 * @brief Estrutura para representar uma antena  
 * @note Cada antena tem uma frequência (caracter) e coordenadas (x, y) e apontador
 */ 
typedef struct Antena
{
    char frequencia;
    int x;
    int y;
    struct Antena *prox;

} Antena;

/**
 * @brief Estrutura para representar um efeito nefasto
 * @note Um efeito nefasto possui coordenadas (x, y) e apontador
 */
typedef struct Nefasto
{
    int x;
    int y;
    struct Nefasto *prox;

} Nefasto;

/**
 * @brief Estrutura para conter Antenas, Nefastos e número de cada
 */
typedef struct Rede
{
    Antena *primeiraAntena;
    int numAntenas;
    Nefasto *primeiroNefasto;
    int numNefastos;

} Rede;

// Declaração das funções
Rede *LL_criarRede();
Antena *LL_criarAntena(char frequencia, int x, int y);
Nefasto *LL_criarNefasto(int x, int y);

Rede *LL_libertarRede(Rede *rede);
int LL_libertarAntenas(Rede *rede);
int LL_libertarNefastos(Rede *rede);

int LL_adicionarAntenaOrdenada(Rede *rede, char *frequencia, int x, int y);
Antena *LL_adicionarAntenaFim(Rede *rede, Antena *ultimaAntena, char frequencia, int x, int y, int *erro);
int LL_adicionarNefastoOrdenado(Rede *rede, int x, int y);
Nefasto *LL_adicionarNefastoFim(Rede *rede, Nefasto *ultimoNefasto, int x, int y, int *erro);

int LL_removerAntena(Rede *rede, char *frequencia, int x, int y);

int LL_calcularNefastos(Rede *rede, bool coordenadasNegativas);

int LL_carregarAntenas(Rede *rede, const char *localizacaoFicheiro);
int LL_carregarNefastos(Rede *rede, const char *localizacaoFicheiro);

bool LL_apresentarAntenas(Rede *rede);
bool LL_apresentarNefastos(Rede *rede);