#include "interface.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>

// --- FUNÇÕES DE MANIPULAÇÃO DO PLACAR ---

void inicializar_placar_vazio(void) {
    FILE *f = fopen("placar.bin", "rb"); // tenta abrir o arquivo de placar existente
    if (!f) { // se nao existe ainda
        f = fopen("placar.bin", "wb"); // cria um arquivo binario vazio
    }
    if (f) fclose(f); // fecha o arquivo
}

int carregar_placar(TIPO_PLACAR placar[]) {
    FILE *f = fopen("placar.bin", "rb"); // abre o placar pra leitura
    int lidos = 0;
    int i = 0;

    if (f) {
        lidos = fread(placar, sizeof(TIPO_PLACAR), 10, f); // le ate 10 entradas do arquivo
        fclose(f);
    }

    for (i = lidos; i < 10; i++) { // preenche as entradas vazias com valores invalidos
        placar[i].time = INT_MAX; // INT_MAX = "sem tempo", sempre perde na comparacao
        placar[i].nome[0] = '\0'; // nome vazio
    }

    return lidos; // retorna quantas entradas foram lidas de verdade
}

void salvar_placar(TIPO_PLACAR placar[]) {
    FILE *f = fopen("placar.bin", "wb"); // abre o placar pra escrita, sobrescreve tudo
    if (f) {
        int contar_validos = 0;
        int i = 0;
        for (i = 0; i < 10; i++) {
            if (placar[i].time != INT_MAX) contar_validos++; // conta so as entradas reais
        }
        fwrite(placar, sizeof(TIPO_PLACAR), contar_validos, f); // salva apenas as entradas validas
        fclose(f);
    }
}

bool verificar_se_entra_no_ranking(int tempo_jogador) {
    TIPO_PLACAR placar[10];
    int lidos = carregar_placar(placar);

    if (lidos < 10) return true; // ranking nao cheio, qualquer tempo entra
    return (tempo_jogador < placar[9].time); // tempo menor que o pior do ranking? entra
}

void inserir_no_ranking(const char *nome, int tempo_jogador) {
    TIPO_PLACAR placar[10];
    int i = 0, j = 0;
    carregar_placar(placar);

    strncpy(placar[9].nome, nome, 19); // coloca o novo jogador na ultima posicao
    placar[9].nome[19] = '\0'; // garante que o nome termina com \0
    placar[9].time = tempo_jogador;

    for (i = 0; i < 9; i++) { // bubble sort pra ordenar do menor tempo pro maior
        for (j = 0; j < 9 - i; j++) {
            if (placar[j].time > placar[j + 1].time) { // tempo maior = posicao pior, troca
                TIPO_PLACAR temp  = placar[j];
                placar[j]         = placar[j + 1];
                placar[j + 1]     = temp;
            }
        }
    }

    salvar_placar(placar); // salva o ranking ja ordenado
}

// --- INTERFACE E RENDERIZACAO ---

void AtualizarBotoesInterface(GameState *estado_atual, int *opcao_menu_pausa) {
    if (*estado_atual == STATE_JOGANDO) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            *estado_atual = STATE_PAUSADO; // pausa o jogo
            *opcao_menu_pausa = 0; // reseta a opcao selecionada no menu de pausa
        }
    }
    else if (*estado_atual == STATE_PAUSADO) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            *estado_atual = STATE_JOGANDO; // volta a jogar
        }
    }
}

void DesenharMenuPrincipal(int opcao_selecionada) {
    int i = 0;
    const char *opcoes[3] = {"NOVO JOGO", "RANKING", "SAIR"};

    ClearBackground(RAYWHITE);
    DrawText("Ninja contra os Ninjas verdes", 1500 / 2 - MeasureText("Ninja contra os Ninjas verdes", 50) / 2, 200, 50, DARKGRAY); // titulo centralizado

    for (i = 0; i < 3; i++) {
        Color cor = (opcao_selecionada == i) ? RED : BLACK; // opcao selecionada fica vermelha
        DrawText(opcoes[i], 1500 / 2 - MeasureText(opcoes[i], 30) / 2, 400 + (i * 80), 30, cor); // cada opcao 80px abaixo da anterior
    }

    DrawText("Use W/S ou Setas para navegar e ENTER para selecionar", 1500 / 2 - MeasureText("Use W/S ou Setas para navegar e ENTER para selecionar", 20) / 2, 800, 20, GRAY); // instrucoes no rodape
}

void DesenharMenuPausa(int opcao_selecionada) {
    int i = 0;
    const char *opcoes[3] = {"CONTINUAR", "VOLTAR AO MENU PRINCIPAL", "SAIR"};

    DrawRectangle(0, 0, 1500, 1000, (Color){ 0, 0, 0, 150 }); // overlay escuro semitransparente sobre o jogo
    DrawText("PAUSADO", 1500 / 2 - MeasureText("PAUSADO", 50) / 2, 250, 50, WHITE); // titulo centralizado

    for (i = 0; i < 3; i++) {
        Color cor = (opcao_selecionada == i) ? YELLOW : LIGHTGRAY; // opcao selecionada fica amarela
        DrawText(opcoes[i], 1500 / 2 - MeasureText(opcoes[i], 30) / 2, 450 + (i * 80), 30, cor); // cada opcao 80px abaixo da anterior
    }
}

void DesenharRanking(void) {
    TIPO_PLACAR placar[10];
    int lidos = carregar_placar(placar); // carrega o placar do disco
    int i = 0;

    ClearBackground(RAYWHITE);
    DrawText("TOP 10 - MELHORES TEMPOS", 1500 / 2 - MeasureText("TOP 10 - MELHORES TEMPOS", 40) / 2, 100, 40, GOLD); // titulo centralizado

    if (lidos == 0) {
        DrawText("Nenhum recorde registrado ainda.", 1500 / 2 - MeasureText("Nenhum recorde registrado ainda.", 25) / 2, 400, 25, GRAY); // aviso se ranking vazio
    } else {
        for (i = 0; i < lidos; i++) {
            char buffer[50];
            sprintf(buffer, "%d. %-15s - %d seg", i + 1, placar[i].nome, placar[i].time); // formata "1. NOME            - 42 seg"
            DrawText(buffer, 1500 / 2 - 200, 220 + (i * 45), 25, BLACK); // cada entrada 45px abaixo da anterior
        }
    }

    DrawText("Pressione ESC para voltar", 1500 / 2 - MeasureText("Pressione ESC para voltar", 20) / 2, 850, 20, DARKGRAY);
}

void DesenharTelaSalvarRecorde(const char *nome_atual, int tempo_final) {
    char txt_tempo[50];
    ClearBackground(RAYWHITE);
    DrawText("FIM DE JOGO! NOVO RECORDE!", 1500 / 2 - MeasureText("FIM DE JOGO! NOVO RECORDE!", 40) / 2, 200, 40, GREEN);

    sprintf(txt_tempo, "Seu tempo total: %d segundos", tempo_final); // formata o tempo pra exibir
    DrawText(txt_tempo, 1500 / 2 - MeasureText(txt_tempo, 25) / 2, 280, 25, BLACK);

    DrawText("Digite seu nome e pressione ENTER:", 1500 / 2 - MeasureText("Digite seu nome e pressione ENTER:", 20) / 2, 450, 20, GRAY);

    DrawRectangle(1500 / 2 - 150, 500, 300, 50, LIGHTGRAY);      // fundo da caixa de texto
    DrawRectangleLines(1500 / 2 - 150, 500, 300, 50, DARKGRAY);  // borda da caixa de texto
    DrawText(nome_atual, 1500 / 2 - 140, 510, 30, BLACK);         // texto digitado pelo jogador
}