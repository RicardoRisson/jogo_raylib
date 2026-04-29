// Inclusão da biblioteca Raylib
#include "raylib.h"

// Definição das constantes do jogo
#define ALTURA_JANELA 1000
#define LARGURA_JANELA 1000
#define ALTURA_JOGADOR 50
#define LARGURA_JOGADOR 50
#define TITULO_DO_JOGO "teste"

int main(void)
{
    int velocidade_jogador = 5; // Velocidade de movimento do jogador (mantida variável para futuras implementações)

    Rectangle jogador = { // Retângulo que representa o jogador
        (LARGURA_JANELA - LARGURA_JOGADOR) / 2.0f,
        ALTURA_JANELA - 100 - ALTURA_JOGADOR,
        LARGURA_JOGADOR,
        ALTURA_JOGADOR
    };

    Rectangle chao = { // Retângulo que representa o chão
        (LARGURA_JANELA - (LARGURA_JANELA - 200)) / 2.0f,
        ALTURA_JANELA - 100,
        LARGURA_JANELA - 200,
        100
    };

    InitWindow(LARGURA_JANELA, ALTURA_JANELA, TITULO_DO_JOGO); // Inicializa a janela
    SetTargetFPS(60); // Fixa a taxa de quadros em 60 FPS

    while (!WindowShouldClose()) // Loop principal do jogo
    {
        // Movimento para a esquerda: só permite mover se o jogador continuar sobre o chão
        if (IsKeyDown(KEY_A))
        {
            if (jogador.x - velocidade_jogador >= chao.x)
            {
                jogador.x -= velocidade_jogador;
            }
        }

        // Movimento para a direita: verifica a borda direita do jogador antes de mover
        if (IsKeyDown(KEY_D))
        {
            if (jogador.x + jogador.width + velocidade_jogador <= chao.x + chao.width)
            {
                jogador.x += velocidade_jogador;
            }
        }

        jogador.y = chao.y - jogador.height; // Mantém o jogador sempre apoiado sobre o chão

        BeginDrawing();
            ClearBackground(RAYWHITE);         // Limpa a tela com fundo branco
            DrawRectangleRec(chao, RED);       // Desenha o chão
            DrawRectangleRec(jogador, GREEN);  // Desenha o jogador
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
   TODO: adicionar mais uma plataforma em cima dele, e criar uma escada que quando pressiona W teleporta ele pra cima
         a escada deve também permitir descer caso ele aperte A em cima dela, devemos também ainda nessa "sessao"
         criar alguma lib atraves do include (faze nosso proprio) para poder colocar comandos que ocupam muito espaco
         nele, como as teclas de movimento bem como TALVEZ alguma forma de gerar plataformas de forma mais facil
 */
