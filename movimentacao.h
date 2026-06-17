#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"
#include "objetos.h" 
#include "personagens.h"

// Definições de tipo para garantir consistência no uso das plataformas
#define PLATAFORMA_NORMAL 0
#define PLATAFORMA_SOBE   1
#define PLATAFORMA_DESCE  2

// Funções de movimentação pública
// Responsável por toda a lógica de física, gravidade e colisão do jogador
Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade);

// Função genérica de atualização, pode ser usada para movimentações simples
Rectangle atualizar_movimento(Rectangle jogador, float velocidade); 

// Função dedicada para fazer o inimigo andar de um lado para o outro e detectar bordas
Inimigo atualizar_movimento_inimigo(Inimigo inimigo, Plataforma plataformas[], int quantidade_plataformas);

#endif