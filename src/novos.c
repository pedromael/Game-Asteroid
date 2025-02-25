#include "../header/novos.h"

bool criar_explosao(SDL_Renderer *render,int tipo, SDL_Rect rect){
    int i = numero_explosoes;

    if(numero_explosoes >= capacidades_explosoes){
        capacidades_explosoes += 5;
        explosoes = realloc(explosoes, capacidades_explosoes * sizeof(explosao));
    }
    
    explosoes[i].rect = rect;
    explosoes[i].rect.w += 40;
    explosoes[i].rect.h += 40;
    explosoes[i].rect.x -= 20;
    explosoes[i].rect.y -= 20;
    explosoes[i].step = 1;
    explosoes[i].lastStep = 16; // 16 estados
    explosoes[i].milissegundo_inicio = SDL_GetTicks();

    if (tipo == 1) // para um bala
    {
        explosoes[i].tempo = 200;
        explosoes[i].textura = textura_explosao_bala;
    }else if(tipo == 2){ // para um e parede
        explosoes[i].tempo = 2*1000;
        explosoes[i].textura = textura_explosao_parede;
    }else if(tipo == 3){ // para um inimigo
        explosoes[i].tempo = 2*800;
        explosoes[i].textura = textura_explosao_inimigo;
        explosoes[i].rect.w += 100;
        explosoes[i].rect.h += 100;
        explosoes[i].rect.x -= 50;
        explosoes[i].rect.y -= 50;

        Mix_PlayChannel(2, som_explosao_inimigo, 0);
    }
    
    numero_explosoes++;
    return true;
}
