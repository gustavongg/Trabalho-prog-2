/**
 * @file janela.h
 * @brief Partida do Jogo do Milhao em uma janela grafica (raylib).
 */
#ifndef JANELA_H
#define JANELA_H

#include "pergunta.h"

/**
 * @brief Abre uma janela e joga uma partida de 15 perguntas.
 *
 * A pescaria e feita clicando (ou apertando o numero) no ponto do rio onde o
 * peixe esta e as respostas clicando nos botoes ou com as teclas A, B, C e D.
 * Ajudas: soltar o peixe (P, 3x), chamar o VAR (V, 1x) e ouvir a torcida (T, 1x).
 *
 * @param lista Perguntas disponiveis (precisa ter 2/2/4/4/3 dos niveis 1 a 5).
 * @return Premio ganho em reais, ou 0 se nao foi possivel jogar.
 */
int jogarJanela(const ListaPerguntas *lista);

#endif
