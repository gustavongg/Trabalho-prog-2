/**
 * @file pergunta.c
 * @brief Implementacao da lista dinamica de perguntas.
 *
 * Funcoes de cadastro (CRUD) e visualizacao da lista dinamica de perguntas.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pergunta.h"

int criarLista(ListaPerguntas *lista, int capacidadeInicial) {
    lista->itens = malloc(capacidadeInicial * sizeof(Pergunta));
    if (lista->itens == NULL) return 0;
    lista->quantidade = 0;
    lista->capacidade = capacidadeInicial;
    return 1;
}

int inserirPergunta(ListaPerguntas *lista, Pergunta nova) {

    if (lista->quantidade == lista->capacidade) {
        int novaCapacidade = lista->capacidade * 2;
        Pergunta *temp = realloc(lista->itens, novaCapacidade * sizeof(Pergunta));
        if (temp == NULL) return 0;
        lista->itens = temp;
        lista->capacidade = novaCapacidade;
    }
    lista->itens[lista->quantidade] = nova;
    lista->quantidade++;
    return 1;
}

void listarPerguntas(const ListaPerguntas *lista) {
    if (lista->quantidade == 0) {
        printf("A lista esta vazia\n");
        return;
    }
    for (int i = 0; i < lista->quantidade; i++) {
        printf("Pergunta N:%d\n", i + 1);
        printf("Enunciado:%s\n", lista->itens[i].enunciado);
        printf("Nivel:%d\n", lista->itens[i].nivel);
        printf("A:%s\n", lista->itens[i].alternativas[0]);
        printf("B:%s\n", lista->itens[i].alternativas[1]);
        printf("C:%s\n", lista->itens[i].alternativas[2]);
        printf("D:%s\n", lista->itens[i].alternativas[3]);
        printf("\n");
    }
}

int pesquisarPergunta(const ListaPerguntas *lista, const char *texto) {

    for (int i = 0; i < lista->quantidade; i++) {
        if (strstr(lista->itens[i].enunciado, texto) != NULL) return i;
    }
    return -1;
}

int alterarPergunta(ListaPerguntas *lista, int indice, Pergunta nova) {
    if (indice < 0 || indice >= lista->quantidade) return 0;
    lista->itens[indice] = nova;
    return 1;
}

int excluirPergunta(ListaPerguntas *lista, int indice) {

    if (indice < 0 || indice >= lista->quantidade) return 0;

    for (int i = indice; i < lista->quantidade - 1; i++) {
        lista->itens[i] = lista->itens[i + 1];
    }

    lista->quantidade--;

    if (lista->quantidade < lista->capacidade / 2) {
        int novaCapacidade = lista->capacidade / 2;

        if (novaCapacidade < 1) novaCapacidade = 1;

        Pergunta *temp = realloc(lista->itens, novaCapacidade * sizeof(Pergunta));

        if (temp != NULL) {
            lista->itens = temp;
            lista->capacidade = novaCapacidade;
        }
    }

    return 1;
}

void liberarLista(ListaPerguntas *lista) {
    free(lista->itens);
    lista->itens = NULL;
    lista->quantidade = 0;
    lista->capacidade = 0;
}

void visualizarPorNivel(const ListaPerguntas *lista, int nivel) {
    /* 1. Separa as posicoes das perguntas desse nivel */
    int *indices = malloc(lista->quantidade * sizeof(int));
    if (indices == NULL) return;

    int total = 0;

    for (int i = 0; i < lista->quantidade; i++) {
        if ((int)lista->itens[i].nivel == nivel) {
            indices[total] = i;
            total++;
        }
    }

    if (total == 0) {
        printf("Nenhuma pergunta nesse nivel!\n");
        free(indices);
        return;
    }

    /* 2. Navega */
    int atual = 0;
    char opcao;

    do {
        Pergunta p = lista->itens[indices[atual]];

        printf("\n--- Nivel %d | Pergunta %d de %d ---\n", nivel, atual + 1, total);
        printf("%s\n", p.enunciado);

        for (int j = 0; j < 4; j++)
            printf("%c) %s\n", 'A' + j, p.alternativas[j]);

        printf("\n[P] Proxima  [A] Anterior  [S] Sair: ");
        scanf(" %c", &opcao);
        opcao = toupper(opcao);

        if (opcao == 'P') {
            if (atual < total - 1)
                atual++;
            else
                printf("Ja esta na ultima!\n");

        } else if (opcao == 'A') {
            if (atual > 0)
                atual--;
            else
                printf("Ja esta na primeira!\n");
        }

    } while (opcao != 'S');

    free(indices);
}
