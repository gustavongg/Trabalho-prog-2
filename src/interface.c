/**
 * @file interface.c
 * @brief Implementacao de cores e sons do jogo.
 */
#include <stdio.h>
#include "interface.h"

#ifdef _WIN32 /* so o Windows precisa (e tem) o windows.h; no Linux o terminal ja entende as cores */
#include <windows.h>

/* Compiladores MinGW mais antigos nao definem essa constante */
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define BIPE(freq, ms) Beep(freq, ms)
#else
#define BIPE(freq, ms) (printf("\a"), fflush(stdout)) /* no Linux: bipe padrao do terminal */
#endif

void iniciarInterface(void) {
#ifdef _WIN32
    /* O terminal do Windows so entende os codigos de cor depois disso */
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo;
    GetConsoleMode(saida, &modo);
    SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

int lerInteiro(void) {
    int valor;
    int leu = scanf("%d", &valor);
    int c;

    if (leu == EOF) return 0; /* entrada acabou: 0 faz o menu sair (e salvar) em vez de travar */
    while ((c = getchar()) != '\n' && c != EOF); /* joga fora o resto da linha (ex.: letras) */
    return leu == 1 ? valor : -1;
}

void mostrarErro(const char *mensagem) {
    printf(COR_VERMELHO "%s\n" COR_RESET, mensagem);
    BIPE(300, 300); /* frequencia baixa = som grave */
}

void mostrarSucesso(const char *mensagem) {
    printf(COR_VERDE "%s\n" COR_RESET, mensagem);
    BIPE(1000, 150); /* frequencia alta = som agudo */
}
