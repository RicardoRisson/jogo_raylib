#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"
#include "objetos.h" 
#include "personagens.h"

#define PLATAFORMA_NORMAL 0
#define PLATAFORMA_SOBE   1
#define PLATAFORMA_DESCE  2

// Logica de física, gravidade e colisão do jogador
Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade);

// Funcao de atualização de movimento
Rectangle atualizar_movimento(Rectangle jogador, float velocidade); 

// Funcao de atualização de movimento do inimigo pq ele detecta bordas
Inimigo atualizar_movimento_inimigo(Inimigo inimigo, Plataforma plataformas[], int quantidade_plataformas);

#endif