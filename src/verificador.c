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
    for (size_t j = 0; j < numero_obstaculos; j++) // verificar se a colisao com obstaculos
    {
        bool colisaoHorizontal = (obstaculos[j].x < *x + *size) && (obstaculos[j].x + obstaculos[j].sizeX > *x);
        bool colisaoVertical = (obstaculos[j].y < *y + *size) && (obstaculos[j].y + obstaculos[j].sizeY > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 1;
    }
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

    for (size_t j = 0; j < numero_obstaculos; j++) // verificar se a colisao com obstaculos
    {
        bool colisaoHorizontal = (obstaculos[j].x < inimigos[i].x + inimigos[i].size) && (obstaculos[j].x + obstaculos[j].sizeX > inimigos[i].x);
        bool colisaoVertical = (obstaculos[j].y < inimigos[i].y + inimigos[i].size) && (obstaculos[j].y + obstaculos[j].sizeY > inimigos[i].y);
        if (colisaoHorizontal && colisaoVertical)
            return 1;
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

        int x = player.x;
        int y = player.y;
        int dx = player.dx;
        int dy = player.dy;
        armas *arma = &player.arma;

    if(inimigos != NULL){
        x = inimigos->x;
        y = inimigos->y;
        dx = inimigos->dx;
        dy = inimigos->dy;
        arma = &inimigos->arma;
    }

    if (!arma->no_pente)
    {
        if(arma->inicio_carregamento){
            if (quadros - arma->inicio_carregamento >= arma->tempo_carregamento*60){
                arma->no_pente = arma->pente_max;
                arma->inicio_carregamento = false;
            }
        }else
            arma->inicio_carregamento = quadros;

        return false;
    }

    if(arma->ultimo_tiro != 0)
        if (arma->ultimo_tiro + (60 / arma->bps) >= quadros)
            return false;

    if (numero_tiros >= capacidade_tiros){
        capacidade_tiros += 5; 
        tiros = realloc(tiros, capacidade_tiros * sizeof(tiro));
    }

    tiros[numero_tiros].x = x + TAMANHO_NAVE/2;
    tiros[numero_tiros].y = y + TAMANHO_NAVE/2;
    tiros[numero_tiros].dx = dx;
    tiros[numero_tiros].dy = dy;
    tiros[numero_tiros].arma = *arma;
    if (inimigos != NULL)
        tiros[numero_tiros].inimigo = 1;
    else
        tiros[numero_tiros].inimigo = 0;
    
    numero_tiros++;
    arma->no_pente--;
    arma->ultimo_tiro = quadros; 

    return true;
}

void retirar_tiro(int *i){
    if (*i !=  --numero_tiros)
        tiros[*i] = tiros[numero_tiros];
}

int area_de_impacto_mira(int *i){
    int margen_de_erro = 30;
    margen_de_erro = (rand() % margen_de_erro) - (rand() % margen_de_erro);

    int x = inimigos[*i].x - (player.x + margen_de_erro);
    int y = inimigos[*i].y - (player.y + margen_de_erro);

    if (inimigos[*i].ultima_ronda + inimigos[*i].tempo_ronda*60 <= quadros)
    {
        inimigos[*i].ultima_ronda = quadros;

        if (abs(x) < abs(y))
        {
            inimigos[*i].dy = 0;
            if (x < 0)
                inimigos[*i].dx = 1;
            else
                inimigos[*i].dx = -1;
            return abs(x);
        }else{
            inimigos[*i].dx = 0;
            if (y < 0)
                inimigos[*i].dy = 1;
            else
                inimigos[*i].dy= -1;
            return abs(y);
        }
    }

    return abs(abs(x) - abs(y));
}

int actualiazar_inimigos(){
    for (int i = 0; i < numero_inimigos; i++)
    {
        if (area_de_impacto_mira(&i) < 30){
            if (inimigos[i].dx)
            {
                inimigos[i].dx = 0;
                if (inimigos[i].y - player.y > 0)
                    inimigos[i].dy = -1;
                else
                    inimigos[i].dy = 1;
                
            }else{
                inimigos[i].dy = 0;
                if (inimigos[i].x - player.x > 0)
                    inimigos[i].dx = -1;
                else
                    inimigos[i].dx = 1;
            }
            
            disparar(&inimigos[i]);
        }

        bool mover = true;
        if (area_de_impacto_mira(&i) < 15)
            mover = calcular_probabilidade(50);
        

        if(mover){
            if (inimigos[i].dx)
            {
                inimigos[i].x += VELOCIDADE_INIMIGA * inimigos[i].dx;
                if (inimigo_colidiu(i)){
                    inimigos[i].x -= VELOCIDADE_INIMIGA * inimigos[i].dx;
                }
                
            }else{
                inimigos[i].y += VELOCIDADE_INIMIGA * inimigos[i].dy;
                if (inimigo_colidiu(i))
                {
                    inimigos[i].y -= VELOCIDADE_INIMIGA * inimigos[i].dy;
                }
            }
        }
    }
    return 1;
}

void actualizar_tiros(){
    for (int i = 0; i < numero_tiros; i++)
        if (!item_colidiu(&tiros[i].x,&tiros[i].y,&tiros[i].arma.bala_size))
        {
            tiros[i].x += tiros[i].dx * tiros[i].arma.bala_velocidade;
            tiros[i].y += tiros[i].dy * tiros[i].arma.bala_velocidade;
        }else
            retirar_tiro(&i);
}

void verificar_atingidos(){
    for (int j = 0; j < numero_tiros; j++)
    {
        for (int i = 0; i < numero_inimigos; i++)
        {
            bool coliX = (inimigos[i].x < tiros[j].x + tiros[j].arma.bala_size) && (inimigos[i].x + inimigos[i].size > tiros[j].x);
            bool coliY = (inimigos[i].y < tiros[j].y + tiros[j].arma.bala_size) && (inimigos[i].y + inimigos[i].size > tiros[j].y);
            if (coliX && coliY && !tiros[j].inimigo){
                retirar_tiro(&j);
                if (inimigos[i].vida - tiros[j].arma.danos > 0)
                    inimigos[i].vida -= tiros[j].arma.danos;
                else
                    if (i != --numero_inimigos)
                        inimigos[i] = inimigos[numero_inimigos];
                break;
            }
        }

        bool coliX = (tiros[j].x < player.x + player.size) && (tiros[j].x + tiros[j].arma.bala_size > player.x);
        bool coliy = (tiros[j].y < player.y + player.size) && (tiros[j].y + tiros[j].arma.bala_size > player.y);
        if (coliX && coliy && tiros[j].inimigo){
            retirar_tiro(&j);
            if (player.vida - tiros[j].arma.danos > 0)
                player.vida -= tiros[j].arma.danos;
            else
                player_status = false;
        }

        for (size_t i = 0; i < numero_obstaculos; i++)
        {
            bool coliX = (tiros[j].x < obstaculos[i].x + obstaculos[i].sizeX) && (tiros[j].x + tiros[j].arma.bala_size > obstaculos[i].x);
            bool coliy = (tiros[j].y < obstaculos[i].y + obstaculos[i].sizeY) && (tiros[j].y + tiros[j].arma.bala_size > obstaculos[i].y);
            if (coliX && coliy){
                retirar_tiro(&j);
                if (obstaculos[i].vida - tiros[j].arma.danos > 0)
                    obstaculos[i].vida -= tiros[j].arma.danos;
                else
                    if (i != --numero_obstaculos)
                        obstaculos[i] = obstaculos[numero_obstaculos];
                break;
            }
        }
    }
}

void actualizar_jogo(){
    actualiazar_inimigos();
    verificar_atingidos();
    actualizar_tiros();
}