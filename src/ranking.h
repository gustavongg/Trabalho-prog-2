/**
 * @file ranking.h
 * @brief Ranking das melhores partidas, guardado em um arquivo de texto.
 */
#ifndef RANKING_H
#define RANKING_H

/**
 * @brief Guarda o nome e o premio de uma partida no arquivo do ranking.
 * @param nome Nome do jogador.
 * @param premio Premio ganho em reais.
 * @return 1 se salvou, 0 se nao conseguiu abrir o arquivo.
 */
int salvarNoRanking(const char *nome, int premio);

/**
 * @brief Mostra na tela os 10 maiores premios.
 */
void mostrarRanking(void);

#endif
