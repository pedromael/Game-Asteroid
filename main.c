#include "header/index.h"

nave_player player;
int numero_inimigos;
int numero_inimigos_inicial = 10;
int capacidade_inimigos = 10;
nave_inimiga *inimigos;
const int numero_obstaculos = 0;
obstaculo *obstaculos;
const int numero_armas = 5;
armas *arsenal;
int numero_tiros;
int capacidade_tiros = 50;
tiro *tiros;

#include "header/desenhar.h"
#include "header/control.h"
#include "header/verificador.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Asteroid v0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    obstaculos = malloc(numero_obstaculos * sizeof(obstaculo));
    arsenal = malloc(numero_armas * sizeof(armas));
    tiros = malloc(capacidade_tiros * sizeof(tiro));
    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    
    arsenal[0].danos = 10;
    arsenal[0].bps = 12;
    arsenal[0].bala_size = 2;

    player.x = LARGURA/2;
    player.y = ALTURA/2;
    player.size = TAMANHO_NAVE;
    player.dx = 0;
    player.dy = -1;
    player.arma = arsenal[0];
    player.velocidade = VELOCIDADE_INICAL;
    player.vida = 100;

    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));

    for (size_t i = 0; i < numero_inimigos_inicial; i++)
        if(criar_inimigo(&inimigos[i],1))
            numero_inimigos++;
        else
            printf("nao conseguiu criar inimigo\n");

    bool run = true;
    while (run)
    {
        SDL_SetRenderDrawColor(render,255,255,255,255);
        SDL_RenderClear(render);

        if(!control()) return 0;

        actualizar_jogo();

        SDL_SetRenderDrawColor(render,0,0,0,255);
        desenhar_player(render);
        for (size_t i = 0; i < numero_inimigos; i++)
            desenhar_inimigo(render,&inimigos[i]);
        
        // for (size_t i = 0; i < numero_obstaculos; i++)
        //     desenhar_obstaculo();

        for (size_t i = 0; i < numero_tiros; i++)
            desenhar_tiro(render, &tiros[i]);
        

        SDL_RenderPresent(render);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}