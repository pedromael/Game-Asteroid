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

bool criar_parede_defensiva() {
    if (numero_paredes_defensivas >= capacidade_paredes_defensivas) {
        capacidade_paredes_defensivas += 5;
        paredes_defensiva = realloc(paredes_defensiva, capacidade_paredes_defensivas * sizeof(parede_defensiva));
    }

    int i = numero_paredes_defensivas;
    paredes_defensiva[i].vida = VIDA_PAREDE;
    paredes_defensiva[i].tempo_posto = segundos;

    int dx = player.modo_construtivo ? player.olddx : player.dir.dx;
    int dy = player.modo_construtivo ? player.olddy : player.dir.dy;

    if (dx || dy) {
        paredes_defensiva[i].rect.x = player.rect.x + (DISTANCIA_PAREDE_PLAYER * dx);
        paredes_defensiva[i].rect.y = player.rect.y + (DISTANCIA_PAREDE_PLAYER * dy);
        paredes_defensiva[i].rect.w = (dx != 0) ? PAREDE_H : PAREDE_W;
        paredes_defensiva[i].rect.h = (dx != 0) ? PAREDE_W : PAREDE_H;
    } else
        return false;

    if (item_colidiu(paredes_defensiva[i].rect, ""))
        return false;
    numero_paredes_defensivas++;
    return true;
}
