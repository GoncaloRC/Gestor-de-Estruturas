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

// Variáveis globais
extern const int MAX_X;

// Declaração das funções
int libertarAntenas(Antena **primeiraAntena, int *numAntenas);
int libertarNefastos(Nefasto **primeiroNefasto, int *numNefastos);

int adicionarAntena(Antena **primeiraAntena, Antena **ultimaAntena, int *numAntenas, char *frequencia, int x, int y, bool inserirNoFim);
int adicionarNefasto(Nefasto **primeiroNefasto, Nefasto **ultimoNefasto, int *numNefastos, int x, int y, bool inserirNoFim);

int removerAntena(Antena **primeiraAntena, int *numAntenas, char *frequencia, int x, int y);

int calcularNefastos(Antena *primeiraAntena, Nefasto **primeiroNefasto, int *numNefastos);

int carregarAntenas(Antena **primeiraAntena, int *numAntenas, const char *localizacaoFicheiro);
int carregarNefastos(Nefasto **primeiroNefasto, int *numNefastos, const char *localizacaoFicheiro);

int apresentarTabela(Antena *primeiraAntena, Nefasto *primeiroNefasto, int numAntenas, int numNefastos, const char *traducoes[3]);