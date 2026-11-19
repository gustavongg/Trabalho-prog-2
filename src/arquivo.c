/**
 * @file arquivo.c
 * @brief Implementacao da leitura e gravacao das perguntas em arquivos.
 */
#include <stdio.h>
#include "arquivo.h"
#include <string.h>
#include <stdlib.h>

int salvarBinario(const ListaPerguntas *lista, const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "wb"); // w = escrever, b = binário
    if (arq == NULL) return 0;

    fwrite(&lista->quantidade, sizeof(int), 1, arq);
    fwrite(lista->itens, sizeof(Pergunta), lista->quantidade, arq);

    fclose(arq);
    return 1;
}

int carregarBinario(ListaPerguntas *lista, const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "rb"); // r = ler, b = binário
    if (arq == NULL) return 0;            // primeira execução: arquivo ainda não existe

    int qtd;
    fread(&qtd, sizeof(int), 1, arq);

    for (int i = 0; i < qtd; i++) {
        Pergunta p;
        fread(&p, sizeof(Pergunta), 1, arq);
        inserirPergunta(lista, p);
    }

    fclose(arq);
    return 1;
}

int exportarCSV(const ListaPerguntas *lista, const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "w"); // escrever TEXTO (sem o b)
    if (arq == NULL) return 0;

    fprintf(arq, "enunciado,alternativa_a,alternativa_b,alternativa_c,alternativa_d,correta,nivel\n");

    for (int i = 0; i < lista->quantidade; i++) {
        fprintf(arq, "%s,%s,%s,%s,%s,%c,%d\n",
                lista->itens[i].enunciado,
                lista->itens[i].alternativas[0],
                lista->itens[i].alternativas[1],
                lista->itens[i].alternativas[2],
                lista->itens[i].alternativas[3],
                lista->itens[i].correta,
                lista->itens[i].nivel);
    }

    fclose(arq);
    return 1;
}

int carregarCSV(ListaPerguntas *lista, const char *nomeArquivo) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (arq == NULL) return 0;

    char linha[1024];
    fgets(linha, sizeof(linha), arq);

    while (fgets(linha, sizeof(linha), arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        if (linha[0] == '\0') continue;

        Pergunta p;
        char *campo = strtok(linha, ",");
        strcpy(p.enunciado, campo);

        for (int j = 0; j < 4; j++) {
            campo = strtok(NULL, ",");
            strcpy(p.alternativas[j], campo);
        }

        campo = strtok(NULL, ",");
        p.correta = campo[0];

        campo = strtok(NULL, ",");
        p.nivel = atoi(campo);

        inserirPergunta(lista, p);
    }

    fclose(arq);
    return 1;
}
