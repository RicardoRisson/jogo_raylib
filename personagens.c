#include "personagens.h"

Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador) {
    Rectangle jogador;
    
    // Define a posição diretamente com base no que veio do mapa
    jogador.x = spawn_x;
    jogador.y = spawn_y;
    
    // Definição das dimensões
    jogador.width = (float)largura_jogador;
    jogador.height = (float)altura_jogador;
    
    return jogador;
}