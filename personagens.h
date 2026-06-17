#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include "raylib.h"

// --- AJUSTE DE HITBOX E RENDERIZAÇÃO ---
#define JOGADOR_LARGURA 30
#define JOGADOR_ALTURA  40
#define SPRITE_LARGURA  50
#define SPRITE_ALTURA   50

// Estrutura para os inimigos, guardando a hitbox (Rectangle) e a velocidade
typedef struct Inimigo {
    Rectangle hitbox;
    float velocidade;
} Inimigo;

// Funções de Inicialização e Ciclo de Vida das Texturas
void CarregarTexturasPersonagens(void);
void DescarregarTexturasPersonagens(void);

// Funções de Criação
Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador);
Inimigo criar_inimigo_normal(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base);
Inimigo criar_inimigo_rapido(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base);

// Funções de Renderização (Desenho)
void DesenharJogador(Rectangle jogador, bool olhando_para_direita);
void DesenharInimigos(Inimigo* inimigos, int qtd_inimigos);

#endif