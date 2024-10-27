#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"
#include "novos.h"
//#include "desenhar.h"

void actualizar_jogo();
bool calcular_probabilidade(int perc);
bool colidiu_nas_bordas();
int item_colidiu(int *x, int *y, int *size, char *item);
bool disparar(nave_inimiga *inimigos);
void ativar_scudo();
void desativar_scudo();

#endif