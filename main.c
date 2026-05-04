#include "raylib.h"
#include "personagens.h"
#include "movimentacao.h" 
#include "objetos.h"

#define JANELA_LARGURA  1500
#define JANELA_ALTURA   1000
#define JOGADOR_LARGURA 50
#define JOGADOR_ALTURA  50
#define JANELA_TITULO   "teste"

#define P1_X 250.0f
#define P1_Y 900.0f
#define P1_W 1000.0f
#define P1_H 50.0f

#define P2_X 340.0f
#define P2_Y 600.0f
#define P2_W 800.0f
#define P2_H 50.0f

int main(void)
{
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, JANELA_TITULO);
    SetTargetFPS(60);

    Texture2D sprite_jogador = LoadTexture("Sprites/Personagem_Parado_Direita.png");
    float velocidade_jogador = 5.0f;

    Rectangle jogador = criar_jogador(JANELA_LARGURA / 2, 0, JOGADOR_LARGURA, JOGADOR_ALTURA);
    Rectangle chao_1_f1 = criar_plataforma(P1_X, P1_Y, P1_W, P1_H);
    Rectangle chao_2_f1 = criar_plataforma(P2_X, P2_Y, P2_W, P2_H);

    Rectangle plataformas[] = { chao_1_f1, chao_2_f1 };
    int qtd_plataformas = 2;

    while (!WindowShouldClose()) 
    {
        // Atualiza lógica
        jogador.y += 5.0f; // Gravidade
        jogador = atualizar_movimento(jogador, velocidade_jogador); 
        jogador = verificar_chao(jogador, plataformas, qtd_plataformas);
        
        // Renderiza
        BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawRectangleRec(chao_1_f1, RED);
            DrawRectangleRec(chao_2_f1, RED);
            DrawRectangleRec(jogador,GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}