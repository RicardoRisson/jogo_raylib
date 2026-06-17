#include "personagens.h"

static Texture2D sprite_parado_dir;
static Texture2D sprite_parado_esq;
static Texture2D sprite_inimigo_dir;
static Texture2D sprite_inimigo_esq;

// Carrega as texturas 
void CarregarTexturasPersonagens(void) {
    sprite_parado_dir = LoadTexture("Sprites/Personagem_Parado_Direita.png");
    sprite_parado_esq = LoadTexture("Sprites/Personagem_Parado_Esquerda.png");
    sprite_inimigo_dir = LoadTexture("Sprites/Inimigo_direita.png");
    sprite_inimigo_esq = LoadTexture("Sprites/Inimigo_esquerda.png");
}

// Descarrega as texturas 
void DescarregarTexturasPersonagens(void) {
    UnloadTexture(sprite_parado_dir);
    UnloadTexture(sprite_parado_esq);
    UnloadTexture(sprite_inimigo_dir);
    UnloadTexture(sprite_inimigo_esq);
}

Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador) {
    Rectangle jogador;
    jogador.x = spawn_x;
    jogador.y = spawn_y;
    jogador.width = (float)largura_jogador;
    jogador.height = (float)altura_jogador;
    return jogador;
}

Inimigo criar_inimigo_normal(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    inimigo.hitbox.x = spawn_x;
    inimigo.hitbox.y = spawn_y;
    inimigo.hitbox.width = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    inimigo.velocidade = velocidade_base;
    return inimigo;
}

Inimigo criar_inimigo_rapido(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    inimigo.hitbox.x = spawn_x;
    inimigo.hitbox.y = spawn_y;
    inimigo.hitbox.width = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    inimigo.velocidade = velocidade_base * 1.5f;
    return inimigo;
}

// Renderiza o Jogador 
void DesenharJogador(Rectangle jogador, bool olhando_para_direita) {
    Texture2D textura_atual = olhando_para_direita ? sprite_parado_dir : sprite_parado_esq;
    
    Rectangle origem_jog = { 0.0f, 0.0f, (float)textura_atual.width, (float)textura_atual.height };
    Rectangle destino_jog = {
        jogador.x - (SPRITE_LARGURA - JOGADOR_LARGURA) / 2.0f,
        jogador.y - (SPRITE_ALTURA - JOGADOR_ALTURA), 
        SPRITE_LARGURA,
        SPRITE_ALTURA
    };
    Vector2 origem_rotacao_jog = { 0.0f, 0.0f };

    DrawTexturePro(textura_atual, origem_jog, destino_jog, origem_rotacao_jog, 0.0f, WHITE);
}

// Renderiza o loop de inimigos
void DesenharInimigos(Inimigo* inimigos, int qtd_inimigos) {
    for (int i = 0; i < qtd_inimigos; i++) {
        Texture2D textura_inimigo_atual = (inimigos[i].velocidade > 0.0f) ? sprite_inimigo_dir : sprite_inimigo_esq;

        Rectangle origem_inimigo = { 0.0f, 0.0f, (float)textura_inimigo_atual.width, (float)textura_inimigo_atual.height };
        Rectangle destino_inimigo = inimigos[i].hitbox;
        Vector2 origem_rotacao_inimigo = { 0.0f, 0.0f };

        DrawTexturePro(textura_inimigo_atual, origem_inimigo, destino_inimigo, origem_rotacao_inimigo, 0.0f, WHITE);
    }
}