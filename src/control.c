#include "../header/control.h"

void trocar_arma(){
    if (player.numero_armas > 1)
        if (++player.arma_select >= player.numero_armas)
            player.arma_select = 0;
}

void ativar_scudo(){
    if (!player.scudo.ativo)
        if(player.scudo.vida <= 0)
            return;
    player.scudo.ativo = true;

    player.scudo.rect.x = player.rect.x - TAMANHO_ESCUDO;
    player.scudo.rect.y = player.rect.y - TAMANHO_ESCUDO;
    player.scudo.rect.w = TAMANHO_NAVE + TAMANHO_ESCUDO * 2;
    player.scudo.rect.h = TAMANHO_NAVE + TAMANHO_ESCUDO * 2;
}

void desativar_scudo(){
    if (player.scudo.ativo)
        player.scudo.ativo = false;
}

bool control(){
    SDL_Event evento;
    while (SDL_PollEvent(&evento)) {
        if (evento.type == SDL_QUIT) {
            return false;
        } else if (evento.type == SDL_KEYDOWN) {
            if (!player.modo_construtivo)  // Verifica antes de entrar no switch
                switch (evento.key.keysym.sym) {
                    case SDLK_v:
                        criar_parede_defensiva();
                        break;
                }

            switch (evento.key.keysym.sym) {
                case SDLK_b:
                    player.modo_construtivo = player.modo_construtivo ? false : true;
                    break;
                case SDLK_TAB:
                    trocar_arma();
                    break;
                case SDLK_n:
                    criar_robo_metralhadora();
            }
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    int bufferx,buffery;
    bufferx = player.dir.dx;
    buffery = player.dir.dy;
    
    if (state[SDL_SCANCODE_UP]) {
        player.dir.dx = 0;
        player.dir.dy = -1;
        player.rect.y -= player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.y += player.velocidade;
    }if (state[SDL_SCANCODE_DOWN]) {
        player.dir.dx = 0;
        player.dir.dy = 1;
        player.rect.y += player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.y -= player.velocidade;
    }if (state[SDL_SCANCODE_LEFT]) {
        player.dir.dy = 0; 
        player.dir.dx = -1;
        player.rect.x -= player.velocidade;
        if(item_colidiu(player.rect, "player"))
            player.rect.x += player.velocidade;
    }if (state[SDL_SCANCODE_RIGHT]) {
        player.dir.dy = 0;
        player.dir.dx = 1;
        player.rect.x += player.velocidade; 
        if(item_colidiu(player.rect, "player"))
            player.rect.x -= player.velocidade;
    }if (state[SDL_SCANCODE_SPACE]){
        disparar(player.rect,player.dir,player.arma,false);
    }

    if (bufferx != player.dir.dx
        || buffery != player.dir.dy)
    {
        player.olddx = bufferx;
        player.olddy = buffery;
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
            }else if (evento.type == SDL_KEYDOWN) {
                if (evento.key.keysym.sym == SDLK_SPACE) {
                    return true;
                }
            }
        }
    }
}