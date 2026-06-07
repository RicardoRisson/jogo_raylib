#include "mapa.h"
#include "objetos.h"
#include "movimentacao.h" // Garante o acesso à struct Plataforma e aos #define de tipos
#include <stdio.h>
#include <string.h>

// ARRUMADO: "Rectangle plataformas[]" virou "Plataforma plataformas[]"
void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, Escada escadas[], int *qtd_escadas, Vector2 *posicao_player) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (!f) {
        TraceLog(LOG_ERROR, "Nao foi possivel abrir o arquivo de mapa: %s", caminho_arquivo);
        return;
    }

    char linha_texto[MAX_COLUNAS + 5]; 
    int linha = 0;
    
    *qtd_plataformas = 0;
    *qtd_escadas = 0;

    posicao_player->x = 100.0f;
    posicao_player->y = 100.0f;

    while (fgets(linha_texto, sizeof(linha_texto), f) && linha < MAX_LINHAS) {
        linha_texto[strcspn(linha_texto, "\r\n")] = '\0';

        for (int coluna = 0; linha_texto[coluna] != '\0' && coluna < MAX_COLUNAS; coluna++) {
            float x = coluna * TILE_SIZE;
            float y = linha * TILE_SIZE;

            // Chão / Plataforma NORMAL
            if (linha_texto[coluna] == 'Z' || linha_texto[coluna] == 'z') {
                if (*qtd_plataformas < MAX_PLATAFORMAS) {
                    plataformas[*qtd_plataformas].rect = (Rectangle){ x, y, TILE_SIZE, TILE_SIZE };
                    plataformas[*qtd_plataformas].tipo = PLATAFORMA_NORMAL;
                    (*qtd_plataformas)++;
                } else {
                    TraceLog(LOG_WARNING, "Limite maximo de plataformas atingido (%d)!", MAX_PLATAFORMAS);
                }
            } 
            // PLATAFORMA QUE SOBE / TELEPORTA (Caractere 'S')
            else if (linha_texto[coluna] == 'S') {
                if (*qtd_plataformas < MAX_PLATAFORMAS) {
                    plataformas[*qtd_plataformas].rect = (Rectangle){ x, y, TILE_SIZE, TILE_SIZE };
                    plataformas[*qtd_plataformas].tipo = PLATAFORMA_SOBE;
                    (*qtd_plataformas)++;
                } else {
                    TraceLog(LOG_WARNING, "Limite maximo de plataformas atingido (%d)!", MAX_PLATAFORMAS);
                }
            }
            // PLATAFORMA QUE ATRAVESSA PARA BAIXO / DESCE (Caractere 'D')
            else if (linha_texto[coluna] == 'D') {
                if (*qtd_plataformas < MAX_PLATAFORMAS) {
                    plataformas[*qtd_plataformas].rect = (Rectangle){ x, y, TILE_SIZE, TILE_SIZE };
                    plataformas[*qtd_plataformas].tipo = PLATAFORMA_DESCE;
                    (*qtd_plataformas)++;
                } else {
                    TraceLog(LOG_WARNING, "Limite maximo de plataformas atingido (%d)!", MAX_PLATAFORMAS);
                }
            }
            // ESCADA NORMAL (H)
            else if (linha_texto[coluna] == 'H') {
                if (*qtd_escadas < MAX_ESCADAS) {
                    escadas[*qtd_escadas] = criar_escada(x, y, TILE_SIZE, TILE_SIZE, 0);
                    (*qtd_escadas)++;
                } else {
                    TraceLog(LOG_WARNING, "Limite maximo de escadas atingido (%d)!", MAX_ESCADAS);
                }
            }
            // Spawn do Player (P)
            else if (linha_texto[coluna] == 'P') {
                posicao_player->x = x;
                posicao_player->y = y;
                TraceLog(LOG_INFO, "Spawn do jogador encontrado em: X: %.1f, Y: %.1f", x, y);
            }
        }
        linha++;
    }

    fclose(f);
}