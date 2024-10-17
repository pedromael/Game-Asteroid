#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "header/index.h"

void desenhar(){
    
}

void actualizar_naves(){

}

int main(){
    SDL_Init(SDL_VideoInit);

    SDL_Window *window = SDL_CreateWindow("Asteroid v0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    player.x = LARGURA/2;
    player.y = ALTURA/2;
    player.dx = 0;
    player.dy = 0;
    player.arma = arsenal[0];
    player.velocidade = VELOCIDADE_INICAL;
    player.vida = 100;

    bool run = true;
    while (run)
    {
        actualizar_naves();
    }
    
}