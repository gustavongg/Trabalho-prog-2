/**
 * @file ranking.c
 * @brief Implementacao do ranking (arquivo dados/ranking.txt, uma partida por linha).
 */
#include <stdio.h>
#include "ranking.h"

#define ARQ_RANKING "dados/ranking.txt"
#define MAX_JOGADORES 100

typedef struct {
    char nome[30];
    int premio;
} Jogador;

int salvarNoRanking(const char *nome, int premio) {
    FILE *arq = fopen(ARQ_RANKING, "a"); /* a = acrescenta no fim */
    if (arq == NULL) return 0;
    fprintf(arq, "%d %s\n", premio, nome);
    fclose(arq);
    return 1;
}

void mostrarRanking(void) {
    Jogador jogadores[MAX_JOGADORES];
    int total = 0;

    FILE *arq = fopen(ARQ_RANKING, "r");
    if (arq == NULL) {
        printf("Ninguem jogou ainda.\n");
        return;
    }
    while (total < MAX_JOGADORES && fscanf(arq, "%d %29[^\n]", &jogadores[total].premio, jogadores[total].nome) == 2) {
        total++;
    }
    fclose(arq);

    /* ordena do maior premio para o menor (bubble sort) */
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - 1 - i; j++) {
            if (jogadores[j].premio < jogadores[j + 1].premio) {
                Jogador troca = jogadores[j];
                jogadores[j] = jogadores[j + 1];
                jogadores[j + 1] = troca;
            }
        }
    }

    printf("\n=== RANKING ===\n");
    for (int i = 0; i < total && i < 10; i++) {
        printf("%2d. %-29s R$ %d\n", i + 1, jogadores[i].nome, jogadores[i].premio);
    }
}
