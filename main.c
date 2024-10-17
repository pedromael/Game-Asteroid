#include "header/index.h"

bool control(){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        }
    }       

    if (state[SDL_SCANCODE_W]) {
        player.dx = 0;
        player.dy = -1;
        player.y -= player.velocidade;
    }if (state[SDL_SCANCODE_S]) {
        player.dx = 0;
        player.dy = 1;
        player.y += player.velocidade;
    }if (state[SDL_SCANCODE_A]) {
        player.dy = 0;
        player.dx = -1;
        player.x -= player.velocidade;
    }if (state[SDL_SCANCODE_D]) {
        player.dy = 0;
        player.dx = 1;
        player.x += player.velocidade; 
    }
    return true;
}

void actualizar_naves(){

}

int main(){
    SDL_Init(SDL_VideoInit);

    SDL_Window *window = SDL_CreateWindow("Asteroid v0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    
    player.x = LARGURA/2;
    player.y = ALTURA/2;
    player.size = TAMANHO_NAVE;
    player.dx = 0;
    player.dy = 0;
    player.arma = arsenal[0];
    player.velocidade = VELOCIDADE_INICAL;
    player.vida = 100;

    bool run = true;
    while (run)
    {
        if(!control()) return 0;

        desenhar_player(render);
        for (size_t i = 0; i < numeros_inimigos; i++)
            desenhar_inimigo(render,inimigos[i]);
        
        actualizar_naves();
        
    SDL_RenderPresent(render);
    SDL_Delay(16);
    }
    
}