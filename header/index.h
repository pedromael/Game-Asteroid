#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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
    int x,y;
    int dx,dy;
} arma;

typedef struct
{
    int x,y;
    int dx,dy;
    int size;
    arma arma;
    int upgrade;
    int vida;
    int velocidade;
} nave_player;

typedef struct
{
    int x,y;
    int sizeX,sizeY;
    int vida;
} abstaculo;

typedef struct
{
    int x,y;
    int dx,dy;
    int size;
    int vida;
    int tipo;
    int velocidade;
    arma arma;
} nave_inimiga;

nave_player player;
int numeros_inimigos = 0;
nave_inimiga *inimigos;
arma arsenal[5];