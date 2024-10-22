#ifndef INDEX_H
#define INDEX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PI 3.141592653589793

#define LARGURA 1300
#define ALTURA 900
#define MAXIMO_INIMIGOS 20

#define TAMANHO_NAVE 50
#define VELOCIDADE_INICIAL 6
#define VELOCIDADE_INIMIGA 3

typedef struct
{
    int danos;
    int bps; // balas por segundo
    int bala_size;
    int no_pente;
    int pente_max;
    SDL_Texture *textura;
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
    SDL_Texture *textura;
} nave_player;

typedef struct
{
    int x,y;
    int sizeX,sizeY;
    int vida;
} obstaculo;

typedef struct
{
    SDL_Rect Rect;
    int dx,dy;
    //int nivel;
    int vida;
    SDL_Texture *textura;
    int velocidade;
    armas arma;
    int tempo_ronda;
    int ultima_ronda;
} nave_inimiga;

#endif