#include "personagens.h"

// texturas do jogador (so visiveis nesse arquivo)
static Texture2D sprite_parado_dir; // jogador olhando pra direita
static Texture2D sprite_parado_esq; // jogador olhando pra esquerda
static Texture2D sprite_inimigo_dir; // inimigo indo pra direita
static Texture2D sprite_inimigo_esq; // inimigo indo pra esquerda

// carrega as texturas do disco pra memoria, chamar uma vez no inicio do jogo
void CarregarTexturasPersonagens(void) {
    sprite_parado_dir  = LoadTexture("Sprites/Personagem_Parado_Direita.png");
    sprite_parado_esq  = LoadTexture("Sprites/Personagem_Parado_Esquerda.png");
    sprite_inimigo_dir = LoadTexture("Sprites/Inimigo_direita.png");
    sprite_inimigo_esq = LoadTexture("Sprites/Inimigo_esquerda.png");
}

// descarrega as texturas da memoria, chamar uma vez no fim do jogo
void DescarregarTexturasPersonagens(void) {
    UnloadTexture(sprite_parado_dir);
    UnloadTexture(sprite_parado_esq);
    UnloadTexture(sprite_inimigo_dir);
    UnloadTexture(sprite_inimigo_esq);
}

// cria o Rectangle de hitbox do jogador na posicao de spawn
Rectangle criar_jogador(float spawn_x, float spawn_y, int largura_jogador, int altura_jogador) {
    Rectangle jogador;
    jogador.x      = spawn_x;
    jogador.y      = spawn_y;
    jogador.width  = (float)largura_jogador;
    jogador.height = (float)altura_jogador;
    return jogador;
}

// cria um inimigo normal com a velocidade base
Inimigo criar_inimigo_normal(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    inimigo.hitbox.x      = spawn_x;
    inimigo.hitbox.y      = spawn_y;
    inimigo.hitbox.width  = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    inimigo.velocidade    = velocidade_base; // velocidade normal
    return inimigo;
}

// cria um inimigo rapido com 1.5x a velocidade base
Inimigo criar_inimigo_rapido(float spawn_x, float spawn_y, int largura_inimigo, int altura_inimigo, float velocidade_base) {
    Inimigo inimigo;
    inimigo.hitbox.x      = spawn_x;
    inimigo.hitbox.y      = spawn_y;
    inimigo.hitbox.width  = (float)largura_inimigo;
    inimigo.hitbox.height = (float)altura_inimigo;
    inimigo.velocidade    = velocidade_base * 1.5f; // 50% mais rapido que o normal
    return inimigo;
}

// desenha o jogador centralizado no sprite, que e maior que a hitbox
void DesenharJogador(Rectangle jogador, bool olhando_para_direita) {
    Texture2D textura_atual = olhando_para_direita ? sprite_parado_dir : sprite_parado_esq; // escolhe o sprite pela direcao

    Rectangle origem_jog = { 0.0f, 0.0f, (float)textura_atual.width, (float)textura_atual.height }; // regiao da textura a usar (a textura inteira)

    Rectangle destino_jog = {
        jogador.x - (SPRITE_LARGURA - JOGADOR_LARGURA) / 2.0f, // centraliza o sprite horizontalmente na hitbox
        jogador.y - (SPRITE_ALTURA - JOGADOR_ALTURA),           // alinha o pe do sprite com o pe da hitbox
        SPRITE_LARGURA,  // largura do sprite (maior que a hitbox)
        SPRITE_ALTURA    // altura do sprite (maior que a hitbox)
    };

    Vector2 origem_rotacao_jog = { 0.0f, 0.0f }; // sem rotacao
    DrawTexturePro(textura_atual, origem_jog, destino_jog, origem_rotacao_jog, 0.0f, WHITE); // desenha o sprite
}

// desenha todos os inimigos, escolhendo o sprite pela direcao que estao indo
void DesenharInimigos(Inimigo* inimigos, int qtd_inimigos) {
    for (int i = 0; i < qtd_inimigos; i++) {
        Texture2D textura_inimigo_atual = (inimigos[i].velocidade > 0.0f) ? sprite_inimigo_dir : sprite_inimigo_esq; // velocidade positiva = indo pra direita

        Rectangle origem_inimigo        = { 0.0f, 0.0f, (float)textura_inimigo_atual.width, (float)textura_inimigo_atual.height }; // regiao da textura a usar (a textura inteira)
        Rectangle destino_inimigo       = inimigos[i].hitbox; // desenha exatamente na hitbox
        Vector2 origem_rotacao_inimigo  = { 0.0f, 0.0f }; // sem rotacao
        DrawTexturePro(textura_inimigo_atual, origem_inimigo, destino_inimigo, origem_rotacao_inimigo, 0.0f, WHITE); // desenha o sprite
    }
}