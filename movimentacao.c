#include "movimentacao.h"

// Atualiza a posição horizontal do jogador com base nas teclas pressionadas
Rectangle atualizar_movimento(Rectangle jogador, Rectangle plataforma_referencia, float velocidade) {
    
    // Movimento para a esquerda (Teclar A)
    if (IsKeyDown(KEY_A)) {
        float nova_posicao_x = jogador.x - velocidade;
        
        // Mantém o jogador dentro do limite esquerdo da plataforma de referência
        if (nova_posicao_x >= plataforma_referencia.x) {
            jogador.x = nova_posicao_x;
        }
    }

    // Movimento para a direita (Tecla D)
    if (IsKeyDown(KEY_D)) {
        float nova_posicao_x = jogador.x + velocidade;
        float limite_direito = plataforma_referencia.x + plataforma_referencia.width;
        
        // Mantém o jogador dentro do limite direito da plataforma de referência
        if (nova_posicao_x + jogador.width <= limite_direito) {
            jogador.x = nova_posicao_x;
        }
    }

    return jogador;
}

// Verifica colisão com todas as plataformas e ajusta a altura do jogador
Rectangle verificar_chao(Rectangle jogador, Rectangle plataformas[], int quantidade_plataformas) {
    
    for (int i = 0; i < quantidade_plataformas; i++) {
        // Se houver colisão, o jogador é posicionado exatamente em cima da plataforma
        if (CheckCollisionRecs(jogador, plataformas[i])) {
            jogador.y = plataformas[i].y - jogador.height;
            break; // Sai do loop ao encontrar a primeira colisão válida
        }
    }
    
    return jogador;
}