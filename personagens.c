#include "personagens.h"

// --- Constantes de Posicionamento ---
static const float MARGEM_INFERIOR = 100.0f;

Rectangle criar_jogador(int largura_janela, int altura_janela, int largura_jogador, int altura_jogador) {
    Rectangle jogador;
    
    // Cálculo de centralização horizontal
    jogador.x = (largura_janela - largura_jogador) / 2.0f;
    
    // Posicionamento vertical relativo à base da janela
    jogador.y = (float)altura_janela - MARGEM_INFERIOR - altura_jogador;
    
    // Definição das dimensões do container
    jogador.width = (float)largura_jogador;
    jogador.height = (float)altura_jogador;
    
    return jogador;
}