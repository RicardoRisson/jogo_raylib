#include "objetos.h"

// variaveis estaticas para armazenar as texturas do cenario (so visiveis nesse arquivo)
static Texture2D textura_plataforma;
static Texture2D textura_escada;
static Texture2D textura_porta;

// carrega as texturas do disco pra memoria, chamar uma vez no inicio do jogo
void CarregarTexturasObjetos(void) {
    textura_plataforma = LoadTexture("Sprites/Plataforma.png");
    textura_escada     = LoadTexture("Sprites/Escada.png");
    textura_porta      = LoadTexture("Sprites/porta.png");
}

// descarrega as texturas da memoria, chamar uma vez no fim do jogo
void DescarregarTexturasObjetos(void) {
    UnloadTexture(textura_plataforma);
    UnloadTexture(textura_escada);
    UnloadTexture(textura_porta);
}

// cria um Rectangle com as dimensoes passadas (usado antes de existir a struct Plataforma)
Rectangle criar_plataforma(float x, float y, float largura, float altura) {
    Rectangle plataforma;
    plataforma.x      = x;
    plataforma.y      = y;
    plataforma.width  = largura;
    plataforma.height = altura;
    return plataforma;
}

// cria uma Escada com posicao, tamanho e tipo
Escada criar_escada(float x, float y, float largura, float altura, int tipo) {
    Escada escada;
    escada.rect.x      = x;
    escada.rect.y      = y;
    escada.rect.width  = largura;
    escada.rect.height = altura;
    escada.tipo        = tipo; // reservado pra uso futuro, nao usado ainda
    return escada;
}

// cria um Rectangle representando o portal de saida da fase
Rectangle criar_portal(float x, float y, float largura, float altura) {
    Rectangle portal;
    portal.x      = x;
    portal.y      = y;
    portal.width  = largura;
    portal.height = altura;
    return portal;
}

// desenha todas as plataformas na tela estirando a textura pro tamanho do tile
void DesenharPlataformas(Plataforma* plataformas, int qtd_plataformas) {
    for (int i = 0; i < qtd_plataformas; i++) {
        Rectangle origem_plat          = { 0.0f, 0.0f, (float)textura_plataforma.width, (float)textura_plataforma.height }; // regiao da textura a usar (a textura inteira)
        Rectangle destino_plat         = plataformas[i].rect; // onde desenhar na tela (posicao e tamanho do tile)
        Vector2 origem_rotacao_plat    = { 0.0f, 0.0f }; // ponto de rotacao (sem rotacao, pode ignorar)
        DrawTexturePro(textura_plataforma, origem_plat, destino_plat, origem_rotacao_plat, 0.0f, WHITE); // desenha a textura esticada no tile
    }
}

// desenha todas as escadas na tela estirando a textura pro tamanho do tile
void DesenharEscadas(Escada* escadas, int qtd_escadas) {
    for (int i = 0; i < qtd_escadas; i++) {
        Rectangle origem_escada        = { 0.0f, 0.0f, (float)textura_escada.width, (float)textura_escada.height }; // regiao da textura a usar (a textura inteira)
        Rectangle destino_escada       = escadas[i].rect; // onde desenhar na tela
        Vector2 origem_rotacao_escada  = { 0.0f, 0.0f }; // ponto de rotacao (sem rotacao)
        DrawTexturePro(textura_escada, origem_escada, destino_escada, origem_rotacao_escada, 0.0f, WHITE); // desenha a textura esticada no tile
    }
}

// desenha o portal de saida, mas so se a fase ja foi totalmente carregada
void DesenharPortal(Rectangle portal, bool precisa_carregar_fase) {
    if (!precisa_carregar_fase) { // se ainda esta carregando, nao desenha o portal
        Rectangle origem_porta         = { 0.0f, 0.0f, (float)textura_porta.width, (float)textura_porta.height }; // regiao da textura a usar (a textura inteira)
        Rectangle destino_porta        = portal; // onde desenhar na tela
        Vector2 origem_rotacao_porta   = { 0.0f, 0.0f }; // ponto de rotacao (sem rotacao)
        DrawTexturePro(textura_porta, origem_porta, destino_porta, origem_rotacao_porta, 0.0f, WHITE); // desenha a textura esticada no tile
    }
}