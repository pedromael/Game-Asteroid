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
#define LARGURA 1800
#define ALTURA 950

#define MAXIMO_PACOTES 10
#define MAXIMO_INIMIGOS 20
#define INIMIGOS_INICIAL 2
#define MAXIMO_SCUDO 2000
#define NUMERO_ROBO_METRALHADORA_INICIAL 5
#define VIDA_INICIAL_PLAYER 300

#define TAMANHO_NAVE 25 // pixel
#define TAMANHO_PACOTE 15 // pixel
#define TEMPO_PARA_APAGAR_METEORO 3 // segundo

#define DANOS_BALA_RAIO 2 // por frame
#define TOTAL_DANOS_BALA_RAIO 500
#define VELOCIDADE_BALA_RAIO 7
#define MAXIMO_RAIO_BALA_RAIO 60 // pixel
#define TEMPO_DE_PAREDE_DEFENSIVA 120 // segundo 
#define DISTANCIA_PAREDE_PLAYER 30 // 
#define TAMANHO_ESCUDO 20 // pixel
#define VIDA_PAREDE 450
#define PAREDE_W 45
#define PAREDE_H 10

#define VELOCIDADE_INICIAL 6 // pixel por quadro
#define VELOCIDADE_INIMIGA 3 // pixel por quadro
#define TEMPO_REFLEXO_INIMIGO 2

#define NUMERO_DE_ARMAS 5
#define TIPOS_DE_INIMIGOS 3

typedef struct
{
    int dx,dy;    
} direcao;

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
    direcao dir;
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
    int robos_metralhadora;
    int bolas_raio;
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
    direcao dir;
    //int nivel;
    int vida;
    SDL_Texture *textura;
    int velocidade;
    armas arma;
    int tempo_ronda;
    int ultima_ronda;
    bool bala_raio;
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
    int step;
    int lastStep;
    int tempo; // milisegundo
    int milissegundo_inicio;
    SDL_Texture *textura;
} explosao;

typedef struct
{
    int vida;
    int angulo;
    int velocidade_giro;
    SDL_Rect rect;
    armas *arma;
} robo_metralhadora;

typedef struct
{
    int x,y;
    float raio;
    int danos;
} bala_raio;

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
extern int numero_robos_metralhadora;
extern int capacidade_robos_metralhadora;
extern robo_metralhadora *robos_metralhadora;
extern int numero_balas_raio;
extern int capacidade_bala_raio;
extern bala_raio *balas_raio;

extern SDL_Renderer *render;
extern int segundos;

extern SDL_Texture *textura_explosao_bala;
extern SDL_Texture *textura_explosao_parede;
extern SDL_Texture *textura_explosao_inimigo;
extern SDL_Texture *textura_meteoro;
extern SDL_Texture **textura_inimigos;
extern SDL_Texture **textura_pacotes;

extern Mix_Chunk *som_explosao_inimigo;

#endif