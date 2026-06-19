#include "mapa.h"
#include "objetos.h"
#include "personagens.h"
#include <stdio.h>
#include <string.h>

void carregar_mapa(const char *caminho_arquivo, Plataforma plataformas[], int *qtd_plataformas, Escada escadas[], int *qtd_escadas, Vector2 *posicao_player, Rectangle *portal, Inimigo inimigos[], int *qtd_inimigos) {
    FILE *f = fopen(caminho_arquivo, "r"); // abre o arquivo de texto do mapa
    if (!f) return; // se nao abriu, desiste

    char linha_texto[MAX_COLUNAS + 5]; // buffer pra guardar uma linha do arquivo
    int linha = 0; // qual linha do mapa estamos lendo

    // zera tudo antes de comecar
    *qtd_plataformas = 0;
    *qtd_escadas = 0;
    *qtd_inimigos = 0;
    *portal = (Rectangle){ 0, 0, 0, 0 }; // portal vazio por padrao
    posicao_player->x = 100.0f; // posicao padrao do jogador caso nao tenha 'P' no mapa
    posicao_player->y = 100.0f;

    while (fgets(linha_texto, sizeof(linha_texto), f) && linha < MAX_LINHAS) { // le linha por linha ate acabar o arquivo ou o mapa
        linha_texto[strcspn(linha_texto, "\r\n")] = '\0'; // remove o \n ou \r do final da linha

        for (int coluna = 0; linha_texto[coluna] != '\0' && coluna < MAX_COLUNAS; coluna++) { // percorre cada caractere da linha
            float x = coluna * TILE_SIZE; // converte coluna em posicao x na tela
            float y = linha * TILE_SIZE;  // converte linha em posicao y na tela

            switch(linha_texto[coluna]) { // qual caractere e esse?
                case 'Z':
                case 'z': // plataforma normal (maiuscula ou minuscula)
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_NORMAL};
                        (*qtd_plataformas)++; // incrementa o contador
                    }
                    break;

                case 'S': // plataforma que permite subir escada
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_SOBE};
                        (*qtd_plataformas)++;
                    }
                    break;

                case 'D': // plataforma que permite descer escada
                    if (*qtd_plataformas < MAX_PLATAFORMAS) {
                        plataformas[*qtd_plataformas] = (Plataforma){{x, y, TILE_SIZE, TILE_SIZE}, PLATAFORMA_DESCE};
                        (*qtd_plataformas)++;
                    }
                    break;

                case 'H': // escada
                    if (*qtd_escadas < MAX_ESCADAS) {
                        escadas[*qtd_escadas] = criar_escada(x, y, TILE_SIZE, TILE_SIZE, 0);
                        (*qtd_escadas)++;
                    }
                    break;

                case 'P': // posicao inicial do jogador
                    posicao_player->x = x;
                    posicao_player->y = y;
                    break;

                case 'F':
                case 'f': // portal de saida do nivel (maiuscula ou minuscula)
                    *portal = criar_portal(x, y, TILE_SIZE, TILE_SIZE);
                    break;

                case 'E': // inimigo normal
                    if (*qtd_inimigos < MAX_INIMIGOS) {
                        inimigos[*qtd_inimigos] = criar_inimigo_normal(x, y, 40, 40, 2.0f);
                        (*qtd_inimigos)++;
                    }
                    break;

                case 'R': // inimigo rapido
                    if (*qtd_inimigos < MAX_INIMIGOS) {
                        inimigos[*qtd_inimigos] = criar_inimigo_rapido(x, y, 40, 40, 2.0f);
                        (*qtd_inimigos)++;
                    }
                    break;
            }
        }
        linha++; // proxima linha do arquivo = proxima linha do mapa
    }
    fclose(f); // fecha o arquivo
}