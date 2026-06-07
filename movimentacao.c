#include "movimentacao.h"
#include <raylib.h>

static const float GRAVIDADE     = 0.5f;
static const float FORCA_PULO    = -11.0f;
static const float LIMITE_QUEDA  = 15.0f;
static const float DISTANCIA_TELEPORTE = 150.0f;

static float velocidade_y = 0.0f;
static bool esta_no_chao  = false;

static Rectangle mover_e_colidir_x(Rectangle jogador, float velocidade_x, Plataforma plataformas[], int quantidade_plataformas) {
    if (velocidade_x == 0.0f) return jogador;
    jogador.x += velocidade_x;

    for (int i = 0; i < quantidade_plataformas; i++) {
        if (CheckCollisionRecs(jogador, plataformas[i].rect)) {
            if (velocidade_x > 0.0f) jogador.x = plataformas[i].rect.x - jogador.width;
            else if (velocidade_x < 0.0f) jogador.x = plataformas[i].rect.x + plataformas[i].rect.width;
        }
    }
    return jogador;
}

static Rectangle mover_e_colidir_y(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas) {
    velocidade_y += GRAVIDADE;
    if (velocidade_y > LIMITE_QUEDA) velocidade_y = LIMITE_QUEDA;

    jogador.y += velocidade_y;
    esta_no_chao = false;

    for (int i = 0; i < quantidade_plataformas; i++) {
        if (CheckCollisionRecs(jogador, plataformas[i].rect)) {
            if (velocidade_y > 0.0f) {
                jogador.y = plataformas[i].rect.y - jogador.height;
                velocidade_y = 0.0f;
                esta_no_chao = true;
            } 
            else if (velocidade_y < 0.0f) {
                jogador.y = plataformas[i].rect.y + plataformas[i].rect.height;
                velocidade_y = 0.0f;
            }
        }
    }
    return jogador;
}

Rectangle atualizar_movimento(Rectangle jogador, float velocidade) {
    (void)velocidade;
    return jogador;
}

// --- FUNÇÃO AUXILIAR DE VALIDAÇÃO ---
// Verifica se existe um caminho contínuo de escadas entre duas alturas Y na mesma posição X do jogador
static bool existe_caminho_de_escadas(float jogador_x, float y_inicio, float y_fim, Escada escadas[], int quantidade_escadas) {
    float passo = 50.0f; // Tamanho do seu TILE_SIZE
    
    // Garante que a varredura vai do ponto mais alto para o mais baixo
    float topo = (y_inicio < y_fim) ? y_inicio : y_fim;
    float fundo = (y_inicio > y_fim) ? y_inicio : y_fim;

    // Varre cada bloco vertical entre as duas plataformas
    for (float checar_y = topo + passo; checar_y < fundo; checar_y += passo) {
        bool encontrou_escada_neste_bloco = false;

        for (int e = 0; e < quantidade_escadas; e++) {
            // Verifica se a escada está na mesma coluna do jogador (X) e na linha atual da varredura (Y)
            if (jogador_x >= escadas[e].rect.x && jogador_x <= escadas[e].rect.x + escadas[e].rect.width - 1.0f) {
                // Margem pequena para evitar problemas de arredondamento de float
                if (checar_y >= escadas[e].rect.y - 5.0f && checar_y <= escadas[e].rect.y + 5.0f) {
                    encontrou_escada_neste_bloco = true;
                    break;
                }
            }
        }

        // Se um único bloco do caminho não tiver escada, a conexão está quebrada!
        if (!encontrou_escada_neste_bloco) {
            return false;
        }
    }
    return true; // Caminho 100% conectado por escadas H
}


Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade) {
    float velocidade_x = 0.0f;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  velocidade_x -= velocidade;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocidade_x += velocidade;

    bool executou_acao_especial = false;

    for (int i = 0; i < quantidade_plataformas; i++) {
        bool colidindo = CheckCollisionRecs(jogador, plataformas[i].rect);
        
        bool em_cima = (jogador.x + jogador.width > plataformas[i].rect.x) &&
                       (jogador.x < plataformas[i].rect.x + plataformas[i].rect.width) &&
                       (jogador.y + jogador.height >= plataformas[i].rect.y - 2.0f) &&
                       (jogador.y + jogador.height <= plataformas[i].rect.y + 5.0f);

        // --- SUBIDA (W) COM VALIDAÇÃO DE ESCADAS ---
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && plataformas[i].tipo == PLATAFORMA_SOBE && (colidindo || em_cima)) {
            int indice_plataforma_cima = -1;
            float menor_distancia_y = 999999.0f;

            // Procura a plataforma acima
            for (int j = 0; j < quantidade_plataformas; j++) {
                if (i == j) continue;
                bool alinhado_x = (jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width);
                bool acima_y = plataformas[j].rect.y + plataformas[j].rect.height <= plataformas[i].rect.y;

                if (alinhado_x && acima_y) {
                    float distancia = plataformas[i].rect.y - (plataformas[j].rect.y + plataformas[j].rect.height);
                    if (distancia < menor_distancia_y) {
                        menor_distancia_y = distancia;
                        indice_plataforma_cima = j;
                    }
                }
            }

            // Se achou uma em cima, valida se há escadas H sem intervalos ligando as duas
            if (indice_plataforma_cima != -1) {
                float y_origem = plataformas[i].rect.y;
                float y_destino = plataformas[indice_plataforma_cima].rect.y + plataformas[indice_plataforma_cima].rect.height;

                if (existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, y_origem, y_destino, escadas, quantidade_escadas)) {
                    jogador.y = plataformas[indice_plataforma_cima].rect.y - jogador.height;
                    velocidade_y = 0.0f;
                    esta_no_chao = true;
                    executou_acao_especial = true;
                    break;
                }
            }
        }

        // --- DESCIDA (S) COM VALIDAÇÃO DE ESCADAS ---
        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && plataformas[i].tipo == PLATAFORMA_DESCE && em_cima) {
            int indice_proxima_plataforma = -1;
            float menor_distancia_y = 999999.0f;

            // Procura a plataforma abaixo
            for (int j = 0; j < quantidade_plataformas; j++) {
                if (i == j) continue;
                bool alinhado_x = (jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width);
                bool abaixo_y = plataformas[j].rect.y >= plataformas[i].rect.y + plataformas[i].rect.height;

                if (alinhado_x && abaixo_y) {
                    float distancia = plataformas[j].rect.y - (plataformas[i].rect.y + plataformas[i].rect.height);
                    if (distancia < menor_distancia_y) {
                        menor_distancia_y = distancia;
                        indice_proxima_plataforma = j;
                    }
                }
            }

            // Se achou uma embaixo, valida se há escadas H sem intervalos ligando as duas
            if (indice_proxima_plataforma != -1) {
                float y_origem = plataformas[i].rect.y + plataformas[i].rect.height;
                float y_destino = plataformas[indice_proxima_plataforma].rect.y;

                if (existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, y_origem, y_destino, escadas, quantidade_escadas)) {
                    jogador.y = plataformas[indice_proxima_plataforma].rect.y - jogador.height;
                    velocidade_y = 0.0f;
                    esta_no_chao = true; 
                    executou_acao_especial = true;
                    break;
                }
            }
        }
    }

    if (!executou_acao_especial && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && esta_no_chao) {
        velocidade_y = FORCA_PULO;
        esta_no_chao = false;
    }

    jogador = mover_e_colidir_x(jogador, velocidade_x, plataformas, quantidade_plataformas);
    jogador = mover_e_colidir_y(jogador, plataformas, quantidade_plataformas);

    return jogador;
}