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

/**
 * @def MAX_CAMINHO
 * @brief Tamanho máximo permitido para caminhos.
 *
 * Este valor é utilizado para limitar o comprimento do caminho
 * encontrado na função procurarCaminhos
 *
 */
#define MAX_CAMINHO 128

/**
 * @struct TabelaIdioma
 * @brief Estrutura que representa uma tabela de traduções por idioma.
 *
 * Esta estrutura contém o nome de um idioma e um conjunto de três traduções
 * correspondentes a esse idioma. Pode ser usada, por exemplo, para apresentar textos
 * da apresentarTabela() em várias línguas.
 *
 */
typedef struct
{
    const char *idioma;
    const char *traducoes[3];

} TabelaIdioma;

// Declaração das funções
int limparEcra();
void menuPrincipal(const char **traducoes);