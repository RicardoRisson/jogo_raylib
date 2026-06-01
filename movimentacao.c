#include "movimentacao.h"
#include <raylib.h>

// --- Constantes de Física ---
static const float GRAVIDADE     = 0.5f;
static const float FORCA_PULO    = -11.0f;
static const float LIMITE_QUEDA  = 15.0f;

// --- Estado do Jogador ---
static float velocidade_y = 0.0f;
static bool esta_no_chao  = false;

static Rectangle mover_e_colidir_x(Rectangle jogador, float velocidade_x, Rectangle plataformas[], int quantidade_plataformas) {
    if (velocidade_x == 0.0f) return jogador;

    jogador.x += velocidade_x;

    for (int i = 0; i < quantidade_plataformas; i++) {
        if (CheckCollisionRecs(jogador, plataformas[i])) {
            if (velocidade_x > 0.0f) {
                jogador.x = plataformas[i].x - jogador.width;
            } 
            else if (velocidade_x < 0.0f) {
                jogador.x = plataformas[i].x + plataformas[i].width;
            }
        }
    }
    return jogador;
}

static Rectangle mover_e_colidir_y(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas) {
    velocidade_y += GRAVIDADE;
    if (velocidade_y > LIMITE_QUEDA) velocidade_y = LIMITE_QUEDA;

    jogador.y += velocidade_y;
    esta_no_chao = false;

    for (int i = 0; i < quantidade_plataformas; i++) {
        if (CheckCollisionRecs(jogador, plataformas[i])) {
            if (velocidade_y > 0.0f) {
                jogador.y = plataformas[i].y - jogador.height;
                velocidade_y = 0.0f;
                esta_no_chao = true;
            } 
            else if (velocidade_y < 0.0f) {
                jogador.y = plataformas[i].y + plataformas[i].height;
                velocidade_y = 0.0f;
            }
        }
    }
    return jogador;
}

Rectangle atualizar_movimento(Rectangle jogador, float velocidade) {
    (void)velocidade; // Evita aviso de variável não usada
    return jogador;
}

// ARRUMADO: Agora a função recebe "float velocidade" corretamente!
Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas, float velocidade) {
    float velocidade_x = 0.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  velocidade_x -= velocidade;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocidade_x += velocidade;

    if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && esta_no_chao) {
        velocidade_y = FORCA_PULO;
        esta_no_chao = false;
    }

    jogador = mover_e_colidir_x(jogador, velocidade_x, plataformas, quantidade_plataformas);
    jogador = mover_e_colidir_y(jogador, plataformas, quantidade_plataformas);

    return jogador;
}