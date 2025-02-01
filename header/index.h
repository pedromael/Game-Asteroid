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

//dimensoes da tela
#define LARGURA 1300
#define ALTURA 850

#define MAXIMO_PACOTES 5
#define MAXIMO_INIMIGOS 5
#define MAXIMO_SCUDO 2000

#define TAMANHO_NAVE 25 // pixel
#define TAMANHO_PACOTE 15 // pixel
#define TEMPO_PARA_APAGAR_METEORO 3 // segundo

#define TEMPO_DE_PAREDE_DEFENSIVA 90 // segundo 
#define DISTANCIA_PAREDE_PLAYER 30 // 
#define TAMANHO_ESCUDO 20 // pixel
#define VIDA_PAREDE 350
#define PAREDE_W 45
#define PAREDE_H 8

#define VELOCIDADE_INICIAL 6 // pixel por quadro
#define VELOCIDADE_INIMIGA 3 // pixel por quadro
#define TEMPO_REFLEXO_INIMIGO 2

#define NUMERO_DE_ARMAS 5
#define TIPOS_DE_INIMIGOS 3

typedef struct
{
    int indice;
    int danos;
    int bps; // balas por segundo
    int bala_size;
    int no_pente;
    int pente_max;
    SDL_Texture *textura;
    int tempo_carregamento;
    int alcance;
    int bala_velocidade;
    int inicio_carregamento;
    int ultimo_bala;
    Mix_Chunk *som;
} armas;

typedef struct{
    SDL_Rect rect;
    int dx,dy;
    int distancia_percorrida;
    armas *arma;
    int inimigo;
} bala;

typedef struct
{
    SDL_Texture *textura;
    SDL_Rect rect;
    int vida;
    bool ativo;
} scudo;

typedef struct
{
    SDL_Rect rect;
    int vida;
    int tipo;
    int tempo_posto;
} parede_defensiva;

typedef struct
{
    int dx,dy;
    int olddx,olddy;
    int numero_armas;
    int arma_select;
    armas *arma;
    int upgrade;
    int vida;
    int velocidade;
    SDL_Rect rect;
    SDL_Texture *textura;
    scudo scudo;
    bool modo_construtivo;
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
    int tempo; // milisegundo
    int milissegundo_inicio;
    SDL_Texture *textura;
} explosao;

extern bool player_status;
extern nave_player player;
extern armas *arsenal;
extern int numero_inimigos;
extern int capacidade_inimigos;
extern nave_inimiga *inimigos;
extern int numero_balas;
extern int capacidade_balas;
extern bala *balas;
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
extern int numero_paredes_defensivas;
extern int capacidade_paredes_defensivas;
extern parede_defensiva *paredes_defensiva;

extern SDL_Renderer *render;
extern int segundos;

extern SDL_Texture *textura_explosao_bala;
extern SDL_Texture *textura_explosao_parede;
extern SDL_Texture *textura_explosao_inimigo;
extern SDL_Texture *textura_meteoro;
extern SDL_Texture **textura_inimigos;

#endif