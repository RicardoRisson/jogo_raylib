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

Inimigo criar_inimigo_normal(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    
    // Define a posição diretamente com base no que veio do mapa
    inimigo.hitbox.x = spawn_x;
    inimigo.hitbox.y = spawn_y;
    
    // Definição das dimensões
    inimigo.hitbox.width = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    
    // Define a velocidade normal
    inimigo.velocidade = velocidade_base;
    
    return inimigo;
}

Inimigo criar_inimigo_rapido(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    
    // Define a posição diretamente com base no que veio do mapa
    inimigo.hitbox.x = spawn_x;
    inimigo.hitbox.y = spawn_y;
    
    // Definição das dimensões
    inimigo.hitbox.width = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    
    // Define a velocidade 1.5x mais rápida
    inimigo.velocidade = velocidade_base * 1.5f;
    
    return inimigo;
}