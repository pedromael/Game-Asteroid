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
    for (size_t j = 0; j < numero_inimigos; j++) // verificar se a colisao com nave inimiga
    {
        bool colisaoHorizontal = (inimigos[j].x < *x + *size) && (inimigos[j].x + inimigos[j].size > *x);
        bool colisaoVertical = (inimigos[j].y < *y + *size) && (inimigos[j].y + inimigos[j].size > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 2;
    }
    return 0;
}

bool inimigo_colidiu(int i){
    int res = colidiu_nas_bordas(&inimigos[i].x, &inimigos[i].y, &inimigos[i].size);
    if (res)
        return true;
    
    for (size_t j = 0; j < numero_inimigos; j++){ // verificar se ha colisao entre naves inimigas
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

bool criar_inimigo(nave_inimiga *inimigo, int nivel){
    int i,size = TAMANHO_NAVE;
    for (i = 0; i < 500; i++){
        printf("%d\n", item_colidiu(&i,&i,&size));
        if(!item_colidiu(&i,&i,&size))
            break;
    }

    if (i == 500)
        return false;
    

    inimigo->x = i;
    inimigo->y = i;
    inimigo->size = TAMANHO_NAVE;
    inimigo->dx = 1;
    inimigo->dy = 0;
    inimigo->vida = 50;
    inimigo->velocidade = VELOCIDADE_INIMIGA;
    inimigo->arma = arsenal[0];

    return true;
}

bool disparar(nave_inimiga *inimigos){
    
    int x;
    int y;
    int dx;
    int dy;
    armas arma;
    
    if (inimigos == NULL) // nave player a disparar.
    {
        x = player.x;
        y = player.y;
        dx = player.dx;
        dy = player.dy;
        arma = player.arma;
    }else{
        x = inimigos->x;
        y = inimigos->y;
        dx = inimigos->dx;
        dy = inimigos->dy;
        arma = inimigos->arma;
    }
    

    if (calcular_probabilidade(50))
    {
        if (numero_tiros >= capacidade_tiros){
            capacidade_tiros += 5; 
            tiros = realloc(tiros, capacidade_tiros * sizeof(tiro));
        }

        numero_tiros += 1;
        tiros[numero_tiros-1].x = x;
        tiros[numero_tiros-1].y = y;
        tiros[numero_tiros-1].dx = dx;
        tiros[numero_tiros-1].dy = dy;
        tiros[numero_tiros-1].arma = arma;
        if (inimigos != NULL)
            tiros[numero_tiros - 1].inimigo = 1;
        else
            tiros[numero_tiros - 1].inimigo = 0;
        
        return 1;
    }
    return 1;
}

int actualiazar_inimigos(){
    for (size_t i = 0; i < numero_inimigos; i++)
    {
        if (disparar(&inimigos[i]))
            return 0;
        
        if (calcular_probabilidade(50))
        {
            inimigos[i].dx = 1;
            inimigos[i].dy = 0;
            inimigos[i].x += VELOCIDADE_INIMIGA;
            if (inimigo_colidiu(i))
            {
                inimigos[i].x -= VELOCIDADE_INIMIGA*2;
                if (inimigo_colidiu(i))
                {
                    inimigos[i].x += VELOCIDADE_INIMIGA;
                    return true; 
                }
            }
            
        }else{
            inimigos[i].dx = 0;
            inimigos[i].dy = 1;
            inimigos[i].y += VELOCIDADE_INIMIGA;
            if (inimigo_colidiu(i))
            {
                inimigos[i].y -= VELOCIDADE_INIMIGA*2;
                if (inimigo_colidiu(i))
                {
                    inimigos[i].y += VELOCIDADE_INIMIGA;
                    return true; 
                }
            }
        }
    }
    return 1;
}

void actualizar_tiros(){
    for (size_t i = 0; i < numero_tiros; i++)
    {
        if (!item_colidiu(&tiros[i].x,&tiros[i].y,&tiros[i].arma.bala_size))
        {
            tiros[i].x += tiros[i].dx * tiros[i].arma.bps;
            tiros[i].y += tiros[i].dy * tiros[i].arma.bps;
        }
    }
}

void verificar_atingidos(){
    for (size_t i = 0; i < numero_inimigos; i++)
    {
        for (size_t j = 0; j < numero_tiros; j++)
        {
            bool colisaoHorizontal = (inimigos[i].x < tiros[j].x + tiros[j].arma.bala_size) && (inimigos[i].x + inimigos[i].size > tiros[j].x);
            bool colisaoVertical = (inimigos[i].y < tiros[j].y + tiros[j].arma.bala_size) && (inimigos[i].y + inimigos[i].size > tiros[j].y);
            if (colisaoHorizontal && colisaoVertical && !tiros[j].inimigo){
                if (inimigos[i].vida - tiros[j].arma.danos > 0){
                    inimigos[i].vida -= tiros[j].arma.danos;
                }else{
                    numero_inimigos--;
                    if (i != numero_inimigos){
                        inimigos[i] = inimigos[numero_inimigos];
                    }
                }
            }
        }
    }
}

void actualizar_jogo(){
    actualiazar_inimigos();
    actualizar_tiros();
    verificar_atingidos();
}