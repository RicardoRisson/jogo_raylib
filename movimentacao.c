#include "movimentacao.h"
#include <raylib.h>

// --- Constantes de Física ---
static const float GRAVIDADE     = 0.5f;
static const float FORCA_PULO    = -15.0f;
static const float LIMITE_QUEDA  = 15.0f;

// --- Estado do Jogador ---
static float velocidade_y = 0.0f;
static bool esta_no_chao  = false;

Rectangle atualizar_movimento(Rectangle jogador, float velocidade) {
    // Deslocamento horizontal
    if (IsKeyDown(KEY_A)) jogador.x -= velocidade;
    if (IsKeyDown(KEY_D)) jogador.x += velocidade;

    // Lógica de pulo
    if (IsKeyPressed(KEY_W) && esta_no_chao) {
        velocidade_y = FORCA_PULO;
        esta_no_chao = false;
    }

    // Aplicação da gravidade com limitador
    velocidade_y += GRAVIDADE;
    if (velocidade_y > LIMITE_QUEDA) velocidade_y = LIMITE_QUEDA;
    
    jogador.y += velocidade_y;

    return jogador;
}

Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas) {
    esta_no_chao = false;

    for (int i = 0; i < quantidade_plataformas; i++) {
        // Detecção de colisão retangular
        if (CheckCollisionRecs(jogador, plataformas[i])) {
            // Ajuste de posição apenas em queda para evitar bugs em saltos
            if (velocidade_y > 0) {
                jogador.y = plataformas[i].y - jogador.height;
                velocidade_y = 0;
                esta_no_chao = true;
            }
        }
    }
    
    return jogador;
}