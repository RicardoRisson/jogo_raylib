#include "interface.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

// --- FUNÇÕES DE MANIPULAÇÃO DO PLACAR ---

void inicializar_placar_vazio(void) {
    FILE *f = fopen("placar.bin", "rb");
    if (!f) {
        // Arquivo não existe, cria um arquivo binário limpo (0 bytes)
        f = fopen("placar.bin", "wb");
    }
    if (f) fclose(f);
}

int carregar_placar(TIPO_PLACAR placar[]) {
    FILE *f = fopen("placar.bin", "rb");
    int lidos = 0;
    int i = 0;
    
    if (f) {
        lidos = fread(placar, sizeof(TIPO_PLACAR), 10, f);
        fclose(f);
    }
    
    // Preenche os slots vazios na memória com um tempo inatingível (999999) 
    for (i = lidos; i < 10; i++) {
        placar[i].time = 999999;
        placar[i].nome[0] = '\0';
    }
    
    return lidos; 
}

void salvar_placar(TIPO_PLACAR placar[]) {
    FILE *f = fopen("placar.bin", "wb");
    if (f) {
        int contar_validos = 0;
        int i = 0;
        for (i = 0; i < 10; i++) {
            if (placar[i].time != 999999) contar_validos++;
        }
        fwrite(placar, sizeof(TIPO_PLACAR), contar_validos, f);
        fclose(f);
    }
}

bool verificar_se_entra_no_ranking(int tempo_jogador) {
    TIPO_PLACAR placar[10];
    int lidos = carregar_placar(placar);
    
    // Se o ranking tiver menos de 10 pessoas ou o tempo for melhor (menor) que o último
    if (lidos < 10) return true;
    return (tempo_jogador < placar[9].time);
}

void inserir_no_ranking(const char *nome, int tempo_jogador) {
    TIPO_PLACAR placar[10];
    int i = 0, j = 0;
    carregar_placar(placar);

    // Substitui o pior tempo (índice 9) pelo novo jogador
    strncpy(placar[9].nome, nome, 19);
    placar[9].nome[19] = '\0';
    placar[9].time = tempo_jogador;

    // Bubble Sort: Ordena em ordem CRESCENTE de tempo (menor tempo = melhor colocação)
    for (i = 0; i < 9; i++) {
        for (j = 0; j < 9 - i; j++) {
            if (placar[j].time > placar[j + 1].time) {
                TIPO_PLACAR temp = placar[j];
                placar[j] = placar[j + 1];
                placar[j + 1] = temp;
            }
        }
    }
    
    salvar_placar(placar);
}

// --- FUNÇÕES DE INTERFACE E RENDERIZAÇÃO ---

void AtualizarBotoesInterface(GameState *estado_atual, int *opcao_menu_pausa) {
    if (*estado_atual == STATE_JOGANDO) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            *estado_atual = STATE_PAUSADO;
            *opcao_menu_pausa = 0; 
        }
    } 
    else if (*estado_atual == STATE_PAUSADO) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            *estado_atual = STATE_JOGANDO;
        }
    }
}

void DesenharMenuPrincipal(int opcao_selecionada) {
    int i = 0;
    const char *opcoes[3] = {"NOVO JOGO", "RANKING", "SAIR"};
    
    ClearBackground(RAYWHITE);
    DrawText("JOGO NINJA EM RAYLIB", 1500 / 2 - MeasureText("JOGO NINJA EM RAYLIB", 50) / 2, 200, 50, DARKGRAY);
    
    for (i = 0; i < 3; i++) {
        Color cor = (opcao_selecionada == i) ? RED : BLACK;
        DrawText(opcoes[i], 1500 / 2 - MeasureText(opcoes[i], 30) / 2, 400 + (i * 80), 30, cor);
    }
    
    DrawText("Use W/S ou Setas para navegar e ENTER para selecionar", 1500 / 2 - MeasureText("Use W/S ou Setas para navegar e ENTER para selecionar", 20) / 2, 800, 20, GRAY);
}

void DesenharMenuPausa(int opcao_selecionada) {
    int i = 0;
    const char *opcoes[3] = {"CONTINUAR", "VOLTAR AO MENU PRINCIPAL", "SAIR"};
    
    DrawRectangle(0, 0, 1500, 1000, (Color){ 0, 0, 0, 150 });
    DrawText("PAUSADO", 1500 / 2 - MeasureText("PAUSADO", 50) / 2, 250, 50, WHITE);
    
    for (i = 0; i < 3; i++) {
        Color cor = (opcao_selecionada == i) ? YELLOW : LIGHTGRAY;
        DrawText(opcoes[i], 1500 / 2 - MeasureText(opcoes[i], 30) / 2, 450 + (i * 80), 30, cor);
    }
}

void DesenharRanking(void) {
    TIPO_PLACAR placar[10];
    int lidos = carregar_placar(placar);
    int i = 0;

    ClearBackground(RAYWHITE);
    DrawText("TOP 10 - MELHORES TEMPOS", 1500 / 2 - MeasureText("TOP 10 - MELHORES TEMPOS", 40) / 2, 100, 40, GOLD);

    if (lidos == 0) {
        DrawText("Nenhum recorde registrado ainda.", 1500 / 2 - MeasureText("Nenhum recorde registrado ainda.", 25) / 2, 400, 25, GRAY);
    } else {
        for (i = 0; i < lidos; i++) {
            char buffer[50];
            sprintf(buffer, "%d. %-15s - %d seg", i + 1, placar[i].nome, placar[i].time);
            DrawText(buffer, 1500 / 2 - 200, 220 + (i * 45), 25, BLACK);
        }
    }

    DrawText("Pressione ESC para voltar", 1500 / 2 - MeasureText("Pressione ESC para voltar", 20) / 2, 850, 20, DARKGRAY);
}

void DesenharTelaSalvarRecorde(const char *nome_atual, int tempo_final) {
    char txt_tempo[50];
    ClearBackground(RAYWHITE);
    DrawText("FIM DE JOGO! NOVO RECORDE!", 1500 / 2 - MeasureText("FIM DE JOGO! NOVO RECORDE!", 40) / 2, 200, 40, GREEN);
    
    sprintf(txt_tempo, "Seu tempo total: %d segundos", tempo_final);
    DrawText(txt_tempo, 1500 / 2 - MeasureText(txt_tempo, 25) / 2, 280, 25, BLACK);

    DrawText("Digite seu nome e pressione ENTER:", 1500 / 2 - MeasureText("Digite seu nome e pressione ENTER:", 20) / 2, 450, 20, GRAY);
    
    // Caixa de texto centralizada
    DrawRectangle(1500 / 2 - 150, 500, 300, 50, LIGHTGRAY);
    DrawRectangleLines(1500 / 2 - 150, 500, 300, 50, DARKGRAY);
    DrawText(nome_atual, 1500 / 2 - 140, 510, 30, BLACK);
}