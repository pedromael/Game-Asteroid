#include "../header/verificador.h"

int calcular_probabilidade(int perc){
    if ((rand() % 100) <= perc)
        return 1;
    return 0;
}

bool colidiu_nas_bordas(int *x, int *y, int *size){
    if (*x < 0 || *y < 0)
        return true;
    
    if (*x + *size > LARGURA || *y + *size > ALTURA)
        return true;
    return false;
}

int item_colidiu(int *x, int *y, int *size){
    if(colidiu_nas_bordas(x,y,size))
        return 1;
    for (size_t j = 0; j < numero_inimigos; j++) // verificar se a colisao entre naves inimigas
    {
        bool colisaoHorizontal = (inimigos[j].x < *x + *size) && (inimigos[j].x + inimigos[j].size > *x);
        bool colisaoVertical = (inimigos[j].y < *y + *size) && (inimigos[j].y + inimigos[j].size > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 2;
    }
    return 0;
}

bool inimigo_colidiu(int i){
    int res = item_colidiu(&inimigos[i].x, &inimigos[i].y, &inimigos[i].size);
    if (res == 1)
        return true;
    
    for (size_t j = 0; j < numero_inimigos; j++){ // verificar se a colisao entre naves inimigas
        if (i != j){
            bool colisaoHorizontal = (inimigos[j].x < inimigos[i].x + inimigos[i].size) && (inimigos[j].x + inimigos[j].size > inimigos[i].x);
            bool colisaoVertical = (inimigos[j].y < inimigos[i].y + inimigos[i].size) && (inimigos[j].y + inimigos[j].size > inimigos[i].y);
            if (colisaoHorizontal && colisaoVertical)
                return true;
        }
    }

    bool colisaoHorizontal = (inimigos[i].x < player.x + player.size) && (inimigos[i].x + inimigos[i].size > player.x);
    bool colisaoVertical = (inimigos[i].y < player.y + player.size) && (inimigos[i].y + inimigos[i].size > player.y);
    if (colisaoHorizontal && colisaoVertical)
        return true;

    return false;
}

void criar_inimigo(nave_inimiga *inimigo, int nivel){
    int i,size = TAMANHO_NAVE;
    for (i = 0; i < 500; i++)
        if(!inimigo_colidiu(i))
            break;
    
    inimigo->x = i;
    inimigo->y = i;
    inimigo->size = TAMANHO_NAVE;
    inimigo->dx = 1;
    inimigo->dy = 0;
    inimigo->vida = size;
    inimigo->velocidade = VELOCIDADE_INIMIGA;
    inimigo->arma = arsenal[0];
}

bool dispar(nave_inimiga *inimigos){
    if (calcular_probabilidade(50))
    {
        return 1;
    }
    return 0;
}

int actualiazar_inimigos(){
    for (size_t i = 0; i < numero_inimigos; i++)
    {
        if (dispar(&inimigos[i]))
            return 0;
        
        if (calcular_probabilidade(50))
        {
            inimigos[i].dx = 1;
            inimigos[i].dy = 0;
            inimigos[i].x += VELOCIDADE_INIMIGA;
            if (item_colidiu(&inimigos[i].x,&inimigos[i].y,&inimigos[i].size))
            {
                inimigos[i].x -= VELOCIDADE_INIMIGA*2;
                if (item_colidiu(&inimigos[i].x,&inimigos[i].y,&inimigos[i].size))
                {
                    inimigos[i].x += VELOCIDADE_INIMIGA;
                    return true; 
                }
            }
            
        }else
        {
            inimigos[i].dx = 0;
            inimigos[i].dy = 1;
            inimigos[i].y += VELOCIDADE_INIMIGA;
            if (item_colidiu(&inimigos[i].x,&inimigos[i].y,&inimigos[i].size))
            {
                inimigos[i].y -= VELOCIDADE_INIMIGA*2;
                if (item_colidiu(&inimigos[i].x,&inimigos[i].y,&inimigos[i].size))
                {
                    inimigos[i].y += VELOCIDADE_INIMIGA;
                    return true; 
                }
            }
        }
    }
    return 1;
}

void actualizar_jogo(){
    actualiazar_inimigos();
}