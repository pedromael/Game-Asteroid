#ifndef DESENHAR_H
#define DESENHAR_H

#include "index.h"

extern nave_player player;
extern int segundos;

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave);
void desenhar_tiro(SDL_Renderer *render, tiro *tiros);
void desenhar_player(SDL_Renderer *render);
void desenhar_obstaculo(SDL_Renderer *render, obstaculo *obstacul);
void desenhar_status(SDL_Renderer *render, TTF_Font *font);

#endif