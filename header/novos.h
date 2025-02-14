#ifndef NOVOS_H
#define NOVOS_H

#include "index.h"
#include "verificador.h"

bool criar_inimigo(SDL_Renderer *render, nave_inimiga *inimigo, int nivel);
bool criar_meteoro(SDL_Renderer *render);
bool criar_pacote(meteoro *met);
bool criar_explosao(SDL_Renderer *render, int tipo, SDL_Rect rect);
bool criar_parede_defensiva();
bool criar_robo_metralhadora();


#endif 