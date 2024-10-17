#include "../header/desenhar.h"

void desenhar_inimigo(SDL_Renderer *render,nave_inimiga *nave){
    SDL_Rect *rect;
    rect->h = nave->size;
    rect->w = nave->size;
    rect->x = nave->x;
    rect->y = nave->y;
    SDL_RenderDrawRect(render, rect);
}
void desenhar_player(SDL_Renderer *render){
    SDL_Rect *rect;
    rect->h = player.size;
    rect->w = player.size;
    rect->x = player.x;
    rect->y = player.y;
    SDL_RenderDrawRect(render, rect);
}