/**
 * @file janela.c
 * @brief Parte grafica da partida (raylib): pescaria, CR7, perguntas no estilo Genio Quiz e sistema de ajuda.
 */

// ============================================================================
// BIBLIOTECAS E CABEÇALHOS DO PROJETO
// ============================================================================
#include "raylib.h"    // Biblioteca gráfica responsável pela janela, desenhos 2D, mouse e teclado
#include "pergunta.h"  // Definições das structs Pergunta e ListaPerguntas
#include "jogo.h"      // Protótipos das regras do jogo (sortearPergunta, nivelDaQuestao, premioAoErrar, etc.)
#include "janela.h"    // Protótipo de jogarJanela (o main.c chama esta função)
#include <stdio.h>     // Funções padrão de entrada/saída (ex: snprintf)
#include <stdlib.h>    // Alocação dinâmica de memória (calloc, free) e números aleatórios (rand, srand)
#include <string.h>    // Manipulação de strings (strncpy, strcpy, strlen, strtok)
#include <ctype.h>     // Manipulação de caracteres (toupper para padronizar 'a'-'d' em 'A'-'D')
#include <time.h>      // Função time() usada como semente para o gerador de números aleatórios

// ============================================================================
// CONSTANTES E ESTRUTURAS AUXILIARES DA INTERFACE
// ============================================================================

// Os valores dos premios vem da tabela PREMIOS do jogo.h (a mesma usada no resto do programa)

/** Nomes dos peixes de cada um dos 5 niveis (posicao 0 = nivel 1). */
static const char *NOMES_PEIXES[5] = {
    "Lambari",   /**< Nivel 1 (Muito Facil) */
    "Tilapia",   /**< Nivel 2 (Facil) */
    "Tucunare",  /**< Nivel 3 (Medio) */
    "Dourado",   /**< Nivel 4 (Dificil) */
    "Pirarucu"   /**< Nivel 5 (Muito Dificil) */
};

/**
 * @brief Maquina de estados: qual tela esta sendo exibida no momento.
 */
typedef enum {
    FASE_PESCA,      /**< Tela do rio onde o jogador escolhe onde lancar o anzol */
    FASE_SIUUU,      /**< Comemoracao do CR7 quando fisga o peixe */
    FASE_PERGUNTA,   /**< Tela principal no estilo Genio Quiz (enunciado + 4 botoes + ajudas) */
    FASE_RESULTADO   /**< Tela final: acertou tudo, parou ou "ERROU!" */
} FaseRaylib;

// ============================================================================
// FUNÇÕES AUXILIARES DE DESENHO (ESTILO GÊNIO QUIZ E CR7)
// ============================================================================

/**
 * @brief Desenha um retangulo preenchido com borda grossa, no estilo dos botoes do Genio Quiz.
 * @param rec Posicao e tamanho do retangulo.
 * @param fundo Cor de dentro.
 * @param borda Cor da borda.
 * @param espessura Espessura da borda em pixels.
 */
static void DesenharCaixaGenio(Rectangle rec, Color fundo, Color borda, float espessura) {
    DrawRectangleRec(rec, fundo);                 // Pinta o interior do botão/quadro
    DrawRectangleLinesEx(rec, espessura, borda);  // Desenha a borda externa com a espessura definida
}

/**
 * @brief Desenha o CR7 com formas simples da raylib (circulos, retangulos e linhas).
 * @param x Posicao horizontal do tronco.
 * @param y Posicao vertical do tronco.
 * @param comemorando true = pose do "SIUUU", false = pescando com a vara.
 */
static void DesenharCR7(int x, int y, bool comemorando) {
    // Cabeça e topete
    DrawCircle(x, y - 55, 18, BEIGE);
    DrawRectangle(x - 16, y - 75, 32, 10, BLACK);

    // Camisa vermelha de Portugal com o número 7
    DrawRectangle(x - 18, y - 35, 36, 45, RED);
    DrawText("7", x - 5, y - 25, 20, WHITE);

    // Calção verde
    DrawRectangle(x - 18, y + 10, 36, 18, DARKGREEN);

    // Pernas
    DrawLineEx((Vector2){x - 8, y + 28}, (Vector2){x - 12, y + 60}, 5, BEIGE);
    DrawLineEx((Vector2){x + 8, y + 28}, (Vector2){x + 12, y + 60}, 5, BEIGE);

    if (comemorando) {
        // Braços abertos para cima e para os lados na comemoração "SIUUU!"
        DrawLineEx((Vector2){x - 18, y - 28}, (Vector2){x - 45, y - 55}, 5, BEIGE);
        DrawLineEx((Vector2){x + 18, y - 28}, (Vector2){x + 45, y - 55}, 5, BEIGE);
    } else {
        // Braço segurando a vara de pesca inclinada em direção ao rio
        DrawLineEx((Vector2){x + 18, y - 25}, (Vector2){x + 42, y - 20}, 5, BEIGE);
        DrawLineEx((Vector2){x + 35, y - 15}, (Vector2){x + 160, y - 85}, 4, BROWN);
        // Linha vertical descendo da ponta da vara até a água
        DrawLineEx((Vector2){x + 160, y - 85}, (Vector2){x + 160, y + 85}, 2, DARKGRAY);
    }
}

/**
 * @brief Escreve um texto quebrando a linha sempre que passar da largura maxima.
 *
 * O DrawText da raylib nao quebra linha sozinho, entao o texto e montado
 * palavra por palavra e desce uma linha quando nao cabe mais.
 *
 * @param texto Texto a escrever (ate 255 letras).
 * @param x Posicao horizontal do inicio do texto.
 * @param y Posicao vertical da primeira linha.
 * @param larguraMax Largura maxima de cada linha em pixels.
 * @param tamanhoFonte Tamanho da letra.
 * @param cor Cor do texto.
 */
static void DesenharTextoQuebrado(const char *texto, int x, int y, int larguraMax, int tamanhoFonte, Color cor) {
    char copia[256];
    strncpy(copia, texto, sizeof(copia) - 1); // Faz uma cópia porque strtok modifica a string original
    copia[sizeof(copia) - 1] = '\0';

    char linha[256] = "";
    char *palavra = strtok(copia, " "); // Separa a primeira palavra pelo espaço
    int linhaY = y;

    while (palavra != NULL) {
        char teste[512]; // cabe linha (256) + espaco + palavra (256), assim o snprintf nunca corta
        // Monta uma string temporária testando como fica a linha atual + a próxima palavra
        if (strlen(linha) == 0) snprintf(teste, sizeof(teste), "%s", palavra);
        else snprintf(teste, sizeof(teste), "%s %s", linha, palavra);

        // Se ultrapassar o limite horizontal e já houver texto na linha, desenha e desce o Y
        if (MeasureText(teste, tamanhoFonte) > larguraMax && strlen(linha) > 0) {
            DrawText(linha, x, linhaY, tamanhoFonte, cor);
            linhaY += tamanhoFonte + 6; // Avança para a próxima linha (altura da fonte + espaçamento)
            snprintf(linha, sizeof(linha), "%s", palavra); // Começa a nova linha com a palavra atual
        } else {
            strcpy(linha, teste); // Cabe na mesma linha, então apenas acumula
        }
        palavra = strtok(NULL, " "); // Pega a próxima palavra
    }

    // Desenha o que restou na última linha
    if (strlen(linha) > 0) {
        DrawText(linha, x, linhaY, tamanhoFonte, cor);
    }
}

// ============================================================================
// SISTEMA DE AJUDA (no tema futebol + pescaria, inspirado no Show do Milhao)
// ============================================================================
// - Soltar o peixe (3x por partida): devolve o peixe ao rio e pesca outra pergunta do mesmo nivel (o "pular")
// - Chamar o VAR (1x): anula 2 alternativas erradas (as "cartas")
// - Ouvir a torcida (1x): mostra a porcentagem de votos em cada alternativa (os "universitarios")

/**
 * @brief Ajuda "Chamar o VAR": anula 2 alternativas erradas escolhidas ao acaso.
 * @param anulada Vetor das 4 alternativas; as anuladas viram true.
 * @param certa Indice (0 a 3) da alternativa certa, que nunca e anulada.
 */
static void ChamarVAR(bool anulada[4], int certa) {
    int anuladas = 0;
    while (anuladas < 2) {
        int i = rand() % 4;
        if (i != certa && !anulada[i]) {
            anulada[i] = true;
            anuladas++;
        }
    }
}

/**
 * @brief Ajuda "Ouvir a torcida": sorteia a porcentagem de votos de cada alternativa.
 *
 * A certa leva de 40% a 70% e o resto e dividido ao acaso entre as erradas
 * que nao foram anuladas. Por isso a torcida as vezes erra! O total e sempre 100%.
 *
 * @param votos Vetor onde ficam as porcentagens das 4 alternativas.
 * @param anulada Alternativas anuladas pelo VAR (recebem 0%).
 * @param certa Indice (0 a 3) da alternativa certa.
 */
static void OuvirTorcida(int votos[4], const bool anulada[4], int certa) {
    int resto = 100 - (40 + rand() % 31); // votos que sobram para as alternativas erradas
    int outras[3], n = 0;

    for (int i = 0; i < 4; i++) {
        votos[i] = 0;
        if (i != certa && !anulada[i]) outras[n++] = i;
    }
    if (n == 0) resto = 0; // sem erradas sobrando, a certa leva tudo
    votos[certa] = 100 - resto;

    for (int k = 0; k < n; k++) {
        int parte = (k == n - 1) ? resto : rand() % (resto + 1); // a ultima leva o que sobrou: total = 100%
        votos[outras[k]] = parte;
        resto -= parte;
    }
}

/**
 * @brief Tira os efeitos das ajudas (VAR, torcida e o segredo 67) quando chega uma pergunta nova.
 * @param anulada Vetor das 4 alternativas anuladas (volta tudo para false).
 * @param mostrarVotos Deixa de mostrar as barras da torcida.
 * @param revelada Deixa de destacar a resposta certa em dourado.
 */
static void LimparAjudas(bool anulada[4], bool *mostrarVotos, bool *revelada) {
    for (int i = 0; i < 4; i++) anulada[i] = false;
    *mostrarVotos = false;
    *revelada = false;
}

// ============================================================================
// FUNÇÃO PRINCIPAL DO MÓDULO GRÁFICO (CHAMADA PELO MAIN.C)
// ============================================================================
// Documentacao (Doxygen) em janela.h. Retorna o premio, que o main.c grava em dados/ranking.txt.
int jogarJanela(const ListaPerguntas *lista) {
    // Verifica se existem perguntas suficientes de cada nível para montar uma partida de 15 questões
    if (!listaSuficiente(lista)) {
        return 0;
    }

    // Inicializa a semente aleatória com o relógio atual do sistema
    srand((unsigned int)time(NULL));

    // Cria um vetor dinâmico zerado (calloc) para marcar quais perguntas já foram sorteadas na partida
    int *usadas = (int *)calloc((size_t)lista->quantidade, sizeof(int));
    if (!usadas) return 0; // Proteção contra falha de alocação de memória

    // Configurações da janela Raylib
    const int largura = 960;
    const int altura = 680;
    InitWindow(largura, altura, "Genio Quiz - CR7 Pescando (Jogo do Milhao)");
    SetTargetFPS(60); // Trava a taxa de atualização em 60 quadros por segundo

    // Variáveis de controle da partida
    int questaoAtual = 0;       // Índice da questão atual (0 a 14)
    int premioAcumulado = 0;    // Prêmio garantido caso o jogador decida parar
    int premioFinal = 0;        // Prêmio final retornado ao encerrar a partida
    int nivelAtual = nivelDaQuestao(questaoAtual); // Descobre o nível (1 a 5) da primeira questão

    // Sorteia a primeira pergunta do nível correspondente e marca como usada
    int idxPergunta = sortearPergunta(lista, usadas, nivelAtual);
    if (idxPergunta >= 0) usadas[idxPergunta] = 1;

    // Variáveis de estado da interface
    FaseRaylib fase = FASE_PESCA; // Toda rodada começa no minigame de pescaria
    float timerSiuuu = 0.0f;      // Cronômetro da animação "SIUUU!"
    char mensagemTopo[128] = "Clique na agua onde o peixe esta escondido!";
    char mensagemFinal[128] = "";
    bool venceuOuParou = false;   // Define se a tela final será verde (vitória/parada) ou vermelha (erro)

    // Estado do sistema de ajuda
    int pulosRestantes = 3;       // Quantas vezes ainda pode soltar o peixe
    bool usouVAR = false;         // VAR e torcida so podem ser usados 1 vez por partida
    bool usouTorcida = false;
    bool anulada[4] = {false};    // Alternativas anuladas pelo VAR na pergunta atual
    int votos[4] = {0};           // Porcentagem da torcida em cada alternativa
    bool mostrarVotos = false;    // Se a torcida ja foi ouvida nesta pergunta
    char mensagemAjuda[128] = ""; // Aviso que aparece acima dos botoes de ajuda

    // Segredo "67" (six seven): digitar 6 e depois 7 na pergunta faz o CR7 soprar a resposta, 1 vez por partida
    bool apertou6 = false;        // Se a ultima tecla do segredo foi o 6
    bool usou67 = false;
    bool revelada = false;        // Se a resposta certa esta destacada em dourado nesta pergunta

    // Configuração inicial da pescaria: quanto maior o nível, mais pontos no rio (3 a 7)
    int qtdPontosRio = 3 + (nivelAtual - 1);
    int peixeEscondido = rand() % qtdPontosRio; // Sorteia em qual bloco o peixe está

    // ------------------------------------------------------------------------
    // LOOP PRINCIPAL DO JOGO (Roda 60 vezes por segundo até fechar a janela)
    // ------------------------------------------------------------------------
    while (!WindowShouldClose()) {
        // Captura a posição atual do cursor do mouse e se houve clique no botão esquerdo neste frame
        Vector2 mouse = GetMousePosition();
        bool clicou = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

        // ====================================================================
        // BLOCO 1: LÓGICA E TRATAMENTO DE ENTRADAS (MOUSE / TECLADO)
        // ====================================================================
        if (fase == FASE_PESCA) {
            // Calcula o posicionamento centralizado dos blocos clicáveis do rio
            int larguraPonto = 95;
            int espaco = 18;
            int larguraTotal = qtdPontosRio * larguraPonto + (qtdPontosRio - 1) * espaco;
            int inicioX = (largura - larguraTotal) / 2;

            for (int i = 0; i < qtdPontosRio; i++) {
                Rectangle recAgua = {(float)(inicioX + i * (larguraPonto + espaco)), 430, (float)larguraPonto, 95};
                // Permite pescar tanto clicando com o mouse quanto apertando as teclas 1..7
                bool clickTeclado = IsKeyPressed(KEY_ONE + i);

                if ((clicou && CheckCollisionPointRec(mouse, recAgua)) || clickTeclado) {
                    if (i == peixeEscondido) {
                        // Acertou onde o peixe estava: inicia a comemoração do CR7
                        timerSiuuu = 1.4f;
                        fase = FASE_SIUUU;
                    } else {
                        // Errou o ponto: o peixe foge para outra posição aleatória no rio
                        snprintf(mensagemTopo, sizeof(mensagemTopo), "O peixe escapou! Tente pescar novamente.");
                        peixeEscondido = rand() % qtdPontosRio;
                    }
                }
            }
        }
        else if (fase == FASE_SIUUU) {
            // Decrementa o cronômetro pelo tempo decorrido do último frame (GetFrameTime)
            timerSiuuu -= GetFrameTime();
            // Avança para a pergunta quando o tempo acabar ou se o jogador clicar / apertar ENTER
            if (timerSiuuu <= 0.0f || clicou || IsKeyPressed(KEY_ENTER)) {
                fase = FASE_PERGUNTA;
            }
        }
        else if (fase == FASE_PERGUNTA && idxPergunta >= 0) {
            Pergunta p = lista->itens[idxPergunta];

            // Verifica clique no botão "PARAR" ou tecla '0' para encerrar levando o prêmio acumulado
            Rectangle btnParar = {710, 595, 210, 45};
            if ((clicou && CheckCollisionPointRec(mouse, btnParar)) || IsKeyPressed(KEY_ZERO)) {
                premioFinal = premioAcumulado;
                venceuOuParou = true;
                snprintf(mensagemFinal, sizeof(mensagemFinal), "VOCE PAROU E LEVOU R$ %d!", premioFinal);
                fase = FASE_RESULTADO;
            }

            // ---------------- SISTEMA DE AJUDA (botoes ou teclas P, V e T) ----------------
            Rectangle btnPular   = {70,  595, 190, 45};
            Rectangle btnVAR     = {275, 595, 190, 45};
            Rectangle btnTorcida = {480, 595, 215, 45};
            int certa = toupper((unsigned char)p.correta) - 'A'; // indice 0..3 da alternativa certa

            if ((clicou && CheckCollisionPointRec(mouse, btnPular)) || IsKeyPressed(KEY_P)) {
                if (pulosRestantes == 0) {
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "Voce ja soltou os 3 peixes da partida!");
                } else if (!existePerguntaLivre(lista, usadas, nivelAtual)) {
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "Nao sobrou outro %s no rio!", NOMES_PEIXES[nivelAtual - 1]);
                } else {
                    // Devolve o peixe ao rio: sorteia outra pergunta do mesmo nivel e volta para a pescaria
                    pulosRestantes--;
                    idxPergunta = sortearPergunta(lista, usadas, nivelAtual);
                    usadas[idxPergunta] = 1;
                    LimparAjudas(anulada, &mostrarVotos, &revelada);
                    mensagemAjuda[0] = '\0';
                    peixeEscondido = rand() % qtdPontosRio;
                    snprintf(mensagemTopo, sizeof(mensagemTopo), "Voce soltou o peixe! Pesque outra pergunta.");
                    fase = FASE_PESCA;
                }
            }
            if (fase == FASE_PERGUNTA && ((clicou && CheckCollisionPointRec(mouse, btnVAR)) || IsKeyPressed(KEY_V))) {
                if (usouVAR) {
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "O VAR ja foi chamado nesta partida!");
                } else {
                    usouVAR = true;
                    ChamarVAR(anulada, certa);
                    if (mostrarVotos) OuvirTorcida(votos, anulada, certa); // a torcida nao vota em alternativa anulada
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "VAR: duas alternativas erradas foram anuladas!");
                }
            }
            if (fase == FASE_PERGUNTA && ((clicou && CheckCollisionPointRec(mouse, btnTorcida)) || IsKeyPressed(KEY_T))) {
                if (usouTorcida) {
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "A torcida ja foi ouvida nesta partida!");
                } else {
                    usouTorcida = true;
                    OuvirTorcida(votos, anulada, certa);
                    mostrarVotos = true;
                    snprintf(mensagemAjuda, sizeof(mensagemAjuda), "A torcida votou! Mas cuidado: ela pode errar...");
                }
            }

            // ---------------- SEGREDO 67: tecla 6 e logo depois tecla 7 ----------------
            if (fase == FASE_PERGUNTA) {
                bool tecla6 = IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6);
                bool tecla7 = IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7);

                if (tecla7 && apertou6) {
                    if (usou67) {
                        snprintf(mensagemAjuda, sizeof(mensagemAjuda), "O 6-7 so funciona uma vez por partida!");
                    } else {
                        usou67 = true;
                        revelada = true;
                        snprintf(mensagemAjuda, sizeof(mensagemAjuda), "SIX SEVEN! O CR7 soprou a resposta certa (em dourado)!");
                    }
                }
                if (tecla6) apertou6 = true;
                else if (GetKeyPressed() != 0) apertou6 = false; // qualquer outra tecla quebra a sequencia
            }

            // Define os retângulos dos 4 botões de resposta em grade 2x2 (estilo Gênio Quiz)
            Rectangle botoes[4] = {
                {70,  265, 390, 130}, // Botão A (superior esquerdo)
                {500, 265, 390, 130}, // Botão B (superior direito)
                {70,  425, 390, 130}, // Botão C (inferior esquerdo)
                {500, 425, 390, 130}  // Botão D (inferior direito)
            };

            int escolha = -1; // -1 indica que nenhuma alternativa foi escolhida neste frame

            // Checa clique do mouse em cada um dos 4 botões
            for (int i = 0; i < 4; i++) {
                if (clicou && CheckCollisionPointRec(mouse, botoes[i])) {
                    escolha = i;
                }
            }

            // Atalhos de teclado (A, B, C ou D)
            if (IsKeyPressed(KEY_A)) escolha = 0;
            if (IsKeyPressed(KEY_B)) escolha = 1;
            if (IsKeyPressed(KEY_C)) escolha = 2;
            if (IsKeyPressed(KEY_D)) escolha = 3;

            // Se o jogador escolheu uma alternativa (alternativa anulada pelo VAR nao vale;
            // e se ja parou ou soltou o peixe neste quadro, a resposta e ignorada):
            if (escolha != -1 && fase == FASE_PERGUNTA && !anulada[escolha]) {
                char letraEscolhida = (char)('A' + escolha); // Converte índice 0..3 em 'A'..'D'

                if (toupper((unsigned char)p.correta) == letraEscolhida) {
                    // RESPOSTA CERTA: atualiza prêmio acumulado e avança para a próxima questão
                    premioAcumulado = PREMIOS[questaoAtual];
                    questaoAtual++;

                    if (questaoAtual >= NUM_QUESTOES) {
                        // Completou todas as 15 questões: ganhou o prêmio máximo de R$ 1.000.000
                        premioFinal = premioAcumulado;
                        venceuOuParou = true;
                        snprintf(mensagemFinal, sizeof(mensagemFinal), "GOOOOL! VOCE GANHOU R$ 1.000.000!");
                        fase = FASE_RESULTADO;
                    } else {
                        // Prepara a próxima rodada: atualiza nível, sorteia nova pergunta e reinicia a pesca
                        nivelAtual = nivelDaQuestao(questaoAtual);
                        idxPergunta = sortearPergunta(lista, usadas, nivelAtual);
                        if (idxPergunta >= 0) usadas[idxPergunta] = 1;
                        LimparAjudas(anulada, &mostrarVotos, &revelada);
                        mensagemAjuda[0] = '\0';

                        qtdPontosRio = 3 + (nivelAtual - 1);
                        peixeEscondido = rand() % qtdPontosRio;
                        snprintf(mensagemTopo, sizeof(mensagemTopo), "Clique na agua onde o peixe esta escondido!");
                        fase = FASE_PESCA;
                    }
                } else {
                    // RESPOSTA ERRADA: aplica a regra dos marcos de segurança (questões 5 e 10)
                    premioFinal = premioAoErrar(questaoAtual);
                    venceuOuParou = false;
                    snprintf(mensagemFinal, sizeof(mensagemFinal), "A resposta correta era a letra %c!", toupper((unsigned char)p.correta));
                    fase = FASE_RESULTADO;
                }
            }
        }
        else if (fase == FASE_RESULTADO) {
            // Na tela final, clicar em "SALVAR E SAIR" ou apertar ENTER encerra o loop gráfico
            Rectangle btnSair = {330, 450, 300, 75};
            if ((clicou && CheckCollisionPointRec(mouse, btnSair)) || IsKeyPressed(KEY_ENTER)) {
                break;
            }
        }

        // ====================================================================
        // BLOCO 2: RENDERIZAÇÃO GRÁFICA (DESENHO NA TELA)
        // ====================================================================
        BeginDrawing();
        ClearBackground(RAYWHITE); // Fundo branco clássico do Gênio Quiz

        // Moldura externa preta grossa em volta de toda a tela
        DrawRectangleLinesEx((Rectangle){12, 12, largura - 24, altura - 24}, 6, BLACK);

        if (fase == FASE_PESCA) {
            // Círculo com o número da questão no canto superior esquerdo
            DrawCircle(65, 65, 32, WHITE);
            DrawCircleLines(65, 65, 32, BLACK);
            DrawText(TextFormat("%d", questaoAtual + 1), questaoAtual + 1 >= 10 ? 50 : 58, 50, 32, BLACK);

            // Informações do prêmio, peixe atual e instrução
            DrawText(TextFormat("VALENDO: R$ %d", PREMIOS[questaoAtual]), 120, 42, 28, DARKGREEN);
            DrawText(TextFormat("Peixe no anzol: %s (Nivel %d)", NOMES_PEIXES[nivelAtual - 1], nivelAtual), 120, 76, 22, DARKBLUE);
            DrawText(mensagemTopo, 120, 125, 24, BLACK);

            // Desenha o CR7 na margem pescando
            DesenharCR7(140, 310, false);

            // Desenha o retângulo azul do rio
            DrawRectangle(40, 385, largura - 80, 185, SKYBLUE);
            DrawRectangleLinesEx((Rectangle){40, 385, largura - 80, 185}, 4, BLACK);

            // Desenha os blocos clicáveis dentro do rio com efeito hover (muda de cor ao passar o mouse)
            int larguraPonto = 95;
            int espaco = 18;
            int larguraTotal = qtdPontosRio * larguraPonto + (qtdPontosRio - 1) * espaco;
            int inicioX = (largura - larguraTotal) / 2;

            for (int i = 0; i < qtdPontosRio; i++) {
                Rectangle recAgua = {(float)(inicioX + i * (larguraPonto + espaco)), 430, (float)larguraPonto, 95};
                Color corPonto = CheckCollisionPointRec(mouse, recAgua) ? BLUE : DARKBLUE;
                DesenharCaixaGenio(recAgua, corPonto, BLACK, 3);
                DrawText(TextFormat("%d", i + 1), (int)recAgua.x + 38, (int)recAgua.y + 32, 32, WHITE);
            }

            DrawText("Dica: Clique em um bloco do rio ou aperte o numero no teclado", 195, 605, 20, DARKGRAY);
        }
        else if (fase == FASE_SIUUU) {
            // Tela de comemoração ao fisgar o peixe
            DesenharCR7(largura / 2, 290, true);
            DrawText("FISGOU A PERGUNTA!", 275, 110, 36, DARKGREEN);
            DrawText("SIUUUUUUUUUU!", 230, 410, 64, RED);
            DrawText(TextFormat("Um %s trouxe a pergunta %d!", NOMES_PEIXES[nivelAtual - 1], questaoAtual + 1), 270, 500, 26, BLACK);
        }
        else if (fase == FASE_PERGUNTA && idxPergunta >= 0) {
            Pergunta p = lista->itens[idxPergunta];

            // Bolinha com o número da questão no canto superior esquerdo (estilo Gênio Quiz)
            DrawCircle(65, 65, 32, WHITE);
            DrawCircleLines(65, 65, 32, BLACK);
            DrawText(TextFormat("%d", questaoAtual + 1), questaoAtual + 1 >= 10 ? 50 : 58, 50, 32, BLACK);

            // Exibe o valor da pergunta atual e quanto o jogador garante se errar (marco de segurança)
            DrawText(TextFormat("Valendo: R$ %d", PREMIOS[questaoAtual]), 670, 35, 24, DARKGREEN);
            DrawText(TextFormat("Seguro: R$ %d", premioAoErrar(questaoAtual)), 670, 65, 20, DARKGRAY);

            // Desenha o enunciado da pergunta com quebra automática de linha
            DesenharTextoQuebrado(p.enunciado, 120, 52, 530, 26, BLACK);

            // Desenha os 4 botões de alternativas em grade 2x2
            Rectangle botoes[4] = {
                {70,  265, 390, 130},
                {500, 265, 390, 130},
                {70,  425, 390, 130},
                {500, 425, 390, 130}
            };

            for (int i = 0; i < 4; i++) {
                int bx = (int)botoes[i].x, by = (int)botoes[i].y;

                if (anulada[i]) {
                    // Alternativa anulada pelo VAR: botão cinza, com cartão vermelho
                    DesenharCaixaGenio(botoes[i], GRAY, BLACK, 4);
                    DrawRectangle(bx + 18, by + 30, 30, 42, RED); // cartão vermelho
                    DrawRectangleLines(bx + 18, by + 30, 30, 42, BLACK);
                    DrawText(TextFormat("%c) ANULADA PELO VAR", 'A' + i), bx + 62, by + 42, 22, WHITE);
                    continue;
                }

                // Destaca o botão em cinza claro quando o cursor passa por cima (hover)
                Color fundo = CheckCollisionPointRec(mouse, botoes[i]) ? LIGHTGRAY : WHITE;
                if (revelada && 'A' + i == toupper((unsigned char)p.correta)) fundo = GOLD; // segredo 67
                DesenharCaixaGenio(botoes[i], fundo, BLACK, 4);

                char textoBotao[120];
                snprintf(textoBotao, sizeof(textoBotao), "%c) %s", 'A' + i, p.alternativas[i]);
                DesenharTextoQuebrado(textoBotao, bx + 18, by + 25, 355, 22, BLACK);

                if (mostrarVotos) {
                    // Barra da torcida no pé do botão (largura proporcional aos votos)
                    DrawRectangle(bx + 18, by + 100, 290, 16, RAYWHITE);
                    DrawRectangle(bx + 18, by + 100, 290 * votos[i] / 100, 16, GOLD);
                    DrawRectangleLines(bx + 18, by + 100, 290, 16, BLACK);
                    DrawText(TextFormat("%d%%", votos[i]), bx + 318, by + 98, 20, BLACK);
                }
            }

            // Botões de ajuda no rodapé (ficam apagados quando não podem mais ser usados)
            Rectangle btnPular   = {70,  595, 190, 45};
            Rectangle btnVAR     = {275, 595, 190, 45};
            Rectangle btnTorcida = {480, 595, 215, 45};
            Rectangle btnsAjuda[3] = {btnPular, btnVAR, btnTorcida};
            bool ajudaUsada[3] = {pulosRestantes == 0, usouVAR, usouTorcida};
            const char *textoAjuda[3] = {
                TextFormat("[P] SOLTAR PEIXE %d", pulosRestantes),
                "[V] CHAMAR O VAR",
                "[T] OUVIR A TORCIDA"
            };
            for (int i = 0; i < 3; i++) {
                Color fundo = ajudaUsada[i] ? LIGHTGRAY : (CheckCollisionPointRec(mouse, btnsAjuda[i]) ? SKYBLUE : WHITE);
                DesenharCaixaGenio(btnsAjuda[i], fundo, BLACK, 3);
                DrawText(textoAjuda[i], (int)btnsAjuda[i].x + 12, 608, 18, ajudaUsada[i] ? GRAY : BLACK);
            }

            // Aviso da última ajuda usada (ou a lista de ajudas, se nenhuma foi usada ainda)
            DrawText(mensagemAjuda[0] ? mensagemAjuda : "AJUDAS: P = soltar o peixe (pula)  |  V = VAR  |  T = torcida",
                     70, 568, 18, mensagemAjuda[0] ? DARKBLUE : DARKGRAY);

            Rectangle btnParar = {710, 595, 210, 45};
            Color corParar = CheckCollisionPointRec(mouse, btnParar) ? GOLD : WHITE;
            DesenharCaixaGenio(btnParar, corParar, BLACK, 3);
            DrawText(TextFormat("PARAR (R$ %d)", premioAcumulado), 725, 608, 18, BLACK);
        }
        else if (fase == FASE_RESULTADO) {
            // Tela final de encerramento da partida
            if (venceuOuParou) {
                DrawText("FIM DE PARTIDA!", 260, 140, 54, DARKGREEN);
                DrawText(mensagemFinal, 210, 230, 32, BLACK);
            } else {
                // Tela clássica de erro do Gênio Quiz
                DrawText("ERROU!", 340, 110, 90, RED);
                DrawText(mensagemFinal, 230, 230, 30, BLACK);
                DrawText(TextFormat("Voce leva para casa: R$ %d", premioFinal), 250, 290, 28, DARKGRAY);
            }

            // Mostra até qual questão o jogador chegou
            DrawText(TextFormat("Questoes acertadas: %d de 15", questaoAtual), 300, 360, 24, DARKBLUE);

            // Botão para salvar no ranking e fechar a janela
            Rectangle btnSair = {330, 450, 300, 75};
            Color corSair = CheckCollisionPointRec(mouse, btnSair) ? GOLD : WHITE;
            DesenharCaixaGenio(btnSair, corSair, BLACK, 4);
            DrawText("SALVAR E SAIR", 375, 475, 28, BLACK);
            DrawText("(ou aperte ENTER)", 390, 545, 20, GRAY);
        }

        EndDrawing();
    }

    // ------------------------------------------------------------------------
    // ENCERRAMENTO: libera memória e fecha a janela
    // ------------------------------------------------------------------------
    // Se o jogador fechou a janela no X no meio da partida, leva o que já tinha acumulado
    if (fase != FASE_RESULTADO) {
        premioFinal = premioAcumulado;
    }

    free(usadas);   // Libera o vetor de perguntas sorteadas
    CloseWindow();  // Fecha a janela e libera os recursos da Raylib

    return premioFinal; // Valor gravado pelo main.c em dados/ranking.txt
}
