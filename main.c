#include "raylib.h"
#include "personagens.h"
#include "movimentacao.h" 
#include "mapa.h" 
#include <stdio.h> 

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

    // Arrays para guardar os inimigos carregados do mapa
    Inimigo inimigos[MAX_INIMIGOS];
    int qtd_inimigos = 0;

    // Vetor para armazenar a posição de spawn que será encontrada no TXT
    Vector2 spawn_jogador;

    // --- SISTEMA DE FASES POR CHAR ---
    char fase_atual = 'A';                 // Começa na fase 'A' (ASCII 65)
    char nome_arquivo[20];                 // Armazena o nome do mapa dinâmico ("mapa_A.txt", etc.)
    Rectangle portal_proxima_fase = { 0 }; // Quadrado do sensor vindo do objetos.c
    bool precisa_carregar_fase = true;     // Flag para controlar quando carregar/trocar o mapa
    
    Rectangle jogador;

    // --- CONFIGURAÇÃO INICIAL DA CÂMERA 2D ---
    Camera2D camera = { 0 };
    camera.offset = (Vector2){ JANELA_LARGURA / 2.0f, JANELA_ALTURA / 2.0f }; // Foca no centro da janela
    camera.rotation = 0.0f;
    camera.zoom = 1.0f; // Zoom normal (100%)

    while (!WindowShouldClose()) 
    {
        // Se a flag estiver ativa, reconstrói o cenário e reinicia o jogador
        if (precisa_carregar_fase) {
            // Monta o nome dinamicamente usando o caractere ASCII atual
            sprintf(nome_arquivo, "mapa_%c.txt", fase_atual);
            
            // 1. Carrega os dados do arquivo passando o novo parâmetro do portal e dos inimigos
            carregar_mapa(nome_arquivo, plataformas, &qtd_plataformas, escadas, &qtd_escadas, &spawn_jogador, &portal_proxima_fase, inimigos, &qtd_inimigos);
            
            // 2. O jogador é criado na posição exata do 'P' lido do mapa atual
            jogador = criar_jogador(spawn_jogador.x, spawn_jogador.y, JOGADOR_LARGURA, JOGADOR_ALTURA);
            
            precisa_carregar_fase = false; // Desativa a flag após carregar tudo
        }

        // --- Atualiza lógica de movimentação e física do jogador ---
        jogador = verificar_chao_com_escadas(jogador, plataformas, qtd_plataformas, escadas, qtd_escadas, velocidade_jogador);

        // --- Atualiza lógica de movimentação de todos os inimigos ---
        for (int i = 0; i < qtd_inimigos; i++) {
            inimigos[i] = atualizar_movimento_inimigo(inimigos[i], plataformas, qtd_plataformas);
            
            // --- VERIFICAÇÃO DE TOQUE NO INIMIGO (DANO / GAME OVER) ---
            if (CheckCollisionRecs(jogador, inimigos[i].hitbox)) {
                fase_atual = 'A';             // Volta para o mapa 1 (fase 'A')
                precisa_carregar_fase = true; // Ativa a flag para recarregar o mapa e spawnar o jogador
                break;                        // Sai do loop de verificação já que a fase vai mudar
            }
        }

        // --- VERIFICAÇÃO DE MUDANÇA DE FASE (PORTAL) ---
        if (!precisa_carregar_fase && CheckCollisionRecs(jogador, portal_proxima_fase)) {
            fase_atual++; // Avança na tabela ASCII: 'A' -> 'B' -> 'C' -> 'D'
            
            if (fase_atual > 'D') { 
                fase_atual = 'A'; 
            }
            
            precisa_carregar_fase = true; // Ativa a flag para carregar o novo arquivo no próximo frame
        }

        // --- ATUALIZA A POSIÇÃO DA CÂMERA ---
        // Faz o alvo (target) da câmera seguir o centro do retângulo do jogador continuamente
        camera.target = (Vector2){ jogador.x + jogador.width / 2.0f, jogador.y + jogador.height / 2.0f };

        // --- Renderiza os elementos na tela ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // --- INICIA MODO CÂMERA 2D ---
            // Tudo desenhado aqui dentro vai se mover de acordo com a posição do jogador
            BeginMode2D(camera);

                // Desenha todas as escadas carregadas
                for (int i = 0; i < qtd_escadas; i++) {
                    DrawRectangleRec(escadas[i].rect, BROWN);
                }

                // Desenha todas as plataformas carregadas
                for (int i = 0; i < qtd_plataformas; i++) {
                    // Diferenciação visual baseada no tipo da plataforma
                    Color cor_plataforma = RED; 
                    if (plataformas[i].tipo == PLATAFORMA_SOBE) cor_plataforma = GREEN;  // Aperta W para subir
                    if (plataformas[i].tipo == PLATAFORMA_DESCE) cor_plataforma = BLUE;   // Aperta S para descer

                    DrawRectangleRec(plataformas[i].rect, cor_plataforma);
                }

                // Desenha todos os inimigos carregados (representados por retângulos laranjas temporários)
                for (int i = 0; i < qtd_inimigos; i++) {
                    DrawRectangleRec(inimigos[i].hitbox, ORANGE);
                }

                // Desenha o sensor/portal de próxima fase (Sem textura, apenas um quadrado roxo)
                if (!precisa_carregar_fase) {
                    DrawRectangleRec(portal_proxima_fase, PURPLE);
                }

                // Desenha o jogador (retângulo verde temporário)
                DrawRectangleRec(jogador, GREEN);

            EndMode2D();
            // --- TERMINA MODO CÂMERA 2D ---
            
            // Dica: Se quiser desenhar elementos fixos na tela (como uma barra de vida ou texto de pontuação),
            // desenhe-os aqui, depois do EndMode2D() para que eles não se movam com o cenário.

        EndDrawing();
    }

    // Descarrega a textura da memória antes de encerrar o processo
    UnloadTexture(sprite_jogador);

    CloseWindow();
    return 0;
}