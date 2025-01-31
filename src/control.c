#include "../header/control.h"

bool control(){
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        } else if (evento.type == SDL_KEYDOWN) {
            if (!player.modo_construtivo) {  // Verifica antes de entrar no switch
                switch (evento.key.keysym.sym) {
                    case SDLK_v:
                        criar_parede_defensiva();
                        break;
                }
            }
            switch (evento.key.keysym.sym) {
                case SDLK_b:
                    player.modo_construtivo = player.modo_construtivo ? false : true;
                    break;
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP]) {
        player.dx = 0;
        player.dy = -1;
        player.rect.y -= player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.y += player.velocidade;
    }if (state[SDL_SCANCODE_DOWN]) {
        player.dx = 0;
        player.dy = 1;
        player.rect.y += player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.y -= player.velocidade;
    }if (state[SDL_SCANCODE_LEFT]) {
        player.dy = 0;
        player.dx = -1;
        player.rect.x -= player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.x += player.velocidade;
    }if (state[SDL_SCANCODE_RIGHT]) {
        player.dy = 0;
        player.dx = 1;
        player.rect.x += player.velocidade; 
        if(item_colidiu(player.rect, "player"))
            player.rect.x -= player.velocidade;
    }if (state[SDL_SCANCODE_SPACE]){
        disparar(NULL);
    }
    
    if (state[SDL_SCANCODE_V] &&
        player.modo_construtivo)
        criar_parede_defensiva();
    
    if (state[SDL_SCANCODE_C])
        ativar_scudo(); 
    else
        desativar_scudo();
    
    return true;
}

bool control_perdeu(){
    while (1)
    {
        SDL_Event evento;
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                return 0;
            }
            
            if (evento.type == SDL_KEYDOWN) {
                if (evento.key.keysym.sym == SDLK_SPACE) {
                    player_status = true;
                    player.vida = 200;
                    return true;
                }
            }
        }

    }
}