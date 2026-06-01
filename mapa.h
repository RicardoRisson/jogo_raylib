#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"

#define TILE_SIZE 50
#define MAX_LINHAS 10000
#define MAX_COLUNAS 10000
#define MAX_PLATAFORMAS 200
#define MAX_ESCADAS 50

// Adicionamos o "Vector2 *posicao_player" no final da assinatura da função
void carregar_mapa(const char *caminho_arquivo, Rectangle plataformas[], int *qtd_plataformas, Rectangle escadas[], int *qtd_escadas, Vector2 *posicao_player);

#endif