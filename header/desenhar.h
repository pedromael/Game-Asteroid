#ifndef DESENHAR_H
#define DESENHAR_H

#include "index.h"

extern nave_player player;

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave);
void desenhar_tiro(SDL_Renderer *render, tiro *tiros);
void desenhar_player(SDL_Renderer *render);
void desenhar_obstaculo(SDL_Renderer *render, obstaculo *obstacul);

#endif