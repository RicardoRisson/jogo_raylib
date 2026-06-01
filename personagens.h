#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include "raylib.h"

// Agora a função pede a posição X e Y de spawn, além do tamanho do jogador
Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador);

#endif