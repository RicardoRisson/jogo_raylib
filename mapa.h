#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include "objetos.h"      
#include "personagens.h" 

#define TILE_SIZE 50
#define MAX_LINHAS 1000
#define MAX_COLUNAS 1000
#define MAX_PLATAFORMAS 500
#define MAX_INIMIGOS 30

void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, 
                   Escada escadas[], int *qtd_escadas, Vector2 *posicao_player, Rectangle *portal,
                   Inimigo inimigos[], int *qtd_inimigos);

#endif