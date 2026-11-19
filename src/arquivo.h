/**
 * @file arquivo.h
 * @brief Leitura e gravacao das perguntas em arquivos (binario e CSV).
 */
#ifndef ARQUIVO_H
#define ARQUIVO_H

#include "pergunta.h"

/**
 * @brief Salva todas as perguntas da lista em um arquivo binario.
 * @param lista Lista com as perguntas.
 * @param nomeArquivo Caminho do arquivo binario.
 * @return 1 se salvou, 0 se nao conseguiu abrir o arquivo.
 */
int salvarBinario(const ListaPerguntas *lista, const char *nomeArquivo);

/**
 * @brief Carrega as perguntas de um arquivo binario para a lista.
 * @param lista Lista que recebe as perguntas.
 * @param nomeArquivo Caminho do arquivo binario.
 * @return 1 se carregou, 0 se o arquivo nao existe (primeira execucao).
 */
int carregarBinario(ListaPerguntas *lista, const char *nomeArquivo);

/**
 * @brief Exporta todas as perguntas para um arquivo CSV.
 * @param lista Lista com as perguntas.
 * @param nomeArquivo Caminho do arquivo CSV.
 * @return 1 se exportou, 0 se nao conseguiu abrir o arquivo.
 */
int exportarCSV(const ListaPerguntas *lista, const char *nomeArquivo);

/**
 * @brief Carrega as perguntas de um arquivo CSV para a lista.
 * @param lista Lista que recebe as perguntas.
 * @param nomeArquivo Caminho do arquivo CSV.
 * @return 1 se carregou, 0 se nao conseguiu abrir o arquivo.
 */
int carregarCSV(ListaPerguntas *lista, const char *nomeArquivo);
#endif
