#include "personagens.h"

// Fábrica de jogador: calcula a posição inicial centralizada e retorna o Rectangle
Rectangle criar_jogador(int largura_janela, int altura_janela, int largura_jogador, int altura_jogador) {
    Rectangle jogador;
    
    // Centraliza o jogador horizontalmente na janela
    jogador.x = (largura_janela - largura_jogador) / 2.0f;
    
    // Posiciona o jogador verticalmente (100 pixels acima da base da janela)
    jogador.y = (float)altura_janela - 100 - altura_jogador;
    
    jogador.width = (float)largura_jogador;
    jogador.height = (float)altura_jogador;
    
    return jogador;
}