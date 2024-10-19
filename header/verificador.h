#ifndef VERIFICADOR_H
#define VERIFICADOR_H

#include "index.h"

extern nave_player player;
extern const int numero_armas;
extern armas *arsenal;
extern int numero_inimigos;
extern nave_inimiga *inimigos;
extern int numero_tiros;
extern int capacidade_tiros;
extern tiro *tiros;

void actualizar_jogo();

bool colidiu_nas_bordas();
int item_colidiu();
bool disparar(nave_inimiga *inimigos);
bool criar_inimigo(nave_inimiga *inimigo, int nivel);

#endif