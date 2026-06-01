#ifndef MOVIMENTACAO_H
#define MOVIMENTACAO_H

#include "raylib.h"

// Garanta que a assinatura receba o float velocidade no final
Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas, float velocidade);

// Mantida apenas se você ainda chamar em algum lugar, senão pode apagar
Rectangle atualizar_movimento(Rectangle jogador, float velocidade); 

#endif