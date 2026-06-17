#include "objetos.h"

// Variáveis estáticas para armazenar as texturas do cenário
static Texture2D textura_plataforma;
static Texture2D textura_escada;
static Texture2D textura_porta;

// Carrega as texturas da VRAM
void CarregarTexturasObjetos(void) {
    textura_plataforma = LoadTexture("Sprites/Plataforma.png");
    textura_escada = LoadTexture("Sprites/Escada.png");
    textura_porta = LoadTexture("Sprites/porta.png");
}

// Descarrega as texturas da VRAM
void DescarregarTexturasObjetos(void) {
    UnloadTexture(textura_plataforma);
    UnloadTexture(textura_escada);
    UnloadTexture(textura_porta);
}

Rectangle criar_plataforma(float x, float y, float largura, float altura) {
    Rectangle plataforma;
    plataforma.x = x;
    plataforma.y = y;
    plataforma.width = largura;
    plataforma.height = altura;
    return plataforma;
}

Escada criar_escada(float x, float y, float largura, float altura, int tipo) {
    Escada escada;
    escada.rect.x = x;
    escada.rect.y = y;
    escada.rect.width = largura;
    escada.rect.height = altura;
    escada.tipo = tipo; 
    return escada;
}

Rectangle criar_portal(float x, float y, float largura, float altura) {
    Rectangle portal;
    portal.x = x;
    portal.y = y;
    portal.width = largura;
    portal.height = altura;
    return portal;
}

// Renderiza todas as plataformas
void DesenharPlataformas(Plataforma* plataformas, int qtd_plataformas) {
    for (int i = 0; i < qtd_plataformas; i++) {
        Rectangle origem_plat = { 0.0f, 0.0f, (float)textura_plataforma.width, (float)textura_plataforma.height };
        Rectangle destino_plat = plataformas[i].rect; // Se for tipo Rectangle direto, use apenas plataformas[i]
        Vector2 origem_rotacao_plat = { 0.0f, 0.0f };

        DrawTexturePro(textura_plataforma, origem_plat, destino_plat, origem_rotacao_plat, 0.0f, WHITE);
    }
}

// Renderiza todas as escadas
void DesenharEscadas(Escada* escadas, int qtd_escadas) {
    for (int i = 0; i < qtd_escadas; i++) {
        Rectangle origem_escada = { 0.0f, 0.0f, (float)textura_escada.width, (float)textura_escada.height };
        Rectangle destino_escada = escadas[i].rect;
        Vector2 origem_rotacao_escada = { 0.0f, 0.0f };

        DrawTexturePro(textura_escada, origem_escada, destino_escada, origem_rotacao_escada, 0.0f, WHITE);
    }
}

// Renderiza o portal/porta de fim de fase
void DesenharPortal(Rectangle portal, bool precisa_carregar_fase) {
    if (!precisa_carregar_fase) {
        Rectangle origem_porta = { 0.0f, 0.0f, (float)textura_porta.width, (float)textura_porta.height };
        Rectangle destino_porta = portal;
        Vector2 origem_rotacao_porta = { 0.0f, 0.0f };

        DrawTexturePro(textura_porta, origem_porta, destino_porta, origem_rotacao_porta, 0.0f, WHITE);
    }
}