#ifndef DESENHAR_H
#define DESENHAR_H

#include "index.h"

extern nave_player player;

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave);
void desenhar_player(SDL_Renderer *render);

#endif