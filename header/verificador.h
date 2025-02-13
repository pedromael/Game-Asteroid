#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"
#include "novos.h"
//#include "quadTree.h"

void actualizar_jogo();
bool calcular_probabilidade(int perc);
bool colidiu_nas_bordas(SDL_Rect rect);
bool colidiram(SDL_Rect *rect1, SDL_Rect *rect2);
bool item_colidiu(SDL_Rect rect, char *item);
bool disparar(SDL_Rect rect, direcao dir, armas *arma, bool de_inimigo);

void destroir_parede_defensiva(int i);

extern int segundos_corrente;
extern int tentar_criar_inimigo;
extern int tentar_criar_meteoro;

#endif