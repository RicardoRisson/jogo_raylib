#include "raylib.h"
#include "personagens.h"
#include "movimentacao.h" 
#include "mapa.h" 

#define JANELA_LARGURA  1500
#define JANELA_ALTURA   1000
#define JOGADOR_LARGURA 50
#define JOGADOR_ALTURA  50
#define JANELA_TITULO   "teste"

int main(void)
{
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, JANELA_TITULO);
    SetTargetFPS(60);

    // Carrega a textura do jogador (pode ser usada futuramente no DrawTexture)
    Texture2D sprite_jogador = LoadTexture("Sprites/Personagem_Parado_Direita.png");
    float velocidade_jogador = 5.0f;

    // Arrays para guardar o que for lido do TXT (Atualizado para o tipo Plataforma)
    Plataforma plataformas[200];
    int qtd_plataformas = 0;

    Escada escadas[MAX_ESCADAS];
    int qtd_escadas = 0;

    // Vetor para armazenar a posição de spawn que será encontrada no TXT
    Vector2 spawn_jogador;

    // 1. Carrega os dados do arquivo para os arrays e descobre onde está o 'P'
    carregar_mapa("mapa.txt", plataformas, &qtd_plataformas, escadas, &qtd_escadas, &spawn_jogador);

    // 2. O jogador é criado na posição exata do 'P' lido do mapa
    Rectangle jogador = criar_jogador(spawn_jogador.x, spawn_jogador.y, JOGADOR_LARGURA, JOGADOR_ALTURA);

    while (!WindowShouldClose()) 
    {
        // --- Atualiza lógica ---
        // ARRUMADO: Agora chama a função correta passando também as escadas para validar o caminho contínuo (DHHHHHS)
        jogador = verificar_chao_com_escadas(jogador, plataformas, qtd_plataformas, escadas, qtd_escadas, velocidade_jogador);

        // --- Renderiza ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Desenha todas as escadas carregadas
            for (int i = 0; i < qtd_escadas; i++) {
                DrawRectangleRec(escadas[i].rect, BROWN);
            }

            // Desenha todas as plataformas carregadas
            for (int i = 0; i < qtd_plataformas; i++) {
                // Diferenciação visual para você testar no mapa:
                Color cor_plataforma = RED; 
                if (plataformas[i].tipo == PLATAFORMA_SOBE) cor_plataforma = GREEN;  // Aperta W para subir
                if (plataformas[i].tipo == PLATAFORMA_DESCE) cor_plataforma = BLUE;   // Aperta S para descer

                // Passando corretamente o .rect interno da estrutura
                DrawRectangleRec(plataformas[i].rect, cor_plataforma);
            }

            // Desenha o jogador (retângulo verde temporário)
            DrawRectangleRec(jogador, GREEN);
            
        EndDrawing();
    }

    // Descarrega a textura da memória antes de fechar o jogo
    UnloadTexture(sprite_jogador);

    CloseWindow();
    return 0;
}