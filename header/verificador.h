#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"
#include "novos.h"
//#include "quadTree.h"

void actualizar_jogo();
bool calcular_probabilidade(int perc);
bool colidiu_nas_bordas(SDL_Rect rect);
int item_colidiu(SDL_Rect rect, char *item);
bool disparar(nave_inimiga *inimigos);
void ativar_scudo();
void desativar_scudo();

extern int segundos_corrente;
extern int tentar_criar_inimigo;
extern int tentar_criar_meteoro;

#endif