#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "header/index.h"

void actualizar_naves(){
    
}

int main(){
    SDL_Init(SDL_VideoInit);

    SDL_Window *window = SDL_CreateWindow("Asteroid v0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool run = true;
    while (run)
    {
        actualizar_naves();
    }
    
}