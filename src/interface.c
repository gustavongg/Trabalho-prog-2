/**
 * @file interface.c
 * @brief Implementacao de cores e sons do jogo.
 */
#include <stdio.h>
#include <windows.h>
#include "interface.h"

/* Compiladores MinGW mais antigos nao definem essa constante */
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void iniciarInterface(void) {
    /* O terminal do Windows so entende os codigos de cor depois disso */
    HANDLE saida = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD modo;
    GetConsoleMode(saida, &modo);
    SetConsoleMode(saida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void mostrarErro(const char *mensagem) {
    printf(COR_VERMELHO "%s\n" COR_RESET, mensagem);
    Beep(300, 300); /* frequencia baixa = som grave */
}

void mostrarSucesso(const char *mensagem) {
    printf(COR_VERDE "%s\n" COR_RESET, mensagem);
    Beep(1000, 150); /* frequencia alta = som agudo */
}
