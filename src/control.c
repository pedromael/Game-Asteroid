#include "../header/control.h"

bool control(){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        }
    }       

    if (state[SDL_SCANCODE_UP]) {
        player.dx = 0;
        player.dy = -1;
        player.y -= player.velocidade;
        if(item_colidiu(&player.x, &player.y, &player.size))
            player.y += player.velocidade;
    }if (state[SDL_SCANCODE_DOWN]) {
        player.dx = 0;
        player.dy = 1;
        player.y += player.velocidade;
        if(item_colidiu(&player.x, &player.y, &player.size))
            player.y -= player.velocidade;
    }if (state[SDL_SCANCODE_LEFT]) {
        player.dy = 0;
        player.dx = -1;
        player.x -= player.velocidade;
        if(item_colidiu(&player.x, &player.y, &player.size))
            player.x += player.velocidade;
    }if (state[SDL_SCANCODE_RIGHT]) {
        player.dy = 0;
        player.dx = 1;
        player.x += player.velocidade; 
        if(item_colidiu(&player.x, &player.y, &player.size))
            player.x -= player.velocidade;
    }
    return true;
}