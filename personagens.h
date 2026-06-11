#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include "raylib.h"

// Estrutura para os inimigos, guardando a hitbox (Rectangle) e a velocidade
typedef struct Inimigo {
    Rectangle hitbox;
    float velocidade;
} Inimigo;

// Função pede a posição X e Y de spawn, além do tamanho do jogador
Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador);

// Funções para criar os inimigos
Inimigo criar_inimigo_normal(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base);
Inimigo criar_inimigo_rapido(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base);

#endif