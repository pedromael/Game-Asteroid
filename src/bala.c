#include "../header/verificador.h"

bool disparar(SDL_Rect rect, direcao dir, armas *arma, bool de_inimigo){    
    int x,y,dx,dy;
    x = rect.x;
    y = rect.y;
    dx = dir.dx;
    dy = dir.dy;

    if (!arma->no_pente)
    {
        if(arma->inicio_carregamento){
            if (SDL_GetTicks() - arma->inicio_carregamento >= arma->tempo_carregamento*1000){
                arma->no_pente = arma->pente_max;
                arma->inicio_carregamento = false;
            }
        }else
            arma->inicio_carregamento = SDL_GetTicks();

        return false;
    }

    if(arma->ultimo_bala != 0)
        if (arma->ultimo_bala + (1000 / arma->bps) >= SDL_GetTicks())
            return false;

    if (numero_balas >= capacidade_balas){
        capacidade_balas += 5; 
        balas = realloc(balas, capacidade_balas * sizeof(bala));
    }

    balas[numero_balas].rect.x = x + TAMANHO_NAVE/2 - arma->bala_size/2;
    balas[numero_balas].rect.y = y + TAMANHO_NAVE/2 - arma->bala_size/2;
    balas[numero_balas].rect.w = arma->bala_size;
    balas[numero_balas].rect.h = arma->bala_size;
    balas[numero_balas].distancia_percorrida = 0;
    balas[numero_balas].dx = dx;
    balas[numero_balas].dy = dy;

    balas[numero_balas].arma = arma;
    if (de_inimigo)
        balas[numero_balas].inimigo = true;
    else
        balas[numero_balas].inimigo = false;
    
    numero_balas++;
    arma->no_pente--;
    arma->ultimo_bala = SDL_GetTicks(); 

    Mix_PlayChannel(-1, balas[numero_balas-1].arma->som, 0);
    return true;
}

void retirar_bala(int *i, bool explosao){
    if (explosao)
    {
        balas[*i].rect.w += 4;
        balas[*i].rect.h += 4;

        criar_explosao(render, 1, balas[*i].rect);
    }

    if (*i !=  --numero_balas)
        balas[*i] = balas[numero_balas];
}

void actualizar_balas() {
    for (int i = numero_balas - 1; i >= 0; i--) {
        if (!item_colidiu(balas[i].rect, "bala")) {
            int passos = balas[i].arma->bala_velocidade; // Número de passos pequenos (1 pixel cada)
            int p;
            for (p = 0; p < passos; p++) {
                balas[i].rect.x += balas[i].dx;
                balas[i].rect.y += balas[i].dy;
                balas[i].distancia_percorrida += 1;
                // Se colidir no meio do caminho, para imediatamente
                if (item_colidiu(balas[i].rect, "bala")) {
                    break;
                }
            }

            // Se chegou até aqui, significa que não colidiu
            if (p == passos) {
                // Se ultrapassou o alcance, remove a bala
                if (balas[i].distancia_percorrida >= balas[i].arma->alcance) {
                    retirar_bala(&i, false);
                }
            }
        }
    }
}