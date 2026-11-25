
/**
 * @file main.c
 * @brief Ponto de entrada do Jogo do Milhao.
 */

#include <stdio.h>
#include <stdlib.h>
#include "pergunta.h"
#include "arquivo.h"
#include "interface.h"
#include "jogo.h"
#include "janela.h"
#include "ranking.h"

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
        /* Menu no tema da pescaria; so caracteres ASCII para nao quebrar no terminal do Windows */
        printf(COR_CIANO "\n        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" COR_RESET);
        printf(COR_AMARELO "             J O G O   D O   M I L H A O\n" COR_RESET);
        printf(COR_CIANO   "                ><(((('>   pesca do CR7\n" COR_RESET);
        printf(COR_CIANO "        ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" COR_RESET);
        printf("        +--------------------------------------+\n");
        printf("        |" COR_AMARELO "  CAIXA DE ISCAS (perguntas)          " COR_RESET "|\n");
        printf("        |   1 - Inserir pergunta               |\n");
        printf("        |   2 - Listar perguntas               |\n");
        printf("        |   3 - Pesquisar pergunta             |\n");
        printf("        |   4 - Alterar pergunta               |\n");
        printf("        |   5 - Excluir pergunta               |\n");
        printf("        |   6 - Exportar para CSV              |\n");
        printf("        |   7 - Ver perguntas por nivel        |\n");
        printf("        +--------------------------------------+\n");
        printf("        |" COR_VERDE "  NA BEIRA DO RIO                     " COR_RESET "|\n");
        printf("        |   8 - Ir pescar (Jogar)              |\n");
        printf("        |   9 - Mural dos pescadores (Ranking) |\n");
        printf("        +--------------------------------------+\n");
        printf("        |" COR_VERMELHO "   0 - Recolher a vara (Sair)         " COR_RESET "|\n");
        printf("        +--------------------------------------+\n");
        printf(COR_CIANO "Equipe: Gustavo Nicole Geraldo, Joao Pedro Rodrigues Ribeiro e Joao Pedro Damiao\n" COR_RESET);
        printf("Escolha sua isca: ");
        opcao = lerInteiro(); /* letras viram -1 e caem no "Opcao invalida!" */
        switch (opcao) {

            case 1: {
                Pergunta nova;
                int nivel;

                printf("Digite o enunciado: ");
                scanf(" %199[^\n]", nova.enunciado);

                do {
                    printf("Digite o nivel (1-5): ");
                    nivel = lerInteiro();

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
                numero = lerInteiro();
                if (numero < 1 || numero > lista.quantidade) { /* avisa antes de pedir os novos dados */
                    mostrarErro("Numero invalido!");
                    break;
                }

                printf("Digite o novo enunciado: ");
                scanf(" %199[^\n]", nova.enunciado);

                do {
                    printf("Digite o novo nivel (1-5): ");
                    nivel = lerInteiro();

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
                numero = lerInteiro();

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
                nivel = lerInteiro();
                if (nivel < 1 || nivel > 5) {
                    mostrarErro("Nivel invalido!");
                    break;
                }
                visualizarPorNivel(&lista, nivel);
                break;
            }

            case 8: {
                int premio = jogarJanela(&lista);

                if (premio > 0) {
                    char nome[30];
                    printf("Seu nome para o ranking: ");
                    scanf(" %29[^\n]", nome);
                    salvarNoRanking(nome, premio);
                }
                break;
            }

            case 9:
                mostrarRanking();
                break;

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
