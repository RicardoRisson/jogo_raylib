#include "mapa.h"
#include "objetos.h"
#include "movimentacao.h" 
#include <stdio.h>
#include <string.h>

void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, Escada escadas[], int *qtd_escadas, Vector2 *posicao_player, Rectangle *portal) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (!f) {
        return;
    }

    char linha_texto[MAX_COLUNAS + 5]; 
    int linha = 0;
    
    *qtd_plataformas = 0;
    *qtd_escadas = 0;

    // Reseta o portal para garantir que ele não herde lixo de memória da fase anterior
    *portal = (Rectangle){ 0, 0, 0, 0 };

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
                }
            } 
            // PLATAFORMA QUE SOBE / TELEPORTA (Caractere 'S')
            else if (linha_texto[coluna] == 'S') {
                if (*qtd_plataformas < MAX_PLATAFORMAS) {
                    plataformas[*qtd_plataformas].rect = (Rectangle){ x, y, TILE_SIZE, TILE_SIZE };
                    plataformas[*qtd_plataformas].tipo = PLATAFORMA_SOBE;
                    (*qtd_plataformas)++;
                }
            }
            // PLATAFORMA QUE ATRAVESSA PARA BAIXO / DESCE (Caractere 'D')
            else if (linha_texto[coluna] == 'D') {
                if (*qtd_plataformas < MAX_PLATAFORMAS) {
                    plataformas[*qtd_plataformas].rect = (Rectangle){ x, y, TILE_SIZE, TILE_SIZE };
                    plataformas[*qtd_plataformas].tipo = PLATAFORMA_DESCE;
                    (*qtd_plataformas)++;
                } 
            }
            // ESCADA NORMAL (H)
            else if (linha_texto[coluna] == 'H') {
                if (*qtd_escadas < MAX_ESCADAS) {
                    escadas[*qtd_escadas] = criar_escada(x, y, TILE_SIZE, TILE_SIZE, 0);
                    (*qtd_escadas)++;
                } 
            }
            // Spawn do Player (P)
            else if (linha_texto[coluna] == 'P') {
                posicao_player->x = x;
                posicao_player->y = y;
            }
            else if (linha_texto[coluna] == 'F' || linha_texto[coluna] == 'f') {
                *portal = criar_portal(x, y, TILE_SIZE, TILE_SIZE);
            }
        }
        linha++;
    }

    fclose(f);
}