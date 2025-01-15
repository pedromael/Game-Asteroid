#include "../header/novos.h"

bool criar_inimigo(SDL_Renderer *render ,nave_inimiga *inimigo, int nivel){
    //return false;
    if(numero_inimigos >= MAXIMO_INIMIGOS)
    return false;
    
    int i,size = TAMANHO_NAVE;
    for (i = 0; i < 500; i++){
        if(!item_colidiu(&i,&i,&size,"inimigo"))
            break;
    }

    if (i == 500)
        return false;

    if(numero_inimigos >= capacidade_inimigos){
        if (capacidade_inimigos + 5 >= MAXIMO_INIMIGOS)
            capacidade_inimigos = MAXIMO_INIMIGOS;
        else
            capacidade_inimigos += 5;
        
        inimigos = realloc(inimigos,  capacidade_inimigos * sizeof(inimigos));
    }

    nivel = (rand() % 3) + 1;
    inimigo->textura = textura_inimigos[nivel-1];
    if (nivel == 1){
        inimigo->arma = arsenal[0];
    }
    else if (nivel == 2){
        inimigo->arma = arsenal[1];
    }
    else if (nivel == 3){
        inimigo->arma = arsenal[4];
    }

    if (!inimigo->textura)
        return false; // nao foi possivel carregar a textura.
    

    inimigo->Rect.x = i;
    inimigo->Rect.y = i;
    inimigo->Rect.h = TAMANHO_NAVE + 10 * nivel;
    inimigo->Rect.w = TAMANHO_NAVE + 10 * nivel;
    inimigo->dx = 1;
    inimigo->dy = 0;
    inimigo->vida = 50*nivel;
    inimigo->velocidade = VELOCIDADE_INIMIGA;
    inimigo->ultima_ronda = 0;
    inimigo->tempo_ronda = 4;

    numero_inimigos++;
    return true;
}

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
    meteoros[numero_meteoros].danos = size + 30;
    meteoros[numero_meteoros].vida = (rand() % size) + 75;
    meteoros[numero_meteoros].velocidade = (rand() % 6) + 2;
    meteoros[numero_meteoros].status = true;

    meteoros[numero_meteoros].textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho1.png"));

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
    explosoes[i].milissegundo_inicio = SDL_GetTicks();

    if (tipo == 1) // para um bala
    {
        explosoes[i].tempo = 200;
        explosoes[i].textura = textura_explosao_bala;
    }else if(tipo == 2){ // para um e parede
        explosoes[i].tempo = 2*1000;
        explosoes[i].textura = textura_explosao_parede;
    }else if(tipo == 3){ // para um inimigo
        explosoes[i].tempo = 2*1000;
        explosoes[i].textura = textura_explosao_inimigo;
    }
    
    numero_explosoes++;
    return true;
}

bool criar_pacote(meteoro *met){
    pacotes[numero_pacotes].rect.x = met->rect.x + (met->rect.w/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.y = met->rect.y + (met->rect.h/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.w = TAMANHO_PACOTE;
    pacotes[numero_pacotes].rect.h = TAMANHO_PACOTE;

    pacotes[numero_pacotes].tipo = rand()%3;

    if(pacotes[numero_pacotes].tipo == 0){
        pacotes[numero_pacotes].valor = (rand()%70) + 20;
    }
    if(pacotes[numero_pacotes].tipo == 1){
        pacotes[numero_pacotes].valor = (rand()%(numero_armas-1)) + 1;
    }
    if(pacotes[numero_pacotes].tipo == 2){
        pacotes[numero_pacotes].valor = (rand()%200) + 100;
    }
    numero_pacotes++;
    return true;
}