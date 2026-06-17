#include "movimentacao.h"
#include <raylib.h>
#include <float.h>

static const float GRAVIDADE     = 0.5f;
static const float FORCA_PULO    = -11.0f;
static const float LIMITE_QUEDA  = 15.0f;

static float velocidade_y = 0.0f;
static bool esta_no_chao  = false;

// funçoes auxiliares e estaticas

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

static bool existe_caminho_de_escadas(float jogador_x, float y_inicio, float y_fim, Escada escadas[], int quantidade_escadas) {
    float passo = 50.0f;
    float topo = (y_inicio < y_fim) ? y_inicio : y_fim;
    float fundo = (y_inicio > y_fim) ? y_inicio : y_fim;
    float checar_y = topo + (passo / 2.0f); 

    while (checar_y < fundo) {
        bool encontrou = false;
        for (int e = 0; e < quantidade_escadas; e++) {
            if (jogador_x >= escadas[e].rect.x && jogador_x <= escadas[e].rect.x + escadas[e].rect.width &&
                checar_y >= escadas[e].rect.y && checar_y <= escadas[e].rect.y + escadas[e].rect.height) {
                encontrou = true;
                break;
            }
        }
        if (!encontrou) return false;
        checar_y += passo;
    }
    return true;
}

// --- Funções Públicas ---

Rectangle atualizar_movimento(Rectangle jogador, float velocidade) {
    (void)velocidade;
    return jogador;
}

Inimigo atualizar_movimento_inimigo(Inimigo inimigo, Plataforma plataformas[], int quantidade_plataformas) {
    float x_anterior = inimigo.hitbox.x;
    bool colidiu_com_chao = false;
    Rectangle sensor_chao;
    bool existe_chao_a_frente = false;
    
    inimigo.hitbox = mover_e_colidir_x(inimigo.hitbox, inimigo.velocidade, plataformas, quantidade_plataformas);
    
    if (inimigo.hitbox.x == x_anterior) {
        inimigo.velocidade *= -1.0f;
    }

    inimigo.hitbox.y += GRAVIDADE * 10.0f;
    
    for (int i = 0; i < quantidade_plataformas; i++) {
        if (CheckCollisionRecs(inimigo.hitbox, plataformas[i].rect)) {
            inimigo.hitbox.y = plataformas[i].rect.y - inimigo.hitbox.height;
            colidiu_com_chao = true;
            break;
        }
    }

    if (colidiu_com_chao) {
        sensor_chao.width = 10.0f;
        sensor_chao.height = 5.0f;
        sensor_chao.y = inimigo.hitbox.y + inimigo.hitbox.height + 2.0f;
        sensor_chao.x = (inimigo.velocidade > 0.0f) ? (inimigo.hitbox.x + inimigo.hitbox.width) : (inimigo.hitbox.x - sensor_chao.width);

        for (int i = 0; i < quantidade_plataformas; i++) {
            if (CheckCollisionRecs(sensor_chao, plataformas[i].rect)) {
                existe_chao_a_frente = true;
                break;
            }
        }

        if (!existe_chao_a_frente) inimigo.velocidade *= -1.0f;
    }
    return inimigo;
}

Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade) {
    float velocidade_x = 0.0f;
    bool executou_acao_especial = false;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  velocidade_x -= velocidade;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocidade_x += velocidade;

    for (int i = 0; i < quantidade_plataformas; i++) {
        bool colidindo = CheckCollisionRecs(jogador, plataformas[i].rect);
        bool em_cima = (jogador.x + jogador.width > plataformas[i].rect.x) &&
                       (jogador.x < plataformas[i].rect.x + plataformas[i].rect.width) &&
                       (jogador.y + jogador.height >= plataformas[i].rect.y - 2.0f) &&
                       (jogador.y + jogador.height <= plataformas[i].rect.y + 5.0f);

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && plataformas[i].tipo == PLATAFORMA_SOBE && (colidindo || em_cima)) {
            int indice_plataforma_cima = -1;
            float menor_distancia_y = FLT_MAX;

            for (int j = 0; j < quantidade_plataformas; j++) {
                if (i == j) continue;
                if ((jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width) &&
                    (plataformas[j].rect.y + plataformas[j].rect.height <= plataformas[i].rect.y)) {
                    float dist = plataformas[i].rect.y - (plataformas[j].rect.y + plataformas[j].rect.height);
                    if (dist < menor_distancia_y) { menor_distancia_y = dist; indice_plataforma_cima = j; }
                }
            }
            if (indice_plataforma_cima != -1 && existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, plataformas[i].rect.y, plataformas[indice_plataforma_cima].rect.y + plataformas[indice_plataforma_cima].rect.height, escadas, quantidade_escadas)) {
                jogador.y = plataformas[indice_plataforma_cima].rect.y - jogador.height;
                velocidade_y = 0.0f; esta_no_chao = true; executou_acao_especial = true; break;
            }
        }

        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && plataformas[i].tipo == PLATAFORMA_DESCE && em_cima) {
            int indice_proxima = -1;
            float menor_distancia_y = 999999.0f;

            for (int j = 0; j < quantidade_plataformas; j++) {
                if (i == j) continue;
                if ((jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width) &&
                    (plataformas[j].rect.y >= plataformas[i].rect.y + plataformas[i].rect.height)) {
                    float dist = plataformas[j].rect.y - (plataformas[i].rect.y + plataformas[i].rect.height);
                    if (dist < menor_distancia_y) { menor_distancia_y = dist; indice_proxima = j; }
                }
            }
            if (indice_proxima != -1 && existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, plataformas[i].rect.y + plataformas[i].rect.height, plataformas[indice_proxima].rect.y, escadas, quantidade_escadas)) {
                jogador.y = plataformas[indice_proxima].rect.y - jogador.height;
                velocidade_y = 0.0f; esta_no_chao = true; executou_acao_especial = true; break;
            }
        }
    }

    if (!executou_acao_especial && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && esta_no_chao) {
        velocidade_y = FORCA_PULO; esta_no_chao = false;
    }

    jogador = mover_e_colidir_x(jogador, velocidade_x, plataformas, quantidade_plataformas);
    jogador = mover_e_colidir_y(jogador, plataformas, quantidade_plataformas);
    return jogador;
}