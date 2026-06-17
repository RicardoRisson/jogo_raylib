#ifndef OBJETOS_H
#define OBJETOS_H

#include "raylib.h"

#define MAX_ESCADAS 50

// Macros de tipo de plataforma que o mapa.c e movimentacao.c precisam
#define PLATAFORMA_NORMAL 0
#define PLATAFORMA_SOBE   1
#define PLATAFORMA_DESCE  2

typedef struct {
    Rectangle rect;
    int tipo; // 0 = Normal, 1 = Sobe, 2 = Desce
} Escada;

typedef struct {
    Rectangle rect;
    int tipo; // 0 = Normal, 1 = Sobe, 2 = Desce
} Plataforma;

// Ciclo de vida das texturas
void CarregarTexturasObjetos(void);
void DescarregarTexturasObjetos(void);

// Funções de criação
Rectangle criar_plataforma(float x, float y, float largura, float altura);
Escada criar_escada(float x, float y, float largura, float altura, int tipo);
Rectangle criar_portal(float x, float y, float largura, float altura);

// Funções de renderização
void DesenharPlataformas(Plataforma* plataformas, int qtd_plataformas);
void DesenharEscadas(Escada* escadas, int qtd_escadas);
void DesenharPortal(Rectangle portal, bool precisa_carregar_fase);

#endif