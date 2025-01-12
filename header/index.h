#ifndef INDEX_H
#define INDEX_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#define PI 3.141592653589793

#define LARGURA 1300
#define ALTURA 850

#define MAXIMO_PACOTES 5
#define MAXIMO_INIMIGOS 20
#define MAXIMO_SCUDO 2000

#define TAMANHO_NAVE 25
#define TAMANHO_PACOTE 10
#define TEMPO_PARA_APAGAR_METEORO 3

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
    Mix_Chunk *som;
} armas;

typedef struct{
    SDL_Rect rect;
    int dx,dy;
    armas arma;
    int inimigo;
} tiro;

typedef struct
{
    SDL_Texture *textura;
    SDL_Rect rect;
    int vida;
    bool ativo;
} scudo;

typedef struct
{
    int dx,dy;
    int numero_armas;
    int arma_select;
    armas *arma;
    int upgrade;
    int vida;
    int velocidade;
    SDL_Rect rect;
    SDL_Texture *textura;
    scudo scudo;
} nave_player;

typedef struct
{
    SDL_Rect rect;
    int vida;
} obstaculo;

typedef struct
{
    SDL_Rect rect;
    SDL_Texture *textura;
    int dx, dy;
    int danos;
    int vida;
    int velocidade;
    bool status;
    int tempo_partiu;
} meteoro;

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

typedef struct
{
    SDL_Rect rect;
    SDL_Texture *textura;
    int tipo; // tipo de item no pacote
    int valor; // valor no caso de vida 
} pacote;

typedef struct
{
    SDL_Rect rect;
    int tempo; //em segundo
    int primeiro_quadro;
    SDL_Texture *textura;
} explosao;

extern bool player_status;
extern nave_player player;
extern const int numero_armas;
extern armas *arsenal;
extern int numero_inimigos;
extern int capacidade_inimigos;
extern nave_inimiga *inimigos;
extern int numero_tiros;
extern int capacidade_tiros;
extern tiro *tiros;
extern int numero_obstaculos;
extern obstaculo *obstaculos;
extern int segundos;
extern int numero_meteoros;
extern int capacidade_meteoros;
extern meteoro *meteoros;
extern int numero_pacotes;
extern pacote *pacotes;
extern int numero_explosoes;
extern int capacidades_explosoes;
extern explosao *explosoes;

extern SDL_Renderer *render;
extern int segundos;

#endif