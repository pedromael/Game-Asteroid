#ifndef DESENHAR_H
#define DESENHAR_H

#include "index.h"

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave);
void desenhar_bala(SDL_Renderer *render, bala *balas);
void desenhar_player(SDL_Renderer *render);
void desenhar_obstaculo(SDL_Renderer *render, obstaculo *obstacul);
void desenhar_status(SDL_Renderer *render, TTF_Font *font);
void desenhar_meteoro(SDL_Renderer *render, meteoro *meteoro);
void desenhar_explosao(SDL_Renderer *render, explosao *eps);
void desenhar_scudo();

#endif