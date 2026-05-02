# 🎮 Jogo Raylib - Estrutura Organizada

Este projeto consiste em um ambiente de desenvolvimento para um jogo 2D utilizando a biblioteca **Raylib** em C. O código foi modularizado para garantir escalabilidade e organização, seguindo boas práticas de programação.

## 🏗️ Estrutura do Projeto

O projeto está dividido em módulos específicos para facilitar a manutenção:

- **`main.c`**: Arquivo principal que gerencia o loop do jogo, inicialização e renderização.
- **`movimentacao.h/.c`**: Lógica de deslocamento do jogador e verificações de colisão com o chão.
- **`personagens.h/.c`**: Fábrica de criação do jogador e configurações iniciais do personagem.
- **`objetos.h/.c`**: Gerenciamento de elementos do cenário, como a criação de plataformas.
- **`Makefile`**: Script de automação para compilação rápida do projeto.

## 🚀 Tecnologias e Padrões

- **Linguagem**: C11
- **Biblioteca Gráfica**: Raylib
- **Padrão de Escrita**: `snake_case` para funções e variáveis.
- **Organização**: Uso de *Header Guards* e separação de interface (.h) e implementação (.c).

## 🎮 Implementação Atual

O jogo atualmente conta com:
1. **Jogador (Quadrado Verde)**: Capaz de se mover horizontalmente (Teclas A e D).
2. **Sistema de Plataformas (Vermelhas)**: Suporte para múltiplas plataformas simultâneas através de arrays.
3. **Detecção de Solo**: O jogador identifica automaticamente em qual plataforma está pisando, ajustando sua altura (Y) de forma independente.

## 🛠️ Como Compilar e Rodar

Certifique-se de ter a Raylib instalada em seu sistema.

1. **Compilar**:
   ```bash
   make

    Executar:
    Bash

    make run

    Limpar arquivos temporários:
    Bash

    make clean