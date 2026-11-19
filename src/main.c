
/**
 * @file main.c
 * @brief Ponto de entrada do Jogo do Milhao.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pergunta.h"

int main(void) {
    ListaPerguntas lista;
    int opcao;

    if (!criarLista(&lista, 2)) {
        printf("Erro de memoria\n");
        return 1;
    }

    do {
        printf("\n=== JOGO DO MILHAO ===\n");
        printf("1 - Inserir\n");
        printf("2 - Listar\n");
        printf("3 - Pesquisar\n");
        printf("4 - Alterar\n");
        printf("5 - Excluir\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
        while (getchar() != '\n');   /* joga fora o que foi digitado */
        opcao = -1;                  /* cai no "Opcao invalida!" */
}
        switch (opcao) {

            case 1: {
                Pergunta nova;
                int nivel;

                printf("Digite o enunciado: ");
                scanf(" %199[^\n]", nova.enunciado);

                do {
                    printf("Digite o nivel (1-5): ");
                    scanf("%d", &nivel);

                    if (nivel < 1 || nivel > 5)
                        printf("Nivel invalido!\n");

                } while (nivel < 1 || nivel > 5);

                nova.nivel = nivel;

                for (int i = 0; i < 4; i++) {
                    printf("Alternativa %c: ", 'A' + i);
                    scanf(" %99[^\n]", nova.alternativas[i]);
                }

                do {
                    printf("Letra correta (A-D): ");
                    scanf(" %c", &nova.correta);

                    if (nova.correta >= 'a' && nova.correta <= 'd')
                        nova.correta -= 32;

                    if (nova.correta < 'A' || nova.correta > 'D')
                        printf("Letra invalida!\n");

                } while (nova.correta < 'A' || nova.correta > 'D');

                if (inserirPergunta(&lista, nova))
                    printf("Pergunta inserida!\n");
                else
                    printf("Erro ao inserir!\n");

                break;
            }

            case 2:
                listarPerguntas(&lista);
                break;

            case 3: {
                char texto[200];

                printf("Digite o texto para pesquisar: ");
                scanf(" %199[^\n]", texto);

                int indice = pesquisarPergunta(&lista, texto);

                if (indice != -1)
                    printf("Pergunta encontrada: numero %d\n", indice + 1);
                else
                    printf("Pergunta nao encontrada!\n");

                break;
            }

            case 4: {
                int numero;
                int nivel;
                Pergunta nova;

                printf("Digite o numero da pergunta que deseja alterar: ");
                scanf("%d", &numero);

                printf("Digite o novo enunciado: ");
                scanf(" %199[^\n]", nova.enunciado);

                do {
                    printf("Digite o novo nivel (1-5): ");
                    scanf("%d", &nivel);

                    if (nivel < 1 || nivel > 5)
                        printf("Nivel invalido!\n");

                } while (nivel < 1 || nivel > 5);

                nova.nivel = nivel;

                for (int i = 0; i < 4; i++) {
                    printf("Alternativa %c: ", 'A' + i);
                    scanf(" %99[^\n]", nova.alternativas[i]);
                }

                do {
                    printf("Letra correta (A-D): ");
                    scanf(" %c", &nova.correta);

                    if (nova.correta >= 'a' && nova.correta <= 'd')
                        nova.correta -= 32;

                    if (nova.correta < 'A' || nova.correta > 'D')
                        printf("Letra invalida!\n");

                } while (nova.correta < 'A' || nova.correta > 'D');

                if (alterarPergunta(&lista, numero - 1, nova))
                    printf("Pergunta alterada!\n");
                else
                    printf("Numero invalido!\n");

                break;
            }

            case 5: {
                int numero;

                printf("Digite o numero da pergunta que deseja excluir: ");
                scanf("%d", &numero);

                if (excluirPergunta(&lista, numero - 1))
                    printf("Pergunta excluida!\n");
                else
                    printf("Numero invalido!\n");

                break;
            }

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    liberarLista(&lista);

    return 0;
}
