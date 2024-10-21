#include "header/index.h"

bool player_status = true;
nave_player player;
int numero_inimigos;
int numero_inimigos_inicial = 10;
int capacidade_inimigos = 10;
nave_inimiga *inimigos;
int numero_obstaculos = 2;
obstaculo *obstaculos;
const int numero_armas = 5;
armas *arsenal;
int numero_tiros;
int capacidade_tiros = 50;
tiro *tiros;
int segundos,quadros;

#include "header/desenhar.h"
#include "header/control.h"
#include "header/verificador.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Window *window = SDL_CreateWindow("Asteroid v0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("files/Roboto-Light.ttf", 14);

    obstaculos = malloc(numero_obstaculos * sizeof(obstaculo));
    arsenal = malloc(numero_armas * sizeof(armas));
    tiros = malloc(capacidade_tiros * sizeof(tiro));
    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    
    int conf_armas[5][6] = {
        { 5,  10, 12, 30,  2,  2},
        {10,  10, 10, 25,  1,  2},
        {15,  6, 12, 20,  3,  3},
        {50,  2,  8, 10, 5,  6},
        {35, 25,  9, 35,  3, 4}
    };
//danos - bps - bala_velocidade - pente_max - tempo_carga - bala_size
    for (size_t j = 0; j < numero_armas; j++){
        arsenal[j].danos =              conf_armas[j][0];
        arsenal[j].bps =                conf_armas[j][1];
        arsenal[j].bala_velocidade =    conf_armas[j][2];
        arsenal[j].pente_max =          conf_armas[j][3];
        arsenal[j].tempo_carregamento = conf_armas[j][4];
        arsenal[j].bala_size =          conf_armas[j][5];
        arsenal[j].no_pente =           arsenal[j].pente_max;  // Corrigido para usar o próprio j
        arsenal[j].inicio_carregamento = false;
        arsenal[j].ultimo_tiro = false;
    }


    obstaculos[0].sizeX = 12;
    obstaculos[0].sizeY = 330;
    obstaculos[0].x = 500;
    obstaculos[0].y = 30;
    obstaculos[0].vida = 3300;

    obstaculos[1].sizeX = 205;
    obstaculos[1].sizeY = 10;
    obstaculos[1].x = 28;
    obstaculos[1].y = 300;
    obstaculos[1].vida = 2890;

    player.x = LARGURA/2;
    player.y = ALTURA/2;
    player.size = TAMANHO_NAVE;
    player.dx = 0;
    player.dy = -1;
    player.arma = arsenal[3];
    player.velocidade = VELOCIDADE_INICIAL;
    player.vida = 200;

    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));

    for (size_t i = 0; i < numero_inimigos_inicial; i++)
        if(criar_inimigo(&inimigos[i],1))
            numero_inimigos++;
        else
            printf("nao conseguiu criar inimigo\n");

    int quadros_corrente;

    bool run = true;
    while (run)
    {
        if (quadros_corrente >= 60){
            quadros_corrente = 0;
            segundos++;
        }

        SDL_SetRenderDrawColor(render,255,255,255,255);
        SDL_RenderClear(render);

        if(!control()) return false;

        actualizar_jogo();

        SDL_SetRenderDrawColor(render,0,0,0,255);
        desenhar_player(render);
        for (size_t i = 0; i < numero_inimigos; i++)
            desenhar_inimigo(render,&inimigos[i]);
        
        for (size_t i = 0; i < numero_obstaculos; i++)
            desenhar_obstaculo(render,&obstaculos[i]);

        for (size_t i = 0; i < numero_tiros; i++)
            desenhar_tiro(render, &tiros[i]);

        desenhar_status(render,font);

        SDL_RenderPresent(render);
        SDL_Delay(16);

        quadros++;
        quadros_corrente++;

        if (!player_status)
        {
            printf("vc perdeu, aperte SPACO para reiniciar jogo\n");
            while (1)
            {
                SDL_Event evento;
                while (SDL_PollEvent(&evento)){
                    if (evento.type == SDL_QUIT)
                        return 0;
                    
                    if (evento.type == SDL_KEYDOWN)
                        if (evento.key.keysym.sym == SDLK_SPACE)
                            break;
                }
            }
        }
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}