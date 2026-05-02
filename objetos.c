#include "objetos.h"

// --- Construtores de Cenário ---
// Inicializa um retângulo com as propriedades da plataforma
Rectangle criar_plataforma(float x, float y, float largura, float altura) {
    Rectangle plataforma;

    plataforma.x = x;
    plataforma.y = y;
    plataforma.width = largura;
    plataforma.height = altura;

    return plataforma;
}