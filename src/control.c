#include "../header/control.h"

bool control(){
    const Uint8* state = SDL_GetKeyboardState(NULL);
    SDL_Event evento;

    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        }else if (evento.type == SDL_KEYDOWN){
            switch (evento.key.keysym.sym)
            {
            case SDLK_LEFT:
                if (player.dx == 1 && player.dy == 1) player.dy = 0;
                if (player.dx == 1 && player.dy == 0) player.dy = -1;
                if (player.dx == 1 && player.dy == -1) player.dx = 0;
                if (player.dx == 0 && player.dy == -1) player.dx = -1;
                if (player.dx == -1 && player.dy == -1) player.dy = 0;
                if (player.dx == -1 && player.dy == 0) player.dy = 1;
                if (player.dx == -1 && player.dy == 1) player.dx = 0;
                if (player.dx == 0 && player.dy == 1) player.dx = 1;
                break;
            
            case SDLK_RIGHT:
                if (player.dx == 1 && player.dy == 1) player.dx = 0;
                if (player.dx == 1 && player.dy == 0) player.dy = 1;
                if (player.dx == 1 && player.dy == -1) player.dy = 0;
                if (player.dx == 0 && player.dy == -1) player.dx = 1;
                if (player.dx == -1 && player.dy == -1) player.dx = 0;
                if (player.dx == -1 && player.dy == 0) player.dy = -1;
                if (player.dx == -1 && player.dy == 1) player.dy = 0;
                if (player.dx == 0 && player.dy == 1) player.dx = -1;
                break;
            }
        }
    }

    if (state[SDL_SCANCODE_UP]) {
        player.x += player.velocidade * player.dx;
        player.y += player.velocidade * player.dy;
        if(item_colidiu(&player.x, &player.y, &player.size)){
            player.x -= player.velocidade * player.dx;
            player.y -= player.velocidade * player.dy;
        }
    }if (state[SDL_SCANCODE_DOWN]) {
        player.x -= player.velocidade * player.dx;
        player.y -= player.velocidade * player.dy;
        if(item_colidiu(&player.x, &player.y, &player.size)){
            player.x += player.velocidade * player.dx;
            player.y += player.velocidade * player.dy;
        }
    }if (state[SDL_SCANCODE_SPACE]){
        disparar(NULL);
    }
    
    return true;
}