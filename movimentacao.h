#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"
#include "objetos.h" 
#include "personagens.h" // Adicionado para reconhecer a struct Inimigo

#define PLATAFORMA_NORMAL 0
#define PLATAFORMA_SOBE   1
#define PLATAFORMA_DESCE  2

typedef struct {
    Rectangle rect;
    int tipo; // 0 = Normal, 1 = Sobe, 2 = Desce
} Plataforma;

Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade);
Rectangle criar_portal(float x, float y, float largura, float altura);
Rectangle atualizar_movimento(Rectangle jogador, float velocidade); 

// Função dedicada para fazer o inimigo andar e bater nas paredes
Inimigo atualizar_movimento_inimigo(Inimigo inimigo, Plataforma plataformas[], int quantidade_plataformas);

#endif