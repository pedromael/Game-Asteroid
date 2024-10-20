#ifndef INDEX_H
#define INDEX_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define LARGURA 800
#define ALTURA 600
#define MAXIMO_INIMIGOS 20

#define TAMANHO_NAVE 12
#define VELOCIDADE_INICIAL 6
#define VELOCIDADE_INIMIGA 3

typedef struct
{
    int danos;
    int bps; // balas por segundo
    int bala_size;
    int no_pente;
    int pente_max;
    int tempo_carregamento;
    int bala_velocidade;
    int inicio_carregamento;
    int ultimo_tiro;
} armas;

typedef struct{
    int x,y;
    int dx,dy;
    armas arma;
    int inimigo;
} tiro;

typedef struct
{
    int x,y;
    int dx,dy;
    int size;
    armas arma;
    int upgrade;
    int vida;
    int velocidade;
} nave_player;

typedef struct
{
    int x,y;
    int sizeX,sizeY;
    int vida;
} obstaculo;

typedef struct
{
    int x,y;
    int dx,dy;
    int size;
    int vida;
    int velocidade;
    armas arma;
} nave_inimiga;

#endif