#include "../header/novos.h"

bool criar_meteoro(SDL_Renderer *render)
{
    if(numero_meteoros >= capacidade_meteoros)
        meteoros = realloc(meteoros, ++capacidade_meteoros * sizeof(meteoro));
    
    int size = (rand() % 180) + 20;
    char pos[8];
    int bufferX,bufferY;

    if (calcular_probabilidade(50))
    {
        if (calcular_probabilidade(50)){
            meteoros[numero_meteoros].rect.x = -size;
            bufferY = rand() % ALTURA;
            meteoros[numero_meteoros].rect.y = bufferY;
            sprintf(pos,"left");
        }else{
            meteoros[numero_meteoros].rect.y = -size;
            bufferX = rand() % LARGURA;
            meteoros[numero_meteoros].rect.x = bufferX;
            sprintf(pos,"up");
        }
    }else{
        if (calcular_probabilidade(50)){
            meteoros[numero_meteoros].rect.x = LARGURA;
            bufferY = rand() % ALTURA;
            meteoros[numero_meteoros].rect.y = bufferY;
            sprintf(pos,"rigth");
        }else{
            meteoros[numero_meteoros].rect.y = ALTURA;
            bufferX = rand() % LARGURA;
            meteoros[numero_meteoros].rect.x = bufferX;
            sprintf(pos,"down");
        }
    }
    
    meteoros[numero_meteoros].rect.w = size;
    meteoros[numero_meteoros].rect.h = size;
    meteoros[numero_meteoros].danos = size + 50;
    meteoros[numero_meteoros].vida = (rand() % size) + 75;
    meteoros[numero_meteoros].velocidade = (rand() % 8) + 2;
    meteoros[numero_meteoros].status = true;

    meteoros[numero_meteoros].textura = textura_meteoro;

    if(!meteoros[numero_meteoros].textura){
        return false;
    }

    if(strcmp(pos,"left") == 0){
        meteoros[numero_meteoros].dx = 1;
        meteoros[numero_meteoros].dy = 0;
    }else if(strcmp(pos,"up") == 0){
        meteoros[numero_meteoros].dy = 1;
        meteoros[numero_meteoros].dx = 0;
    }else if(strcmp(pos,"rigth") == 0){
        meteoros[numero_meteoros].dx = -1;
        meteoros[numero_meteoros].dy = 0;
    }else if(strcmp(pos,"down") == 0){
        meteoros[numero_meteoros].dx = 0;
        meteoros[numero_meteoros].dy = -1;
    }    

    numero_meteoros++;
    return true;
}

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

bool criar_pacote(meteoro *met){
    if (numero_pacotes >= MAXIMO_PACOTES)
       return false;
    
    pacotes[numero_pacotes].rect.x = met->rect.x + (met->rect.w/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.y = met->rect.y + (met->rect.h/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.w = TAMANHO_PACOTE;
    pacotes[numero_pacotes].rect.h = TAMANHO_PACOTE;

    if (colidiu_nas_bordas(pacotes[numero_pacotes].rect))
        return false;

    pacotes[numero_pacotes].tipo = rand()%4;

    if(pacotes[numero_pacotes].tipo == 0){ // para vida
        pacotes[numero_pacotes].valor = (rand()%70) + 20;
        pacotes[numero_pacotes].textura = textura_pacotes[0];
    }
    if(pacotes[numero_pacotes].tipo == 1){ // para armas
        pacotes[numero_pacotes].valor = (rand()%(NUMERO_DE_ARMAS-1)) + 1;
        pacotes[numero_pacotes].textura = textura_pacotes[1];
    }
    if(pacotes[numero_pacotes].tipo == 2){ // para scudo
        pacotes[numero_pacotes].valor = (rand()%200) + 100;
        pacotes[numero_pacotes].textura = textura_pacotes[2];
    }
    if(pacotes[numero_pacotes].tipo == 3){
        if(calcular_probabilidade(65))
            pacotes[numero_pacotes].valor = 1;
        else
            pacotes[numero_pacotes].valor = (rand()%3) + 1;
        pacotes[numero_pacotes].textura = textura_pacotes[3];
    }
    numero_pacotes++;
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
