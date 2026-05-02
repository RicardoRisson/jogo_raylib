#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"

// Protótipo para a função de movimentação horizontal
// Controla as teclas A/D e limita o jogador a uma plataforma de referência
Rectangle atualizar_movimento(Rectangle jogador, Rectangle plataforma_referencia, float velocidade);

// Protótipo para a função de detecção de solo
// Percorre o array de plataformas para encontrar onde o jogador deve pisar
Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas);

#endif