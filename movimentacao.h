#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"
#include "objetos.h" // ADICIONADO: Necessário para o cabeçalho reconhecer a estrutura Escada

// Usando constantes numéricas simples em vez de enum
#define PLATAFORMA_NORMAL 0
#define PLATAFORMA_SOBE   1
#define PLATAFORMA_DESCE  2

typedef struct {
    Rectangle rect;
    int tipo; // 0 = Normal, 1 = Sobe, 2 = Desce
} Plataforma;

// ARRUMADO: Nome atualizado e agora recebe também as escadas e sua quantidade para a validação do caminho contínuo
Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade);
Rectangle criar_portal(float x, float y, float largura, float altura);
Rectangle atualizar_movimento(Rectangle jogador, float velocidade); 

#endif // MOVIMENTACAO_H