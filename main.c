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

#define JOGADOR_LARGURA 30  // largura da hitbox do jogador
#define JOGADOR_ALTURA  40  // altura da hitbox do jogador
#define SPRITE_LARGURA  50  // largura do sprite visual (maior que a hitbox)
#define SPRITE_ALTURA   50  // altura do sprite visual (maior que a hitbox)

#define JANELA_TITULO   "ninja contra os ninjas verdes"

int main(void)
{
    InitWindow(JANELA_LARGURA, JANELA_ALTURA, JANELA_TITULO); // cria a janela
    SetTargetFPS(60); // limita o jogo a 60 frames por segundo

    SetExitKey(KEY_NULL); // impede que ESC feche a janela automaticamente

    inicializar_placar_vazio(); // garante que o arquivo de placar exista antes de qualquer leitura

    GameState estado_atual = STATE_MENU; // comeca no menu principal
    int opcao_menu_principal = 0; // opcao selecionada no menu principal
    int opcao_menu_pausa = 0;     // opcao selecionada no menu de pausa

    char nome_input[20] = "\0"; // nome digitado pelo jogador ao salvar recorde
    int letras_input = 0;       // quantas letras foram digitadas
    int tecla = 0;              // tecla capturada no input de nome

    float tempo_jogado = 0.0f; // cronometro em segundos, acumula durante o jogo
    int tempo_final = 0;       // tempo convertido pra inteiro ao terminar o jogo

    float cronometro_queda = 0.0f; // acumula quantos segundos o jogador esta caindo
    float y_ultimo_frame = 0.0f;   // posicao Y do jogador no frame anterior

    CarregarTexturasPersonagens(); // carrega sprites do jogador e inimigos
    CarregarTexturasObjetos();     // carrega sprites das plataformas, escadas e portal
    bool olhando_para_direita = true; // controla qual sprite do jogador usar

    float velocidade_jogador = 5.0f; // velocidade horizontal do jogador em pixels por frame

    Plataforma plataformas[200]; // array de plataformas do mapa atual
    int qtd_plataformas = 0;
    Escada escadas[MAX_ESCADAS]; // array de escadas do mapa atual
    int qtd_escadas = 0;
    Inimigo inimigos[MAX_INIMIGOS]; // array de inimigos do mapa atual
    int qtd_inimigos = 0;

    Vector2 spawn_jogador;                     // posicao inicial do jogador lida do mapa
    char fase_atual = 'A';                     // fase atual, vai de A ate C
    char nome_arquivo[20];                     // nome do arquivo do mapa ex: mapa_A.txt
    Rectangle portal_proxima_fase = { 0 };    // hitbox do portal de saida da fase
    bool precisa_carregar_fase = true;         // flag que dispara o carregamento do mapa
    Rectangle jogador;                         // hitbox do jogador

    Camera2D camera = { 0 };
    camera.offset   = (Vector2){ JANELA_LARGURA / 2.0f, JANELA_ALTURA / 2.0f }; // centraliza a camera na tela
    camera.rotation = 0.0f; // sem rotacao
    camera.zoom     = 1.0f; // sem zoom

    while (!WindowShouldClose()) // loop principal, roda ate fechar a janela
    {
        AtualizarBotoesInterface(&estado_atual, &opcao_menu_pausa); // checa ESC pra pausar/despausar

        switch (estado_atual) {
            case STATE_MENU:
                // navega entre as opcoes do menu
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcao_menu_principal = (opcao_menu_principal + 1) % 3;
                if (IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_W)) opcao_menu_principal = (opcao_menu_principal + 2) % 3;

                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcao_menu_principal == 0) { // novo jogo
                        fase_atual           = 'A';
                        precisa_carregar_fase = true;
                        tempo_jogado         = 0.0f;
                        estado_atual         = STATE_JOGANDO;
                    }
                    else if (opcao_menu_principal == 1) { // ranking
                        estado_atual = STATE_RANKING;
                    }
                    else if (opcao_menu_principal == 2) { // sair
                        CloseWindow();
                        return 0;
                    }
                }
                break;

            case STATE_JOGANDO:
                tempo_jogado += GetFrameTime(); // acumula o tempo jogado em segundos

                if (precisa_carregar_fase) { // carrega o mapa da fase atual
                    sprintf(nome_arquivo, "mapa_%c.txt", fase_atual); // monta o nome do arquivo ex: mapa_A.txt
                    carregar_mapa(nome_arquivo, plataformas, &qtd_plataformas, escadas, &qtd_escadas, &spawn_jogador, &portal_proxima_fase, inimigos, &qtd_inimigos);
                    jogador              = criar_jogador(spawn_jogador.x, spawn_jogador.y, JOGADOR_LARGURA, JOGADOR_ALTURA);
                    precisa_carregar_fase = false;
                    cronometro_queda     = 0.0f;
                    y_ultimo_frame       = jogador.y;
                }

                // atualiza a direcao que o jogador esta olhando
                if      (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) olhando_para_direita = true;
                else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  olhando_para_direita = false;

                y_ultimo_frame = jogador.y; // guarda o Y antes de mover pra comparar depois

                jogador = verificar_chao_com_escadas(jogador, plataformas, qtd_plataformas, escadas, qtd_escadas, velocidade_jogador); // aplica toda a fisica e input do jogador

                if (jogador.y > y_ultimo_frame) { // Y aumentou = jogador esta descendo/caindo
                    cronometro_queda += GetFrameTime(); // acumula tempo de queda
                } else {
                    cronometro_queda = 0.0f; // parou de cair, reseta o cronometro
                }

                if (cronometro_queda >= 2.0f) { // caiu por mais de 2 segundos = buraco sem fundo
                    Rectangle inimigo_invisivel = { jogador.x, jogador.y + jogador.height - 5.0f, jogador.width, 10.0f }; // hitbox invisivel no pe do jogador

                    if (CheckCollisionRecs(jogador, inimigo_invisivel)) { // colisao consigo mesmo = morte
                        fase_atual            = 'A';
                        precisa_carregar_fase  = true;
                        tempo_jogado          = 0.0f;
                        cronometro_queda      = 0.0f;
                        break;
                    }
                }

                for (int i = 0; i < qtd_inimigos; i++) {
                    inimigos[i] = atualizar_movimento_inimigo(inimigos[i], plataformas, qtd_plataformas); // move o inimigo

                    if (CheckCollisionRecs(jogador, inimigos[i].hitbox)) { // jogador tocou inimigo = morte
                        fase_atual            = 'A';
                        precisa_carregar_fase  = true;
                        tempo_jogado          = 0.0f;
                        break;
                    }
                }

                if (!precisa_carregar_fase && CheckCollisionRecs(jogador, portal_proxima_fase)) { // jogador entrou no portal?
                    fase_atual++;

                    if (fase_atual > 'C') { // passou da ultima fase = jogo completo
                        tempo_final = (int)tempo_jogado;

                        if (verificar_se_entra_no_ranking(tempo_final)) { // tempo bom o suficiente pro ranking?
                            estado_atual      = STATE_SALVAR_RECORDE;
                            nome_input[0]     = '\0';
                            letras_input      = 0;
                        } else {
                            estado_atual = STATE_MENU; // nao entrou no ranking, volta pro menu
                        }
                    } else {
                        precisa_carregar_fase = true; // carrega a proxima fase
                    }
                }

                camera.target = (Vector2){ jogador.x + jogador.width / 2.0f, jogador.y + jogador.height / 2.0f }; // camera segue o centro do jogador
                break;

            case STATE_PAUSADO:
                // navega entre as opcoes do menu de pausa
                if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) opcao_menu_pausa = (opcao_menu_pausa + 1) % 3;
                if (IsKeyPressed(KEY_UP)   || IsKeyPressed(KEY_W)) opcao_menu_pausa = (opcao_menu_pausa + 2) % 3;

                if (IsKeyPressed(KEY_ENTER)) {
                    if (opcao_menu_pausa == 0)      estado_atual = STATE_JOGANDO; // continuar
                    else if (opcao_menu_pausa == 1) estado_atual = STATE_MENU;    // voltar ao menu
                    else if (opcao_menu_pausa == 2) { CloseWindow(); return 0; }  // sair
                }
                break;

            case STATE_RANKING:
                if (IsKeyPressed(KEY_ESCAPE)) estado_atual = STATE_MENU; // ESC volta pro menu
                break;

            case STATE_SALVAR_RECORDE:
                tecla = GetCharPressed(); // pega a primeira tecla digitada na fila
                while (tecla > 0) { // processa todas as teclas digitadas nesse frame
                    if ((tecla >= 32) && (tecla <= 125) && (letras_input < 19)) { // caractere valido e nome nao cheio?
                        nome_input[letras_input]     = (char)tecla;
                        nome_input[letras_input + 1] = '\0'; // mantem o \0 no final
                        letras_input++;
                    }
                    tecla = GetCharPressed(); // pega a proxima tecla da fila
                }

                if (IsKeyPressed(KEY_BACKSPACE)) { // apaga a ultima letra
                    letras_input--;
                    if (letras_input < 0) letras_input = 0; // nao deixa ficar negativo
                    nome_input[letras_input] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER) && letras_input > 0) { // confirmou o nome?
                    inserir_no_ranking(nome_input, tempo_final); // salva no placar
                    estado_atual = STATE_MENU;
                }
                break;

            default:
                break;
        }

        BeginDrawing();
        ClearBackground(GRAY); // limpa a tela com cinza antes de desenhar tudo

        if (estado_atual == STATE_JOGANDO || estado_atual == STATE_PAUSADO) {
            BeginMode2D(camera); // tudo dentro aqui e desenhado com a camera (move com o jogador)

                DesenharPlataformas(plataformas, qtd_plataformas); // 1 - fundo
                DesenharEscadas(escadas, qtd_escadas);              // 2 - sobre as plataformas
                DesenharInimigos(inimigos, qtd_inimigos);           // 3 - sobre as escadas
                DesenharPortal(portal_proxima_fase, precisa_carregar_fase); // 4 - portal de saida
                if (!precisa_carregar_fase) DesenharJogador(jogador, olhando_para_direita); // 5 - jogador por cima de tudo

            EndMode2D(); // a partir daqui e desenhado fixo na tela (HUD)

            char texto_tempo[30];
            sprintf(texto_tempo, "TEMPO: %d", (int)tempo_jogado);
            DrawText(texto_tempo, 20, 20, 30, BLACK); // cronometro fixo no canto superior esquerdo

            if (estado_atual == STATE_PAUSADO) DesenharMenuPausa(opcao_menu_pausa); // overlay de pausa por cima do jogo
        }
        else if (estado_atual == STATE_MENU)          DesenharMenuPrincipal(opcao_menu_principal);
        else if (estado_atual == STATE_RANKING)        DesenharRanking();
        else if (estado_atual == STATE_SALVAR_RECORDE) DesenharTelaSalvarRecorde(nome_input, tempo_final);

        EndDrawing(); // envia tudo pra tela
    }

    DescarregarTexturasPersonagens(); // libera memoria das texturas dos personagens
    DescarregarTexturasObjetos();     // libera memoria das texturas dos objetos

    CloseWindow(); // fecha a janela
    return 0;
}