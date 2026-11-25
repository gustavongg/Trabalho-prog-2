/**
 * @file jogo.c
 * @brief Regras da partida (usadas pela janela).
 */
#include <stdio.h>
#include <stdlib.h>
#include "jogo.h"

/* Premio de cada pergunta (a 5 e a 10 sao os marcos de seguranca) */
const int PREMIOS[NUM_QUESTOES] = {1000,  2000,  3000,  4000,   5000,   10000, 20000, 30000,
                                   40000, 50000, 100000, 200000, 300000, 500000, 1000000};

/* Quantas perguntas de cada nivel a partida usa (posicao 0 nao e usada) */
static const int NECESSARIAS[6] = {0, 2, 2, 4, 4, 3};

int nivelDaQuestao(int q) {
    if (q < 2) return 1;
    if (q < 4) return 2;
    if (q < 8) return 3;
    if (q < 12) return 4;
    return 5;
}

/**
 * @brief Conta quantas perguntas da lista sao de um nivel.
 * @param lista Lista de perguntas.
 * @param nivel Nivel de 1 a 5.
 * @return Quantidade de perguntas do nivel.
 */
static int contarNivel(const ListaPerguntas *lista, int nivel) {
    int total = 0;
    for (int i = 0; i < lista->quantidade; i++) {
        if ((int)lista->itens[i].nivel == nivel) total++;
    }
    return total;
}

int sortearPergunta(const ListaPerguntas *lista, const int *usadas, int nivel) {
    int i;
    do {
        i = rand() % lista->quantidade;
    } while (usadas[i] || (int)lista->itens[i].nivel != nivel);
    return i;
}

int existePerguntaLivre(const ListaPerguntas *lista, const int *usadas, int nivel) {
    for (int i = 0; i < lista->quantidade; i++) {
        if (!usadas[i] && (int)lista->itens[i].nivel == nivel) return 1;
    }
    return 0;
}

int listaSuficiente(const ListaPerguntas *lista) {
    for (int nivel = 1; nivel <= 5; nivel++) {
        if (contarNivel(lista, nivel) < NECESSARIAS[nivel]) {
            printf("Faltam perguntas de nivel %d: precisa de %d.\n", nivel, NECESSARIAS[nivel]);
            return 0;
        }
    }
    return 1;
}

int premioAoErrar(int q) {
    /* errar leva o ultimo marco: 5 perguntas certas = R$ 5000, 10 = R$ 50000 */
    return q >= 10 ? PREMIOS[9] : (q >= 5 ? PREMIOS[4] : 0);
}
