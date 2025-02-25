#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"
#include "novos.h"
//#include "quadTree.h"

void actualizar_jogo();
void actualizar_balas();
void actualizar_pacotes();
void actualizar_explosoes();
void actualizar_meteoros();
void actualizar_inimigos();
void actualizar_robos_metralhadora();
void actualizar_paredes_defensivas();
bool disparar(SDL_Rect rect, direcao dir, armas *arma, bool de_inimigo);

bool calcular_probabilidade(int perc);
bool colidiu_nas_bordas(SDL_Rect rect);
bool colidiram(SDL_Rect *rect1, SDL_Rect *rect2);
bool item_colidiu(SDL_Rect rect, char *item);
void destroir_parede_defensiva(int i);
bool na_mira(direcao dir, SDL_Rect rectIndex, SDL_Rect rectRec, int errorMarge);

extern int segundos_corrente;
extern int tentar_criar_inimigo;
extern int tentar_criar_meteoro;

#endif