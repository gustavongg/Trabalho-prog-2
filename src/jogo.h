/**
 * @file jogo.h
 * @brief Regras da partida do Jogo do Milhao.
 */
#ifndef JOGO_H
#define JOGO_H

#include "pergunta.h"

#define NUM_QUESTOES 15 /**< Perguntas de uma partida */

/** Premio de cada pergunta (a 5 e a 10 sao os marcos de seguranca). */
extern const int PREMIOS[NUM_QUESTOES];

/**
 * @brief Diz o nivel da pergunta numero q (0 a 14).
 * @param q Posicao da pergunta na partida.
 * @return Nivel de 1 a 5.
 */
int nivelDaQuestao(int q);

/**
 * @brief Sorteia uma pergunta do nivel que ainda nao foi usada.
 * @param lista Lista de perguntas.
 * @param usadas Vetor com 1 nas perguntas ja usadas.
 * @param nivel Nivel desejado.
 * @return Indice da pergunta sorteada.
 */
int sortearPergunta(const ListaPerguntas *lista, const int *usadas, int nivel);

/**
 * @brief Diz se ainda sobrou alguma pergunta do nivel que nao foi usada.
 *
 * Usada antes de sortearPergunta na ajuda "soltar o peixe", porque
 * sortearPergunta ficaria procurando para sempre se nao sobrasse nenhuma.
 *
 * @param lista Lista de perguntas.
 * @param usadas Vetor com 1 nas perguntas ja usadas.
 * @param nivel Nivel desejado.
 * @return 1 se sobrou alguma, 0 se nao.
 */
int existePerguntaLivre(const ListaPerguntas *lista, const int *usadas, int nivel);

/**
 * @brief Confere se a lista tem perguntas suficientes de cada nivel (2/2/4/4/3).
 * @param lista Lista de perguntas.
 * @return 1 se tem, 0 se nao (e avisa na tela).
 */
int listaSuficiente(const ListaPerguntas *lista);

/**
 * @brief Premio que o jogador leva ao errar a pergunta numero q (marcos nas 5 e 10).
 * @param q Posicao da pergunta errada (0 a 14).
 * @return Premio em reais.
 */
int premioAoErrar(int q);

#endif
