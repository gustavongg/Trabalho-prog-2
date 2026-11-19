
/**
 * @file main.c
 * @brief Ponto de entrada do Jogo do Milhao.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pergunta.h"
#include "arquivo.h"
#include "interface.h"

/**
 * @brief Funcao principal do Jogo do Milhao.
 *
 * Carrega as perguntas (binario ou CSV na primeira execucao),
 * mostra o menu ate o usuario sair e salva tudo no binario ao final.
 *
 * @return 0 se o programa terminou normalmente, 1 em caso de erro de memoria.
 */
int main(void) {
    ListaPerguntas lista;
    int opcao;

    iniciarInterface();

    if (!criarLista(&lista, 2)) {
        mostrarErro("Erro de memoria");
        return 1;
    }
    if (!carregarBinario(&lista, "dados/perguntas.dat")) {
        carregarCSV(&lista, "dados/perguntas.csv"); /* 1a execucao: binario ainda nao existe */
    }

    do {
        printf(COR_AMARELO "\n=== JOGO DO MILHAO ===\n" COR_RESET);
        printf("1 - Inserir\n");
        printf("2 - Listar\n");
        printf("3 - Pesquisar\n");
        printf("4 - Alterar\n");
        printf("5 - Excluir\n");
        printf("6 - Exportar CSV\n");
        printf("7 - Ver por nivel\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            while (getchar() != '\n'); /* joga fora o que foi digitado */
            opcao = -1;                /* cai no "Opcao invalida!" */
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
                        mostrarErro("Nivel invalido!");

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
                        mostrarErro("Letra invalida!");

                } while (nova.correta < 'A' || nova.correta > 'D');

                if (inserirPergunta(&lista, nova))
                    mostrarSucesso("Pergunta inserida!");
                else
                    mostrarErro("Erro ao inserir!");

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
                    printf(COR_CIANO "Pergunta encontrada: numero %d\n" COR_RESET, indice + 1);
                else
                    mostrarErro("Pergunta nao encontrada!");

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
                        mostrarErro("Nivel invalido!");

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
                        mostrarErro("Letra invalida!");

                } while (nova.correta < 'A' || nova.correta > 'D');

                if (alterarPergunta(&lista, numero - 1, nova))
                    mostrarSucesso("Pergunta alterada!");
                else
                    mostrarErro("Numero invalido!");

                break;
            }

            case 5: {
                int numero;

                printf("Digite o numero da pergunta que deseja excluir: ");
                scanf("%d", &numero);

                if (excluirPergunta(&lista, numero - 1))
                    mostrarSucesso("Pergunta excluida!");
                else
                    mostrarErro("Numero invalido!");

                break;
            }

            case 6: {

                if (exportarCSV(&lista, "dados/exportado.csv"))
                    mostrarSucesso("Exportado para dados/exportado.csv!");
                else
                    mostrarErro("Erro ao exportar!");
                break;
            }

            case 7: {
                int nivel;
                printf("Nivel (1-5): ");
                if (scanf("%d", &nivel) != 1 || nivel < 1 || nivel > 5) {
                    while (getchar() != '\n');
                    mostrarErro("Nivel invalido!");
                    break;
                }
                visualizarPorNivel(&lista, nivel);
                break;
            }

            case 0:
                printf("Saindo...\n");
                break;

            default:
                mostrarErro("Opcao invalida!");
        }

    } while (opcao != 0);

    salvarBinario(&lista, "dados/perguntas.dat");
    liberarLista(&lista);

    return 0;
}
