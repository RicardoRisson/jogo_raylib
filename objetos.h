#ifndef OBJETOS_H
#define OBJETOS_H

#include "raylib.h" 

typedef struct {
    Rectangle rect;
    int tipo; // 0 = Normal, 1 = Subir, 2 = Descer
} Escada;

// Declaração das funções
Rectangle criar_plataforma(float x, float y, float largura, float altura);
Escada criar_escada(float x, float y, float largura, float altura, int tipo);

#endif