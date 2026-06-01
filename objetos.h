#ifndef OBJETOS_H
#define OBJETOS_H

#include "raylib.h"

// Define as dimensões e posição de um obstáculo/chão
Rectangle criar_plataforma(float x, float y, float largura, float altura);
Rectangle criar_escada(float x, float y, float largura, float altura);
#endif