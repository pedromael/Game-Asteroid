#include "../header/desenhar.h"

void desenhar_inimigo(SDL_Renderer *render, nave_inimiga *nave){
    SDL_RenderCopy(render, nave->textura, NULL, &nave->Rect);
}

void desenhar_player(SDL_Renderer *render){
    float angulo = atan2(player.dy, player.dx) * 180 / PI;
    angulo += 90;
    // Centro da rotação (podemos usar o centro da imagem)
    SDL_Point centro = { player.rect.w / 2, player.rect.h / 2 };

    // Renderizar a textura com rotação
    SDL_RenderCopyEx(render, player.textura, NULL, &player.rect, angulo, &centro, SDL_FLIP_NONE);
}

void desenhar_explosao(SDL_Renderer *render, explosao *item){
    SDL_RenderCopy(render, SDL_CreateTextureFromSurface(render,IMG_Load("files/img/explosao0.png")), NULL, &item->rect);
}

void desenhar_tiro(SDL_Renderer *render, tiro *tiros){
    SDL_RenderCopy(render, tiros->arma.textura, NULL, &tiros->rect);
}

void desenhar_meteoro(SDL_Renderer *render, meteoro *meteoro){
    SDL_RenderCopy(render, meteoro->textura, NULL, &meteoro->rect);
}

void desenhar_obstaculo(SDL_Renderer *render, obstaculo *obstacul){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &obstacul->rect);
}

void desenhar_scudo(){
    SDL_RenderDrawRect(render, &player.scudo.rect);
}

void desenhar_status(SDL_Renderer *render, TTF_Font *font){
    SDL_Color color = {0, 0, 0};

    char buffer[10];
    sprintf(buffer,"%d",player.arma[player.arma_select].no_pente);
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect destRect = {LARGURA - surface->w - 17, ALTURA - surface->h - 9, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &destRect);

    sprintf(buffer,"HP:%d",player.vida);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect Rect = {LARGURA - surface->w - 17, 0, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &Rect);

    sprintf(buffer,"time: %d",segundos);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect Rect1 = {10, 5, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &Rect1);
}