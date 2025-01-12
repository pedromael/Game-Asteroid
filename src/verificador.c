#include "../header/verificador.h"

bool calcular_probabilidade(int perc){
    if ((rand() % 101) <= perc)
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

bool meteoro_colidio_nas_borda(int *x, int *y, int *size){
    if (*x < -(*size + 2) || *y < -(*size + 2))
        return true;
    
    if (*x > LARGURA + 2 || *y > ALTURA + 2)
        return true;
    return false;
}

int colidiram(SDL_Rect *rect1, SDL_Rect *rect2){
    bool coliX = (rect1->x < rect2->x + rect2->w) && (rect1->x + rect1->w > rect2->x);
    bool coliY = (rect1->y < rect2->y + rect2->h) && (rect1->y + rect1->h > rect2->y);
    if (coliX && coliY)
        return true;
    return false;
}

int item_colidiu(int *x, int *y, int *size, char *item){
    if (strcmp(item, "meteoro") != 0){
        if(colidiu_nas_bordas(x,y,size))
            return 1;
    }else
        if(meteoro_colidio_nas_borda(x,y,size))
            return 1;

    for (size_t j = 0; j < numero_obstaculos; j++) // verificar se a colisao com obstaculos
    {
        bool colisaoHorizontal = (obstaculos[j].rect.x < *x + *size) && (obstaculos[j].rect.x + obstaculos[j].rect.w > *x);
        bool colisaoVertical = (obstaculos[j].rect.y < *y + *size) && (obstaculos[j].rect.y + obstaculos[j].rect.h > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 1;
    }
    if (strcmp(item, "bala") != 0)
        for (size_t j = 0; j < numero_inimigos; j++) // verificar se a colisao com nave inimiga
        {
            bool colisaoHorizontal = (inimigos[j].Rect.x < *x + *size) && (inimigos[j].Rect.x + inimigos[j].Rect.w > *x);
            bool colisaoVertical = (inimigos[j].Rect.y < *y + *size) && (inimigos[j].Rect.y + inimigos[j].Rect.h > *y);
            if (colisaoHorizontal && colisaoVertical)
                return 2;
        }
    if (strcmp(item, "meteoro") != 0)
        for (size_t j = 0; j < numero_meteoros; j++)
        {
            bool colisaoHorizontal = (meteoros[j].rect.x < *x + *size) && (meteoros[j].rect.x + meteoros[j].rect.w > *x);
            bool colisaoVertical = (meteoros[j].rect.y < *y + *size) && (meteoros[j].rect.y + meteoros[j].rect.h > *y);
            if (colisaoHorizontal && colisaoVertical)
                return 1;
        }
    
    if (strcmp(item, "player") != 0)
    {
        bool colisaoHorizontal = (player.scudo.rect.x < *x + *size) && (player.scudo.rect.x + player.scudo.rect.w > *x);
        bool colisaoVertical = (player.scudo.rect.y < *y + *size) && (player.scudo.rect.y + player.scudo.rect.h > *y);
        if (colisaoHorizontal && colisaoVertical)
            return 1;
    }
    

    return 0;
}

bool inimigo_colidiu(int i){
    int res = colidiu_nas_bordas(&inimigos[i].Rect.x, &inimigos[i].Rect.y, &inimigos[i].Rect.w);
    if (res)
        return true;
    
    for (size_t j = 0; j < numero_inimigos; j++) // verificar se ha colisao entre naves inimigas
        if (i != j){
            if (colidiram(&inimigos[i].Rect,&inimigos[j].Rect))
                return true;
        }

    for (size_t j = 0; j < numero_obstaculos; j++) // verificar se a colisao com obstaculos
        if (colidiram(&obstaculos[j].rect, &inimigos[i].Rect))
            return 1;

    if (colidiram(&inimigos[i].Rect, &player.rect))
        return true;

    return false;
}

bool disparar(nave_inimiga *inimigos){
    int x,y,dx,dy;
    armas *arma;

    if(inimigos != NULL){
        x = inimigos->Rect.x;
        y = inimigos->Rect.y;
        dx = inimigos->dx;
        dy = inimigos->dy;
        arma = &inimigos->arma;
    }else{
        x = player.rect.x;
        y = player.rect.y;
        dx = player.dx;
        dy = player.dy;
        arma = &player.arma[player.arma_select];
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

    tiros[numero_tiros].rect.x = x + TAMANHO_NAVE/2 - arma->bala_size/2;
    tiros[numero_tiros].rect.y = y + TAMANHO_NAVE/2 - arma->bala_size/2;
    tiros[numero_tiros].rect.w = arma->bala_size;
    tiros[numero_tiros].rect.h = arma->bala_size;
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

    Mix_PlayChannel(-1, tiros[numero_tiros-1].arma.som, 0);
    return true;
}

void ativar_scudo(){
    if (!player.scudo.ativo)
        if(player.scudo.vida <= 0)
            return;
    player.scudo.ativo = true;

    int size = 20;

    player.scudo.rect.x = player.rect.x - size;
    player.scudo.rect.y = player.rect.y - size;
    player.scudo.rect.w = TAMANHO_NAVE + size * 2;
    player.scudo.rect.h = TAMANHO_NAVE + size * 2;
}

void desativar_scudo(){
    if (player.scudo.ativo)
        player.scudo.ativo = false;
}

void retirar_tiro(int *i, bool explosao){
    if (explosao)
    {
        tiros[*i].rect.w += 4;
        tiros[*i].rect.h += 4;

        criar_explosao(render, 1, tiros[*i].rect);
    }

    if (*i !=  --numero_tiros)
        tiros[*i] = tiros[numero_tiros];
}

int area_de_impacto_mira(int *i){
    int margen_de_erro = 20;
    margen_de_erro = (rand() % margen_de_erro) - (rand() % margen_de_erro);

    int x = inimigos[*i].Rect.x - (player.rect.x + margen_de_erro);
    int y = inimigos[*i].Rect.y - (player.rect.y + margen_de_erro);

    if (inimigos[*i].ultima_ronda + inimigos[*i].tempo_ronda*60 <= quadros || inimigos[*i].ultima_ronda == 0)
    {
        inimigos[*i].ultima_ronda = quadros;

        if (abs(x) < abs(y))
        {
            inimigos[*i].dy = 0;
            inimigos[*i].dx = inimigos[*i].Rect.x - player.rect.x > 0 ? -1 : 1;
            if(calcular_probabilidade(20)){
                inimigos[*i].dy = inimigos[*i].Rect.y - player.rect.y > 0 ? -1 : 1;
                inimigos[*i].dx = 0;
                return abs(y);
            }
            return abs(x);
        }else{
            inimigos[*i].dx = 0;
            inimigos[*i].dy = inimigos[*i].Rect.y - player.rect.y > 0 ? -1 : 1;
            if(calcular_probabilidade(20)){
                inimigos[*i].dx = inimigos[*i].Rect.x - player.rect.x > 0 ? -1 : 1;
                inimigos[*i].dy = 0;
                return abs(x);
            }
            return abs(y);
        }
    }else{
        if (inimigos[*i].dx != 0)
        {
            inimigos[*i].dy = 0;
            inimigos[*i].dx = inimigos[*i].Rect.x - player.rect.x > 0 ? -1 : 1;
            return abs(x);
        }else{
            inimigos[*i].dx = 0;
            inimigos[*i].dy = inimigos[*i].Rect.y - player.rect.y > 0 ? -1 : 1;
            return abs(y);
        }
    }
}

int actualiazar_inimigos(){
    for (int i = 0; i < numero_inimigos; i++)
    {
        if (area_de_impacto_mira(&i) < 150){
            if (inimigos[i].dx != 0)
            {
                int buffer_dx = inimigos[i].dx;
                inimigos[i].dx = 0;
                inimigos[i].dy = inimigos[i].Rect.y - player.rect.y > 0 ? -1 : 1;
                disparar(&inimigos[i]);
                inimigos[i].dy = 0;
                inimigos[i].dx = buffer_dx;
            }else{
                int buffer_dy = inimigos[i].dy;
                inimigos[i].dy = 0;
                inimigos[i].dx = inimigos[i].Rect.x - player.rect.x > 0 ? -1 : 1;
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
        if (!item_colidiu(&tiros[i].rect.x,&tiros[i].rect.y,&tiros[i].arma.bala_size,"bala"))
        {
            tiros[i].rect.x += tiros[i].dx * tiros[i].arma.bala_velocidade;
            tiros[i].rect.y += tiros[i].dy * tiros[i].arma.bala_velocidade;
        }else
            retirar_tiro(&i, false);
}

void destroir_meteoro(int *i)
{
    if (meteoros[*i].status)
    {
        meteoros[*i].status = false;
        meteoros[*i].tempo_partiu = segundos;
        meteoros[*i].velocidade = 1;
        meteoros[*i].textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho_d1.png"));
    }
}

void verificar_atingidos(){
    for (int j = 0; j < numero_tiros; j++)
    {
        for (int i = 0; i < numero_inimigos; i++)
        {
            if (colidiram(&tiros[j].rect, &inimigos[i].Rect) && !tiros[j].inimigo){
                retirar_tiro(&j, true);
                if (inimigos[i].vida - tiros[j].arma.danos > 0)
                    inimigos[i].vida -= tiros[j].arma.danos;
                else
                    if (i != --numero_inimigos){
                        criar_explosao(render, 3, inimigos[i].Rect);
                        inimigos[i] = inimigos[numero_inimigos];
                    }
                break;
            }
        }

        if (colidiram(&tiros[j].rect, &player.rect) && tiros[j].inimigo){
            retirar_tiro(&j, true);
            if (player.vida - tiros[j].arma.danos > 0)
                player.vida -= tiros[j].arma.danos;
            else{
                player_status = false;
                player.vida = 0;
            }
        }

        for (size_t i = 0; i < numero_obstaculos; i++)
        {
            if (colidiram(&tiros[j].rect, &obstaculos[j].rect)){
                retirar_tiro(&j, true);
                if (obstaculos[i].vida - tiros[j].arma.danos > 0)
                    obstaculos[i].vida -= tiros[j].arma.danos;
                else
                    if (i != --numero_obstaculos){
                        criar_explosao(render, 2, obstaculos[i].rect);
                        obstaculos[i] = obstaculos[numero_obstaculos];
                    }
                break;
            }
        }
        for (int i = 0; i < numero_meteoros; i++)
        {
            if (colidiram(&tiros[j].rect, &meteoros[i].rect)){
                retirar_tiro(&j, true);
                if (meteoros[i].vida - tiros[j].arma.danos > 0)
                    meteoros[i].vida -= tiros[j].arma.danos;
                else
                    destroir_meteoro(&i);
                break;
            }
        }
        
    }

    for (int j = 0; j < numero_meteoros; j++)
    {
        if (meteoros[j].status)
        {
            for (int i = 0; i < numero_inimigos; i++)
            {
                if (colidiram(&inimigos[i].Rect, &meteoros[j].rect) && meteoros[j].status){
                    destroir_meteoro(&j);
                    if (inimigos[i].vida - meteoros[j].danos > 0)
                        inimigos[i].vida -= meteoros[j].danos;
                    else
                        if (i != --numero_inimigos){
                            criar_explosao(render, 3, inimigos[i].Rect);
                            inimigos[i] = inimigos[numero_inimigos];
                        }
                    break;
                }
            }

            if (colidiram(&meteoros[j].rect, &player.rect) && meteoros[j].status){
                destroir_meteoro(&j);
                if (player.vida - meteoros[j].danos > 0)
                    player.vida -= meteoros[j].danos;
                else{
                    player_status = false;
                    player.vida = 0;
                }
            }

            for (size_t i = 0; i < numero_obstaculos; i++)
            {
                if (colidiram(&obstaculos[i].rect, &meteoros[j].rect) && meteoros[i].status){
                    destroir_meteoro(&j);
                    if (obstaculos[i].vida - meteoros[j].danos > 0)
                        obstaculos[i].vida -= meteoros[j].danos;
                    else
                        if (i != --numero_obstaculos)
                            obstaculos[i] = obstaculos[numero_obstaculos];
                    break;
                }
            }
        }    
    }
}

void actualizar_meteoros()
{
    for (int i = 0; i < numero_meteoros; i++)
    {
        meteoros[i].rect.x += meteoros[i].dx * meteoros[i].velocidade;
        meteoros[i].rect.y += meteoros[i].dy * meteoros[i].velocidade;
        if (item_colidiu(&meteoros[i].rect.x,&meteoros[i].rect.y,&meteoros[i].rect.w,"meteoro"))
        {
            meteoros[i].rect.x -= meteoros[i].dx * meteoros[i].velocidade;
            meteoros[i].rect.y -= meteoros[i].dy * meteoros[i].velocidade;
            if(meteoros[i].status)
                destroir_meteoro(&i);
        }
        if(!meteoros[i].status){
            if (meteoros[i].tempo_partiu + TEMPO_PARA_APAGAR_METEORO <= segundos)
            {
                if(i != numero_meteoros--){
                    if (calcular_probabilidade(0))
                        criar_pacote(&meteoros[i]);
                    
                    meteoros[i] = meteoros[numero_meteoros];
                }
            }
        }
    }
}

void remover_pacote(int *i)
{
    numero_pacotes--;
    if (*i != numero_pacotes)
        pacotes[*i] = pacotes[numero_pacotes];
    
}

void obter_pacote(int *i)
{
    //codigo para obter pacote
    remover_pacote(i);
}

void actualizar_pacotes(){
    for (int i = 0; i < numero_pacotes; i++)
    {
        if (colidiram(&pacotes[i].rect, &player.rect))
        {
            obter_pacote(&i);
            break;        
        }
    }
}

void actualizar_explosoes(){
    for (size_t i = 0; i < numero_explosoes; i++)
        if (explosoes[i].primeiro_quadro <= 0)
            explosoes[i].primeiro_quadro = quadros;
        else
            if ((explosoes[i].tempo * 60 - (quadros - explosoes[i].primeiro_quadro)) <= 0)
                explosoes[i] = explosoes[--numero_explosoes]; 
}

void actualizar_jogo(){
    actualiazar_inimigos();
    verificar_atingidos();
    actualizar_tiros();
    actualizar_meteoros();
    actualizar_pacotes();
    actualizar_explosoes();
}