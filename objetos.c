#include "objetos.h"

Rectangle criar_plataforma(float x, float y, float largura, float altura) {
    Rectangle plataforma;
    plataforma.x = x;
    plataforma.y = y;
    plataforma.width = largura;
    plataforma.height = altura;
    return plataforma;
}

Escada criar_escada(float x, float y, float largura, float altura, int tipo) {
    Escada escada;
    escada.rect.x = x;
    escada.rect.y = y;
    escada.rect.width = largura;
    escada.rect.height = altura;
    escada.tipo = tipo; // Salvando o número inteiro aqui

    return escada;
}