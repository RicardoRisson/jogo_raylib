#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include "objetos.h"      // Agora o mapa conhece a struct Escada
#include "movimentacao.h" // ADICIONADO: Agora o mapa conhece a struct Plataforma e suas constantes

#define TILE_SIZE 50
#define MAX_LINHAS 10000
#define MAX_COLUNAS 10000
#define MAX_PLATAFORMAS 200
#define MAX_ESCADAS 50

// Função atualizada de Rectangle[] para Plataforma[]
void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, Escada escadas[], int *qtd_escadas, Vector2 *posicao_player);

#endif // MAPA_H