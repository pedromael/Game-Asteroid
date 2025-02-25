#ifndef NOVOS_H
#define NOVOS_H

#include "index.h"
#include "verificador.h"

bool criar_meteoro(SDL_Renderer *render);
bool criar_inimigo(int nivel);
bool criar_robo_metralhadora();
bool criar_parede_defensiva();
bool criar_pacote(meteoro *met);
bool criar_explosao(SDL_Renderer *render, int tipo, SDL_Rect rect);

void destroir_meteoro(int *i);
void remover_inimigo(int i);
void retirar_bala(int *i, bool explosao);

#endif 