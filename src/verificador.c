#include "../header/verificador.h"

bool calcular_probabilidade(int perc){
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
        bool colisaoHorizontal = (inimigos[j].Rect.x < *x + *size) && (inimigos[j].Rect.x + inimigos[j].Rect.w > *x);
        bool colisaoVertical = (inimigos[j].Rect.y < *y + *size) && (inimigos[j].Rect.y + inimigos[j].Rect.h > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 2;
    }
    return 0;
}

bool inimigo_colidiu(int i){
    int res = colidiu_nas_bordas(&inimigos[i].Rect.x, &inimigos[i].Rect.y, &inimigos[i].Rect.w);
    if (res)
        return true;
    
    for (size_t j = 0; j < numero_inimigos; j++){ // verificar se ha colisao entre naves inimigas
        if (i != j){
            bool colisaoHorizontal = (inimigos[j].Rect.x < inimigos[i].Rect.x + inimigos[i].Rect.w) && (inimigos[j].Rect.x + inimigos[j].Rect.w > inimigos[i].Rect.x);
            bool colisaoVertical = (inimigos[j].Rect.y < inimigos[i].Rect.y + inimigos[i].Rect.h) && (inimigos[j].Rect.y + inimigos[j].Rect.h > inimigos[i].Rect.y);
            if (colisaoHorizontal && colisaoVertical)
                return true;
        }
    }

    for (size_t j = 0; j < numero_obstaculos; j++) // verificar se a colisao com obstaculos
    {
        bool colisaoHorizontal = (obstaculos[j].x < inimigos[i].Rect.x + inimigos[i].Rect.w) && (obstaculos[j].x + obstaculos[j].sizeX > inimigos[i].Rect.x);
        bool colisaoVertical = (obstaculos[j].y < inimigos[i].Rect.y + inimigos[i].Rect.h) && (obstaculos[j].y + obstaculos[j].sizeY > inimigos[i].Rect.y);
        if (colisaoHorizontal && colisaoVertical)
            return 1;
    }

    bool colisaoHorizontal = (inimigos[i].Rect.x < player.x + player.size) && (inimigos[i].Rect.x + inimigos[i].Rect.w > player.x);
    bool colisaoVertical = (inimigos[i].Rect.y < player.y + player.size) && (inimigos[i].Rect.y + inimigos[i].Rect.h > player.y);
    if (colisaoHorizontal && colisaoVertical)
        return true;

    return false;
}

bool criar_inimigo(SDL_Renderer *render ,nave_inimiga *inimigo, int nivel){
    int i,size = TAMANHO_NAVE;
    for (i = 0; i < 500; i++){
        if(!item_colidiu(&i,&i,&size))
            break;
    }

    if (i == 500)
        return false;

    nivel = (rand() % 3) + 1;

    if (nivel == 1){
        inimigo->textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/nave1.png"));
        inimigo->arma = arsenal[0];
    }
    else if (nivel == 2){
        inimigo->textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/nave2.png"));
        inimigo->arma = arsenal[1];
    }
    else if (nivel == 3){
        inimigo->textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/nave3.png"));
        inimigo->arma = arsenal[2];
    }

    if (!inimigo->textura)
    {
        numero_inimigos--;
        return false;;
    }
    

    inimigo->Rect.x = i;
    inimigo->Rect.y = i;
    inimigo->Rect.h = TAMANHO_NAVE;
    inimigo->Rect.w = TAMANHO_NAVE;
    inimigo->dx = 1;
    inimigo->dy = 0;
    inimigo->vida = 50*nivel;
    inimigo->velocidade = VELOCIDADE_INIMIGA;
    inimigo->ultima_ronda = 0;
    inimigo->tempo_ronda = 4;

    return true;
}

bool disparar(nave_inimiga *inimigos){

        int x = player.x;
        int y = player.y;
        int dx = player.dx;
        int dy = player.dy;
        armas *arma = &player.arma;

    if(inimigos != NULL){
        x = inimigos->Rect.x;
        y = inimigos->Rect.y;
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

    tiros[numero_tiros].x = x + TAMANHO_NAVE/2 - arma->bala_size/2;
    tiros[numero_tiros].y = y + TAMANHO_NAVE/2 - arma->bala_size/2;
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
    int margen_de_erro = 20;
    margen_de_erro = (rand() % margen_de_erro) - (rand() % margen_de_erro);

    int x = inimigos[*i].Rect.x - (player.x + margen_de_erro);
    int y = inimigos[*i].Rect.y - (player.y + margen_de_erro);

    if (inimigos[*i].ultima_ronda + inimigos[*i].tempo_ronda*60 <= quadros || inimigos[*i].ultima_ronda == 0)
    {
        inimigos[*i].ultima_ronda = quadros;

        if (abs(x) < abs(y))
        {
            inimigos[*i].dy = 0;
            inimigos[*i].dx = inimigos[*i].Rect.x - player.x > 0 ? -1 : 1;
            if(calcular_probabilidade(20)){
                inimigos[*i].dy = inimigos[*i].Rect.y - player.y > 0 ? -1 : 1;
                inimigos[*i].dx = 0;
                return abs(y);
            }
            return abs(x);
        }else{
            inimigos[*i].dx = 0;
            inimigos[*i].dy = inimigos[*i].Rect.y - player.y > 0 ? -1 : 1;
            if(calcular_probabilidade(20)){
                inimigos[*i].dx = inimigos[*i].Rect.x - player.x > 0 ? -1 : 1;
                inimigos[*i].dy = 0;
                return abs(x);
            }
            return abs(y);
        }
    }else{
        if (inimigos[*i].dx != 0)
        {
            inimigos[*i].dy = 0;
            inimigos[*i].dx = inimigos[*i].Rect.x - player.x > 0 ? -1 : 1;
            return abs(x);
        }else{
            inimigos[*i].dx = 0;
            inimigos[*i].dy = inimigos[*i].Rect.y - player.y > 0 ? -1 : 1;
            return abs(y);
        }
    }

    return inimigos[*i].dx ? x : y;
}

int actualiazar_inimigos(){
    for (int i = 0; i < numero_inimigos; i++)
    {
        if (area_de_impacto_mira(&i) < 150){
            if (inimigos[i].dx != 0)
            {
                int buffer_dx = inimigos[i].dx;
                inimigos[i].dx = 0;
                inimigos[i].dy = inimigos[i].Rect.y - player.y > 0 ? -1 : 1;
                disparar(&inimigos[i]);
                inimigos[i].dy = 0;
                inimigos[i].dx = buffer_dx;
            }else{
                int buffer_dy = inimigos[i].dy;
                inimigos[i].dy = 0;
                inimigos[i].dx = inimigos[i].Rect.x - player.x > 0 ? -1 : 1;
                disparar(&inimigos[i]);
                inimigos[i].dx = 0;
                inimigos[i].dy = buffer_dy;
            }
        }

        bool mover = true;
        if (area_de_impacto_mira(&i) < 15)
            mover = calcular_probabilidade(50);
        

        if(mover){
            if (inimigos[i].dx != 0)
            {
                inimigos[i].Rect.x += VELOCIDADE_INIMIGA * inimigos[i].dx;
                if (inimigo_colidiu(i)){
                    inimigos[i].Rect.x -= VELOCIDADE_INIMIGA * inimigos[i].dx;
                }
                
            }else{
                inimigos[i].Rect.y += VELOCIDADE_INIMIGA * inimigos[i].dy;
                if (inimigo_colidiu(i))
                {
                    inimigos[i].Rect.y -= VELOCIDADE_INIMIGA * inimigos[i].dy;
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
            bool coliX = (inimigos[i].Rect.x < tiros[j].x + tiros[j].arma.bala_size) && (inimigos[i].Rect.x + inimigos[i].Rect.w > tiros[j].x);
            bool coliY = (inimigos[i].Rect.y < tiros[j].y + tiros[j].arma.bala_size) && (inimigos[i].Rect.y + inimigos[i].Rect.h > tiros[j].y);
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