/**
 * @file interface.h
 * @brief Cores, sons e utilidades de tela.
 */
#ifndef INTERFACE_H
#define INTERFACE_H

#define COR_RESET "\033[0m"      /**< Volta a cor normal */
#define COR_VERMELHO "\033[31m"  /**< Erros */
#define COR_VERDE "\033[32m"     /**< Sucesso */
#define COR_AMARELO "\033[1;33m" /**< Titulos */
#define COR_CIANO "\033[36m"     /**< Destaques */

/**
 * @brief Ativa as cores no terminal do Windows.
 */
void iniciarInterface(void);

/**
 * @brief Le um numero inteiro digitado pelo usuario sem travar com letras.
 *
 * Sempre descarta o resto da linha, para que letras digitadas por engano
 * nao fiquem presas na entrada e atrapalhem a proxima leitura.
 *
 * @return O numero digitado, -1 se nao foi digitado um numero, ou 0 se a entrada acabou.
 */
int lerInteiro(void);

/**
 * @brief Mostra uma mensagem de erro em vermelho com beep grave.
 * @param mensagem Texto a ser exibido.
 */
void mostrarErro(const char *mensagem);

/**
 * @brief Mostra uma mensagem de sucesso em verde com beep agudo.
 * @param mensagem Texto a ser exibido.
 */
void mostrarSucesso(const char *mensagem);

#endif
