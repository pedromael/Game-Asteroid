#include "../header/desenhar.h"

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave){
    SDL_Rect rect;
    rect.h = nave->size;
    rect.w = nave->size;
    rect.x = nave->x;
    rect.y = nave->y;
    SDL_RenderDrawRect(render, &rect);
}

void desenhar_player(SDL_Renderer *render){
    SDL_Rect rect;
    rect.h = player.size;
    rect.w = player.size;
    rect.x = player.x;
    rect.y = player.y;
    SDL_RenderDrawRect(render, &rect);
}

void desenhar_tiro(SDL_Renderer *render, tiro *tiros){
    SDL_Rect rect;
    rect.h = tiros->arma.bala_size;
    rect.w = tiros->arma.bala_size;
    rect.x = tiros->x;
    rect.y = tiros->y;
    SDL_RenderDrawRect(render, &rect);
}

void desenhar_obstaculo(SDL_Renderer *render, obstaculo *obstacul){
    SDL_Rect rect;
    rect.h = obstacul->sizeY;
    rect.w = obstacul->sizeX;
    rect.x = obstacul->x;
    rect.y = obstacul->y;
    SDL_RenderDrawRect(render, &rect);
}

void desenhar_status(SDL_Renderer *render, TTF_Font *font){
    SDL_Color color = {0, 0, 0};

    char buffer[10];
    sprintf(buffer,"%d",player.arma.no_pente);
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect destRect = {LARGURA - surface->w - 17, ALTURA - surface->h - 9, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &destRect);

    sprintf(buffer,"HP:%d",player.vida);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect Rect = {LARGURA - surface->w - 17, 0, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &Rect);
}