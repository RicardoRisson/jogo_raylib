#include "objetos.h"

// Fábrica de plataformas: recebe coordenadas e dimensões e retorna um Rectangle
Rectangle criar_plataforma(float x, float y, float largura, float altura) {
    Rectangle plataforma;

    plataforma.x = x;
    plataforma.y = y;
    plataforma.width = largura;
    plataforma.height = altura;

    return plataforma;
}