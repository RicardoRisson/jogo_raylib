#include "mapa.h"
#include "objetos.h"
#include "personagens.h"
#include <stdio.h>
#include <string.h>

void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, Escada escadas[], int *qtd_escadas, Vector2 *posicao_player, Rectangle *portal, Inimigo inimigos[], int *qtd_inimigos) {
    FILE *f = fopen(caminho_arquivo, "r");
    if (!f) return;

    char linha_texto[MAX_COLUNAS + 5];
    int linha = 0;

    *qtd_plataformas = 0;
    *qtd_escadas = 0;
    *qtd_inimigos = 0;

    *portal = (Rectangle){ 0, 0, 0, 0 };
    posicao_player->x = 100.0f;
    posicao_player->y = 100.0f;

    while (fgets(linha_texto, sizeof(linha_texto), f) && linha < MAX_LINHAS) {
        linha_texto[strcspn(linha_texto, "\r\n")] = '\0';

        for (int coluna = 0; linha_texto[coluna] != '\0' && coluna < MAX_COLUNAS; coluna++) {
            float x = coluna * TILE_SIZE;
            float y = linha * TILE_SIZE;

            switch(linha_texto[coluna]) {
                case 'Z':
                case 'z':
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_NORMAL};
                        (*qtd_plataformas)++;
                    }
                    break;
                case 'S':
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_SOBE};
                        (*qtd_plataformas)++;
                    }
                    break;
                case 'D':
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_DESCE};
                        (*qtd_plataformas)++;
                    }
                    break;
                case 'H':
                    if (*qtd_escadas < MAX_ESCADAS) {
                        escadas[*qtd_escadas] = criar_escada(x, y, TILE_SIZE, TILE_SIZE, 0);
                        (*qtd_escadas)++;
                    }
                    break;
                case 'P':
                    posicao_player->x = x;
                    posicao_player->y = y;
                    break;
                case 'F':
                case 'f':
                    *portal = criar_portal(x, y, TILE_SIZE, TILE_SIZE);
                    break;
                case 'E':
                    if (*qtd_inimigos < MAX_INIMIGOS) {
                        inimigos[*qtd_inimigos] = criar_inimigo_normal(x, y, 40, 40, 2.0f);
                        (*qtd_inimigos)++;
                    }
                    break;
                case 'R':
                    if (*qtd_inimigos < MAX_INIMIGOS) {
                        inimigos[*qtd_inimigos] = criar_inimigo_rapido(x, y, 40, 40, 2.0f);
                        (*qtd_inimigos)++;
                    }
                    break;
            }
        }
        linha++;
    }
    fclose(f);
}