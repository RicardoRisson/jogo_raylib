#include "movimentacao.h"
#include <raylib.h>
#include <float.h>

static const float GRAVIDADE     = 0.5f; // quanto é puxado pra baixo, é oq feria o força pulo, TODO FRAME, entao é tipo x60
static const float FORCA_PULO    = -9.0f; // quao forte é o pulo num unico frame
static const float LIMITE_QUEDA  = 15.0f; // aceleraçao maxima

static float velocidade_y = 0.0f;
static bool esta_no_chao  = false;

// funçoes auxiliares e estaticas

static Rectangle mover_e_colidir_x(Rectangle jogador, float velocidade_x, Plataforma plataformas[], int quantidade_plataformas) {
    if (velocidade_x == 0.0f) return jogador;
    jogador.x += velocidade_x;

    for (int i = 0; i < quantidade_plataformas; i++) { //sempre checa todos
        if (CheckCollisionRecs(jogador, plataformas[i].rect)) { //colodiu com essa?
            if (velocidade_x > 0.0f) /* vinha da direita? */ jogador.x = plataformas[i].rect.x - jogador.width; //encosta na borda esquerda
            else if (velocidade_x < 0.0f) /* vinha da esquer
INFO: TEXTURE: [ID 7] Texture loaded successfully (1600x1600 | R8G8B8 | 1 mipmaps)da? */ jogador.x = plataformas[i].rect.x + plataformas[i].rect.width; //encosta na borda direita
        }
    }
    return jogador;
}

static Rectangle mover_e_colidir_y(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas) {
    velocidade_y += GRAVIDADE; // sempre aumenta
    if (velocidade_y > LIMITE_QUEDA) velocidade_y = LIMITE_QUEDA; // sempre move pra baixo

    jogador.y += velocidade_y; // aplica o movimento de "queda" no jogador
    esta_no_chao = false; // ve se ta no chao pra ver se pode pular

    for (int i = 0; i < quantidade_plataformas; i++) { // checa todas as lataformas
        if (CheckCollisionRecs(jogador, plataformas[i].rect)) { // colodiu com essa?
            if (velocidade_y > 0.0f) { // vinha de cima?
                jogador.y = plataformas[i].rect.y - jogador.height; // cola no topo
                velocidade_y = 0.0f; // zera a velocidade, para de descer
                esta_no_chao = true; // jogador fica no chao, pode pular
            } 
            else if (velocidade_y < 0.0f) { // vinha de baixo?
                jogador.y = plataformas[i].rect.y + plataformas[i].rect.height; //gruda em cima
                velocidade_y = 0.0f; //zara a velocidade, para de subir
            }
        }
    }
    return jogador;
}

static bool existe_caminho_de_escadas(float jogador_x, float y_inicio, float y_fim, Escada escadas[], int quantidade_escadas) {
    float passo = 50.0f; // intervalo de verificacao entre escadas
    float topo = (y_inicio < y_fim) ? y_inicio : y_fim; // mais acima da tela
    float fundo = (y_inicio > y_fim) ? y_inicio : y_fim; // mais abaixo da tela
    float checar_y = topo + (passo / 2.0f); //começa no meio pra nao bugar pegando a borda

    while (checar_y < fundo) { // percorre de cima ao baixo de 50px, oq equivale a 1 bloco
        bool encontrou = false; // assume que n tem escada
        for (int e = 0; e < quantidade_escadas; e++) { // checa todas as escadas
            if (jogador_x >= escadas[e].rect.x /* passou da borda esquerda? */ && jogador_x <= escadas[e].rect.x + escadas[e].rect.width /* nao passou da borda direita? */ && 
            checar_y >= escadas[e].rect.y /* passou da borda de cima? */ && checar_y <= escadas[e].rect.y + escadas[e].rect.height /* nao passou da borda de baixo? */) {
                encontrou = true; // entao achou escada
                break; // nao precisa checar mais
            }
        }
        if (!encontrou) return false; // se n encontrou, escada incompleta
        checar_y += passo; // avança 50px pra ir pro prox ponto
    }
    return true; //todos os pontos tinham escadas, escada completa
}

// --- Funções Públicas ---

Rectangle atualizar_movimento(Rectangle jogador, float velocidade) { // funçao legado nao usamos mais
    (void)velocidade;
    return jogador;     
}

Inimigo atualizar_movimento_inimigo(Inimigo inimigo, Plataforma plataformas[], int quantidade_plataformas) {
    float x_anterior = inimigo.hitbox.x; // guarda o x antes de mover, pra detectar colisao com parede depois
    bool colidiu_com_chao = false; // assume que nao ta no chao
    Rectangle sensor_chao; // retangulo pequeno na frente do inimigo pra detectar borda
    bool existe_chao_a_frente = false; // assume que nao tem chao a frente

    inimigo.hitbox = mover_e_colidir_x(inimigo.hitbox, inimigo.velocidade, plataformas, quantidade_plataformas); // move o inimigo horizontalmente e colide com paredes

    if (inimigo.hitbox.x == x_anterior) { // se o x nao mudou, bateu numa parede
        inimigo.velocidade *= -1.0f; // inverte a direcao
    }

    inimigo.hitbox.y += GRAVIDADE * 10.0f; // aplica queda (mais forte que o jogador pra nao flutuar)

    for (int i = 0; i < quantidade_plataformas; i++) { // checa todas as plataformas
        if (CheckCollisionRecs(inimigo.hitbox, plataformas[i].rect)) { // colidiu com o chao?
            inimigo.hitbox.y = plataformas[i].rect.y - inimigo.hitbox.height; // cola no topo da plataforma
            colidiu_com_chao = true; // confirma que ta no chao
            break; // nao precisa checar mais
        }
    }

    if (colidiu_com_chao) { // so verifica borda se estiver no chao
        sensor_chao.width = 10.0f; // largura do sensor
        sensor_chao.height = 5.0f; // altura do sensor
        sensor_chao.y = inimigo.hitbox.y + inimigo.hitbox.height + 2.0f; // posiciona logo abaixo do inimigo
        sensor_chao.x = (inimigo.velocidade > 0.0f) ? (inimigo.hitbox.x + inimigo.hitbox.width) : (inimigo.hitbox.x - sensor_chao.width); // posiciona na frente do inimigo, dependendo da direcao

        for (int i = 0; i < quantidade_plataformas; i++) { // checa todas as plataformas
            if (CheckCollisionRecs(sensor_chao, plataformas[i].rect)) { // tem chao a frente?
                existe_chao_a_frente = true; // confirmou que tem chao
                break; // nao precisa checar mais
            }
        }

        if (!existe_chao_a_frente) inimigo.velocidade *= -1.0f; // sem chao a frente, inverte direcao pra nao cair
    }
    return inimigo; // retorna o inimigo atualizado
}

// "O MONOLITO"
Rectangle verificar_chao_com_escadas(Rectangle jogador, Plataforma plataformas[], int quantidade_plataformas, Escada escadas[], int quantidade_escadas, float velocidade) {
    float velocidade_x = 0.0f; // velocidade horizontal começa zerada
    bool executou_acao_especial = false; // controla se usou escada, pra nao pular junto

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  velocidade_x -= velocidade; // move pra esquerda
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) velocidade_x += velocidade; // move pra direita

    for (int i = 0; i < quantidade_plataformas; i++) { // checa todas as plataformas
        bool colidindo = CheckCollisionRecs(jogador, plataformas[i].rect); // esta dentro da plataforma?
        bool em_cima = (jogador.x + jogador.width > plataformas[i].rect.x) && // passou da borda esquerda?
                       (jogador.x < plataformas[i].rect.x + plataformas[i].rect.width) && // nao passou da borda direita?
                       (jogador.y + jogador.height >= plataformas[i].rect.y - 2.0f) && // pe do jogador perto do topo?
                       (jogador.y + jogador.height <= plataformas[i].rect.y + 5.0f); // mas nao afundado demais?

        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && plataformas[i].tipo == PLATAFORMA_SOBE && (colidindo || em_cima)) { // apertou pra subir e esta numa plataforma de subida?
            int indice_plataforma_cima = -1; // indice da plataforma destino, -1 = nao achou ainda
            float menor_distancia_y = FLT_MAX; // guarda a menor distancia encontrada, comeca no maximo possivel

            for (int j = 0; j < quantidade_plataformas; j++) { // procura todas as plataformas
                if (i == j) continue; // ignora a propria plataforma
                if ((jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width) && // esta no mesmo x?
                    (plataformas[j].rect.y + plataformas[j].rect.height <= plataformas[i].rect.y)) { // esta acima da plataforma atual?
                    float dist = plataformas[i].rect.y - (plataformas[j].rect.y + plataformas[j].rect.height); // calcula distancia entre as duas
                    if (dist < menor_distancia_y) { menor_distancia_y = dist; indice_plataforma_cima = j; } // guarda a mais proxima
                }
            }
            if (indice_plataforma_cima != -1 && existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, plataformas[i].rect.y, plataformas[indice_plataforma_cima].rect.y + plataformas[indice_plataforma_cima].rect.height, escadas, quantidade_escadas)) { // achou plataforma acima e tem escada?
                jogador.y = plataformas[indice_plataforma_cima].rect.y - jogador.height; // teleporta pro andar de cima
                velocidade_y = 0.0f; esta_no_chao = true; executou_acao_especial = true; break; // zera velocidade, marca no chao e que usou escada
            }
        }

        if ((IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) && plataformas[i].tipo == PLATAFORMA_DESCE && em_cima) { // apertou pra descer e esta numa plataforma de descida?
            int indice_proxima = -1; // indice da plataforma destino, -1 = nao achou ainda
            float menor_distancia_y = 999999.0f; // guarda a menor distancia encontrada

            for (int j = 0; j < quantidade_plataformas; j++) { // procura todas as plataformas
                if (i == j) continue; // ignora a propria plataforma
                if ((jogador.x + jogador.width > plataformas[j].rect.x) && (jogador.x < plataformas[j].rect.x + plataformas[j].rect.width) && // esta no mesmo x?
                    (plataformas[j].rect.y >= plataformas[i].rect.y + plataformas[i].rect.height)) { // esta abaixo da plataforma atual?
                    float dist = plataformas[j].rect.y - (plataformas[i].rect.y + plataformas[i].rect.height); // calcula distancia entre as duas
                    if (dist < menor_distancia_y) { menor_distancia_y = dist; indice_proxima = j; } // guarda a mais proxima
                }
            }
            if (indice_proxima != -1 && existe_caminho_de_escadas(jogador.x + jogador.width/2.0f, plataformas[i].rect.y + plataformas[i].rect.height, plataformas[indice_proxima].rect.y, escadas, quantidade_escadas)) { // achou plataforma abaixo e tem escada?
                jogador.y = plataformas[indice_proxima].rect.y - jogador.height; // teleporta pro andar de baixo
                velocidade_y = 0.0f; esta_no_chao = true; executou_acao_especial = true; break; // zera velocidade, marca no chao e que usou escada
            }
        }
    }

    if (!executou_acao_especial && (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE)) && esta_no_chao) { // nao usou escada, apertou pular e esta no chao?
        velocidade_y = FORCA_PULO; esta_no_chao = false; // aplica forca do pulo e sai do chao
    }

    jogador = mover_e_colidir_x(jogador, velocidade_x, plataformas, quantidade_plataformas); // aplica movimento horizontal com colisao
    jogador = mover_e_colidir_y(jogador, plataformas, quantidade_plataformas); // aplica gravidade e movimento vertical com colisao
    return jogador; // retorna jogador atualizado
}