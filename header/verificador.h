#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"

extern nave_player player;
extern const int numero_armas;
extern armas *arsenal;
extern int numero_inimigos;
extern nave_inimiga *inimigos;

void actualizar_jogo();

bool colidiu_nas_bordas();
int item_colidiu();
void criar_inimigo(nave_inimiga *inimigo, int nivel);

#endif