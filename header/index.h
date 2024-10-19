#ifndef INDEX_H
#define INDEX_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define LARGURA 800
#define ALTURA 600
#define MAXIMO_INIMIGOS 20

#define TAMANHO_NAVE 10
#define VELOCIDADE_INICAL 6
#define VELOCIDADE_INIMIGA 4

typedef struct
{
    int danos;
    int bps; // balas por segundo
    int bala_size;
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
    int tipo;
    int velocidade;
    armas arma;
} nave_inimiga;

#endif