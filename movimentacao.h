#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"

// --- Lógica de Teclado e Física ---
// Processa entradas A/D para X e aplica gravidade/pulo para Y
Rectangle atualizar_movimento(Rectangle jogador, float velocidade);

// --- Lógica de Colisão ---
// Ajusta altura do jogador e reseta aceleração ao tocar plataformas
Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas);
//teste
#endif