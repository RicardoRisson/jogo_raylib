#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdbool.h> // Garante o funcionamento do tipo 'bool' de forma nativa no C

// --- ESTRUTURAS E ENUMS ---

// Struct obrigatória do placar
typedef struct tipo_placar {
    char nome[20];
    int time;
} TIPO_PLACAR;

// Estados globais do fluxo do jogo
typedef enum GameState {
    STATE_MENU,
    STATE_JOGANDO,
    STATE_PAUSADO,
    STATE_RANKING,
    STATE_SALVAR_RECORDE
} GameState;

// --- PROTÓTIPOS DAS FUNÇÕES ---

// Funções de persistência (Arquivo)
void inicializar_placar_vazio(void);
int carregar_placar(TIPO_PLACAR placar[]);
void salvar_placar(TIPO_PLACAR placar[]);
bool verificar_se_entra_no_ranking(int tempo_jogador);
void inserir_no_ranking(const char *nome, int tempo_jogador);

// Lógica de transição de telas e menus
void AtualizarBotoesInterface(GameState *estado_atual, int *opcao_menu_pausa);

// Funções de renderização de telas (Interface)
void DesenharMenuPrincipal(int opcao_selecionada);
void DesenharMenuPausa(int opcao_selecionada);
void DesenharRanking(void);
void DesenharTelaSalvarRecorde(const char *nome_atual, int tempo_final);

#endif // INTERFACE_H