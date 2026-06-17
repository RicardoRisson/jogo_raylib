#include "raylib.h"
#include "personagens.h"
#include "movimentacao.h" 
#include "mapa.h" 
#include "interface.h"
#include "objetos.h" 
#include <stdio.h>
#include <float.h>

#define JANELA_LARGURA  1500
#define JANELA_ALTURA   1000

#define JOGADOR_LARGURA 30
#define JOGADOR_ALTURA  40
#define SPRITE_LARGURA  50
#define SPRITE_ALTURA   50

#define JANELA_TITULO   "ninja contra os ninjas verdes"

int main(void)
{
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, JANELA_TITULO);
    SetTargetFPS(60);

    // Impede que a tecla ESC feche a janela do jogo automaticamente
    SetExitKey(KEY_NULL);

    // Garante que o arquivo do placar exista
    inicializar_placar_vazio();

    GameState estado_atual = STATE_MENU;
    int opcao_menu_principal = 0;
    int opcao_menu_pausa = 0;
    
    // Variáveis de input de nome do jogador
    char nome_input[20] = "\0";
    int letras_input = 0;
    int tecla = 0; 

    // Tempo total de jogo
    float tempo_jogado = 0.0f;
    int tempo_final = 0;

    // Variáveis para rastrear o tempo de queda livre do jogador
    float cronometro_queda = 0.0f;
    float y_ultimo_frame = 0.0f;

    // Carrega as texturas usando o sistema modular que você criou
    CarregarTexturasPersonagens();
    CarregarTexturasObjetos();
    bool olhando_para_direita = true;

    float velocidade_jogador = 5.0f;

    Plataforma plataformas[200];
    int qtd_plataformas = 0;
    Escada escadas[MAX_ESCADAS];
    int qtd_escadas = 0;
    Inimigo inimigos[MAX_INIMIGOS];
    int qtd_inimigos = 0;

    Vector2 spawn_jogador;
    char fase_atual = 'A';
    char nome_arquivo[20];
    Rectangle portal_proxima_fase = { 0 };
    bool precisa_carregar_fase = true;
    Rectangle jogador;

    Camera2D camera = { 0 };
    camera.offset = (Vector2){ JANELA_LARGURA / 2.0f, JANELA_ALTURA / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    while (!WindowShouldClose()) 
    {
        // Botoes interface
        AtualizarBotoesInterface(&estado_atual, &opcao_menu_pausa);

        // Logica de estado atual
        switch (estado_atual) {
            case STATE_MENU:
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcao_menu_principal = (opcao_menu_principal + 1) % 3;
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) opcao_menu_principal = (opcao_menu_principal + 2) % 3;

                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcao_menu_principal == 0) { 
                        fase_atual = 'A';
                        precisa_carregar_fase = true;
                        tempo_jogado = 0.0f; 
                        estado_atual = STATE_JOGANDO;
                    } 
                    else if (opcao_menu_principal == 1) { 
                        estado_atual = STATE_RANKING;
                    } 
                    else if (opcao_menu_principal == 2) { 
                        CloseWindow();
                        return 0;
                    }
                }
                break;

            case STATE_JOGANDO:
                tempo_jogado += GetFrameTime();

                if (precisa_carregar_fase) {
                    sprintf(nome_arquivo, "mapa_%c.txt", fase_atual);
                    carregar_mapa(nome_arquivo, plataformas, &qtd_plataformas, escadas, &qtd_escadas, &spawn_jogador, &portal_proxima_fase, inimigos, &qtd_inimigos);
                    jogador = criar_jogador(spawn_jogador.x, spawn_jogador.y, JOGADOR_LARGURA, JOGADOR_ALTURA);
                    precisa_carregar_fase = false;
                    cronometro_queda = 0.0f;
                    y_ultimo_frame = jogador.y;
                }

                if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
                    olhando_para_direita = true;
                }
                else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
                    olhando_para_direita = false;
                }

                // Guarda a posição Y antiga antes de aplicar a movimentação/gravidade
                y_ultimo_frame = jogador.y;

                jogador = verificar_chao_com_escadas(jogador, plataformas, qtd_plataformas, escadas, qtd_escadas, velocidade_jogador);

                // Se a posição Y aumentou, ele está descendo/caindo
                if (jogador.y > y_ultimo_frame) {
                    cronometro_queda += GetFrameTime();
                } else {
                    cronometro_queda = 0.0f; 
                }

                // Se cair por mais de 2 segundos cria um hit-box invisível no pé e força a morte
                if (cronometro_queda >= 2.0f) {
                    Rectangle inimigo_invisivel = { jogador.x, jogador.y + jogador.height - 5.0f, jogador.width, 10.0f };
                    
                    if (CheckCollisionRecs(jogador, inimigo_invisivel)) {
                        fase_atual = 'A'; 
                        precisa_carregar_fase = true;
                        tempo_jogado = 0.0f; 
                        cronometro_queda = 0.0f;
                        break;
                    }
                }

                // Loop de colisão com os inimigos do mapa
                for (int i = 0; i < qtd_inimigos; i++) {
                    inimigos[i] = atualizar_movimento_inimigo(inimigos[i], plataformas, qtd_plataformas);
                    
                    if (CheckCollisionRecs(jogador, inimigos[i].hitbox)) {
                        fase_atual = 'A'; 
                        precisa_carregar_fase = true;
                        tempo_jogado = 0.0f; 
                        break;
                    }
                }

                if (!precisa_carregar_fase && CheckCollisionRecs(jogador, portal_proxima_fase)) {
                    fase_atual++; 
                    
                    if (fase_atual > 'C') { 
                        tempo_final = (int)tempo_jogado;
                        
                        if (verificar_se_entra_no_ranking(tempo_final)) {
                            estado_atual = STATE_SALVAR_RECORDE;
                            nome_input[0] = '\0';
                            letras_input = 0;
                        } else {
                            estado_atual = STATE_MENU; 
                        }
                    } else {
                        precisa_carregar_fase = true;
                    }
                }
                // Camera
                camera.target = (Vector2){ jogador.x + jogador.width / 2.0f, jogador.y + jogador.height / 2.0f };
                break;

            case STATE_PAUSADO:
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcao_menu_pausa = (opcao_menu_pausa + 1) % 3;
                if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) opcao_menu_pausa = (opcao_menu_pausa + 2) % 3;

                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcao_menu_pausa == 0) { 
                        estado_atual = STATE_JOGANDO;
                    } 
                    else if (opcao_menu_pausa == 1) { 
                        estado_atual = STATE_MENU;
                    } 
                    else if (opcao_menu_pausa == 2) { 
                        CloseWindow();
                        return 0;
                    }
                }
                break;

            case STATE_RANKING:
                if (IsKeyPressed(KEY_ESCAPE)) {
                    estado_atual = STATE_MENU;
                }
                break;

            case STATE_SALVAR_RECORDE:
                tecla = GetCharPressed();
                while (tecla > 0) {
                    if ((tecla >= 32) && (tecla <= 125) && (letras_input < 19)) {
                        nome_input[letras_input] = (char)tecla;
                        nome_input[letras_input + 1] = '\0';
                        letras_input++;
                    }
                    tecla = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    letras_input--;
                    if (letras_input < 0) letras_input = 0;
                    nome_input[letras_input] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letras_input > 0) {
                    inserir_no_ranking(nome_input, tempo_final);
                    estado_atual = STATE_MENU; 
                }
                break;

            default:
                break;
        }

        
        BeginDrawing();
        ClearBackground(GRAY);

        if (estado_atual == STATE_JOGANDO || estado_atual == STATE_PAUSADO) {
            BeginMode2D(camera);
                
                // 1 Desenha as plataformas primeiro
                DesenharPlataformas(plataformas, qtd_plataformas);

                // 2 Desenha as escadas por cima das plataformas
                DesenharEscadas(escadas, qtd_escadas);
                
                // 3 Desenha os inimigos
                DesenharInimigos(inimigos, qtd_inimigos);

                // 4 Desenha a Porta
                DesenharPortal(portal_proxima_fase, precisa_carregar_fase);
                
                // 5 Desenha o personagem principal
                if (!precisa_carregar_fase) {
                    DesenharJogador(jogador, olhando_para_direita);
                }

            EndMode2D();

            char texto_tempo[30];
            sprintf(texto_tempo, "TEMPO: %d", (int)tempo_jogado);
            DrawText(texto_tempo, 20, 20, 30, BLACK);

            if (estado_atual == STATE_PAUSADO) {
                DesenharMenuPausa(opcao_menu_pausa);
            }
        } 
        else if (estado_atual == STATE_MENU) {
            DesenharMenuPrincipal(opcao_menu_principal);
        }
        else if (estado_atual == STATE_RANKING) {
            DesenharRanking();
        }
        else if (estado_atual == STATE_SALVAR_RECORDE) {
            DesenharTelaSalvarRecorde(nome_input, tempo_final);
        }

        EndDrawing();
    }

    // Descarrega todas as texturas usando o sistema modular que você criou
    DescarregarTexturasPersonagens();
    DescarregarTexturasObjetos();
    
    CloseWindow();
    return 0;
}