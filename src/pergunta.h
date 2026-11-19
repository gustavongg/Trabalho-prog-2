/**
 * @file pergunta.h
 * @brief Estrutura Pergunta e funcoes da lista dinamica de perguntas.
 */
#ifndef PERGUNTA_H
#define PERGUNTA_H

#define TAM_ENUNCIADO    256 /**< Tamanho maximo do enunciado */
#define TAM_ALTERNATIVA  100 /**< Tamanho maximo de cada alternativa */
#define NUM_ALTERNATIVAS 4   /**< Quantidade de alternativas (A, B, C, D) */

/**
 * @brief Niveis de dificuldade das perguntas.
 */
typedef enum {
    MUITO_FACIL = 1, /**< Perguntas 1 e 2 */
    FACIL,           /**< Perguntas 3 e 4 */
    MEDIO,           /**< Perguntas 5 a 8 */
    DIFICIL,         /**< Perguntas 9 a 12 */
    MUITO_DIFICIL    /**< Perguntas 13 a 15 */
} NivelDificuldade;

/**
 * @brief Representa uma pergunta do jogo.
 */
typedef struct {
    char enunciado[TAM_ENUNCIADO];                        /**< Texto da pergunta */
    char alternativas[NUM_ALTERNATIVAS][TAM_ALTERNATIVA]; /**< Alternativas A, B, C e D */
    char correta;                                         /**< Letra correta: 'A' a 'D' */
    NivelDificuldade nivel;                               /**< Nivel de 1 a 5 */
} Pergunta;

/**
 * @brief Lista dinamica de perguntas (cresce e diminui com realloc).
 */
typedef struct {
    Pergunta *itens; /**< Vetor alocado dinamicamente */
    int quantidade;  /**< Quantas perguntas estao guardadas */
    int capacidade;  /**< Quantas cabem antes de precisar de realloc */
} ListaPerguntas;

/**
 * @brief Inicializa a lista com uma capacidade inicial.
 * @param lista Ponteiro para a lista.
 * @param capacidadeInicial Quantidade de posicoes alocadas no inicio.
 * @return 1 se deu certo, 0 se faltou memoria.
 */
int criarLista(ListaPerguntas *lista, int capacidadeInicial);

/**
 * @brief Insere uma pergunta no fim da lista, aumentando o vetor se precisar.
 * @param lista Ponteiro para a lista.
 * @param nova Pergunta a ser copiada para a lista.
 * @return 1 se deu certo, 0 se faltou memoria.
 */
int inserirPergunta(ListaPerguntas *lista, Pergunta nova);

/**
 * @brief Mostra todas as perguntas da lista na tela.
 * @param lista Ponteiro para a lista.
 */
void listarPerguntas(const ListaPerguntas *lista);

/**
 * @brief Procura a primeira pergunta cujo enunciado contem o texto buscado.
 * @param lista Ponteiro para a lista.
 * @param texto Trecho a procurar.
 * @return Indice da pergunta encontrada ou -1 se nao achou.
 */
int pesquisarPergunta(const ListaPerguntas *lista, const char *texto);

/**
 * @brief Substitui a pergunta de uma posicao pelos novos dados.
 * @param lista Ponteiro para a lista.
 * @param indice Posicao da pergunta a alterar.
 * @param nova Novos dados da pergunta.
 * @return 1 se deu certo, 0 se o indice for invalido.
 */
int alterarPergunta(ListaPerguntas *lista, int indice, Pergunta nova);

/**
 * @brief Remove a pergunta de uma posicao e diminui o vetor.
 * @param lista Ponteiro para a lista.
 * @param indice Posicao da pergunta a excluir.
 * @return 1 se deu certo, 0 se o indice for invalido.
 */
int excluirPergunta(ListaPerguntas *lista, int indice);

/**
 * @brief Libera a memoria da lista.
 * @param lista Ponteiro para a lista.
 */
void liberarLista(ListaPerguntas *lista);

#endif
