    #include "../header/verificador.h"

    bool calcular_probabilidade(int perc){
        if ((rand() % 101) <= perc)
            return 1;
        return 0;
    }

    bool colidiu_nas_bordas(SDL_Rect rect){ 
        if (rect.x < 0 || rect.y < 0)
            return true;
        
        if (rect.x + rect.w > LARGURA || rect.y + rect.h > ALTURA)
            return true;
        return false;
    }

    bool meteoro_colidio_nas_borda(SDL_Rect rect){
        if (rect.x < -(rect.w + 2) || rect.y < -(rect.h + 2))
            return true;
        
        if (rect.x > LARGURA + 2 || rect.y > ALTURA + 2)
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

    int item_colidiu(SDL_Rect rect, char *item){
        if (strcmp(item, "meteoro") != 0){
            if(colidiu_nas_bordas(rect))
                return 1;
        }else
            if(meteoro_colidio_nas_borda(rect))
                return 1;

        for (int j = numero_obstaculos - 1; j >= 0; j--) // verificar se a colisao com obstaculos
        {
            if (colidiram(&obstaculos[j].rect ,&rect))
                return 1;
        }
        if (strcmp(item, "bala") != 0)
            for (int j = numero_inimigos - 1; j >= 0; j--) // verificar se a colisao com nave inimiga
            {
                if (colidiram(&inimigos[j].Rect ,&rect))
                    return 2;
            }
        if (strcmp(item, "meteoro") != 0)
            for (int j = numero_meteoros - 1; j >= 0; j--)
            {
                if(meteoros[j].status){
                    if (colidiram(&meteoros[j].rect ,&rect))
                        return 1;
                }
            }
        
        // if (strcmp(item, "player") != 0)
        // {   
        //     if (colidiram(&player.rect ,&rect))
        //         return 1;
        // }
        
        return 0;
    }

    bool inimigo_colidiu(int i){
        int res = colidiu_nas_bordas(inimigos[i].Rect);
        if (res)
            return true;
        
        for (int j = numero_inimigos - 1; j >= 0; j--) // verificar se ha colisao entre naves inimigas
            if (i != j){
                if (colidiram(&inimigos[i].Rect,&inimigos[j].Rect))
                    return true;
            }

        for (int j = numero_obstaculos - 1; j >= 0; j--) // verificar se a colisao com obstaculos
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
        if (inimigos != NULL)
            balas[numero_balas].inimigo = true;
        else
            balas[numero_balas].inimigo = false;
        
        numero_balas++;
        arma->no_pente--;
        arma->ultimo_bala = SDL_GetTicks(); 

        Mix_PlayChannel(-1, balas[numero_balas-1].arma->som, 0);
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

    int area_de_impacto_mira(int *i){
        int margen_de_erro = 20;
        margen_de_erro = (rand() % margen_de_erro) - (rand() % margen_de_erro);

        int x = inimigos[*i].Rect.x - (player.rect.x + margen_de_erro);
        int y = inimigos[*i].Rect.y - (player.rect.y + margen_de_erro);

        if (inimigos[*i].ultima_ronda + inimigos[*i].tempo_ronda*1000 <= SDL_GetTicks() || inimigos[*i].ultima_ronda == 0)
        {
            inimigos[*i].ultima_ronda = SDL_GetTicks();

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

    void remover_inimigo(int i){
        if (i != --numero_inimigos){
            criar_explosao(render, 3, inimigos[i].Rect);
            inimigos[i] = inimigos[numero_inimigos];
        }
    }

    int actualizar_inimigos(){
        for (int i = numero_inimigos - 1; i >= 0; i--)
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

    void actualizar_balas(){
        for (int i = numero_balas - 1; i >= 0; i--)
            if (!item_colidiu(balas[i].rect,"bala"))
            {
                if(balas[i].distancia_percorrida + balas[i].arma->bala_velocidade <= balas[i].arma->alcance){
                    balas[i].rect.x += balas[i].dx * balas[i].arma->bala_velocidade;
                    balas[i].rect.y += balas[i].dy * balas[i].arma->bala_velocidade;
                    balas[i].distancia_percorrida += balas[i].arma->bala_velocidade; // distancia percorrida pela bala
                }else
                    retirar_bala(&i, false);    
            }else
                retirar_bala(&i, true);
    }

    void destroir_meteoro(int *i)
    {
        if (calcular_probabilidade(10))
            criar_pacote(&meteoros[*i]);
        
        if (meteoros[*i].status)
        {
            meteoros[*i].status = false;
            meteoros[*i].tempo_partiu = segundos;
            meteoros[*i].velocidade = 1;
            meteoros[*i].textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho_d1.png"));
        }
    }

    void verificar_atingidos(){
        for (int j = numero_balas - 1; j >= 0; j--)
        {
            for (int i = numero_inimigos - 1; i >= 0; i--)
                if (colidiram(&balas[j].rect, &inimigos[i].Rect) && !balas[j].inimigo){
                    retirar_bala(&j, true);
                    if (inimigos[i].vida - balas[j].arma->danos > 0)
                        inimigos[i].vida -= balas[j].arma->danos;
                    else
                        remover_inimigo(i);
                    break;
                }

            if (colidiram(&balas[j].rect, &player.rect) && balas[j].inimigo){
                retirar_bala(&j, true);
                if (player.vida - balas[j].arma->danos > 0)
                    player.vida -= balas[j].arma->danos;
                else{
                    player_status = false;
                    player.vida = 0;
                }
            }

            for (int i = numero_obstaculos - 1; i >= 0; i--)
                if (colidiram(&balas[j].rect, &obstaculos[j].rect)){
                    retirar_bala(&j, true);
                    if (obstaculos[i].vida - balas[j].arma->danos > 0)
                        obstaculos[i].vida -= balas[j].arma->danos;
                    else
                        if (i != --numero_obstaculos){
                            criar_explosao(render, 2, obstaculos[i].rect);
                            obstaculos[i] = obstaculos[numero_obstaculos];
                        }
                    break;
                }

            for (int i = numero_meteoros - 1; i >= 0; i--)
                if (colidiram(&balas[j].rect, &meteoros[i].rect) && meteoros[i].status){
                    retirar_bala(&j, true);
                    if (meteoros[i].vida - balas[j].arma->danos > 0)
                        meteoros[i].vida -= balas[j].arma->danos;
                    else
                        destroir_meteoro(&i);
                    break;
                }
        }

        for (int j = numero_meteoros - 1; j >= 0; j--)
        {
            if (meteoros[j].status)
            {
                for (int i = numero_inimigos - 1; i >= 0; i--)
                    if (colidiram(&inimigos[i].Rect, &meteoros[j].rect) && meteoros[j].status){
                        destroir_meteoro(&j);
                        if (inimigos[i].vida - meteoros[j].danos > 0)
                            inimigos[i].vida -= meteoros[j].danos;
                        else
                            remover_inimigo(i);
                        break;
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

                for (int i = numero_obstaculos - 1; i >= 0; i--)
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

    void actualizar_meteoros()
    {
        for (int i = numero_meteoros - 1; i >= 0; i--)
        {
            meteoros[i].rect.x += meteoros[i].dx * meteoros[i].velocidade;
            meteoros[i].rect.y += meteoros[i].dy * meteoros[i].velocidade;
            if (item_colidiu(meteoros[i].rect,"meteoro"))
            {
                meteoros[i].rect.x -= meteoros[i].dx * meteoros[i].velocidade;
                meteoros[i].rect.y -= meteoros[i].dy * meteoros[i].velocidade;
                if(meteoros[i].status)
                    destroir_meteoro(&i);
            }
            if(!meteoros[i].status){
                if (meteoros[i].tempo_partiu + TEMPO_PARA_APAGAR_METEORO <= segundos)
                    if(i != --numero_meteoros)            
                        meteoros[i] = meteoros[numero_meteoros];
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
        if (pacotes[*i].tipo == 0)       // para vida 
        {
            player.vida += pacotes[*i].valor;
        }else if(pacotes[*i].tipo == 1){ // para armas
            int ja_tem = false;
            for (size_t j = 0; j < player.numero_armas; j++)
                if (player.arma[j].indice == arsenal[pacotes[*i].valor].indice)
                    ja_tem = true;

            if (!ja_tem)
            {
                if (player.numero_armas <= 1){
                    player.arma = realloc(player.arma, ++player.numero_armas * sizeof(armas));
                    player.arma_select = 1;
                }
                player.arma[player.arma_select] = arsenal[pacotes[*i].valor];
            }
            
        }else if(pacotes[*i].tipo == 2){ // para scudos
            player.scudo.vida += pacotes[*i].valor;
        }
        
        remover_pacote(i);
    }

    void actualizar_pacotes(){
        for (int i = numero_pacotes - 1; i >= 0; i--)
        {
            if (colidiram(&pacotes[i].rect, &player.rect))
            {
                obter_pacote(&i);
                break;        
            }
        }
    }

    void actualizar_explosoes(){
        for (int i = numero_explosoes - 1; i >= 0; i--)
            if ((SDL_GetTicks() - explosoes[i].milissegundo_inicio) > explosoes[i].tempo)
                if (i != --numero_explosoes)    
                    explosoes[i] = explosoes[numero_explosoes]; 
    }

    void retirar_parede_defensiva(int i){
        int j = --numero_paredes_defensivas;
        paredes_defensiva[i] = paredes_defensiva[j];
    }

    void actualizar_paredes_defensivas() {
        for (int i = numero_paredes_defensivas - 1; i >= 0; i--) {
            if (paredes_defensiva[i].vida <= 0 || 
                paredes_defensiva[i].tempo_posto + TEMPO_DE_PAREDE_DEFENSIVA <= segundos) {
                retirar_parede_defensiva(i);
            }
        }
    }


    void actualizar_jogo(){
        if (segundos_corrente + 1 <= segundos){
            segundos_corrente = segundos;
            tentar_criar_inimigo++;
            tentar_criar_meteoro++;

            if (player.scudo.vida < MAXIMO_SCUDO)
                player.scudo.vida = (player.scudo.vida + 2 < MAXIMO_SCUDO)? 2: 1;
        }

        if (tentar_criar_meteoro >= 3)
            if (calcular_probabilidade(80))
            {
                criar_meteoro(render);
                tentar_criar_meteoro = 0;
            }

        if(tentar_criar_inimigo > 0){
            if (numero_inimigos < 20)
            {
                if(numero_inimigos < 10){
                    if(calcular_probabilidade(30)){
                        criar_inimigo(render,&inimigos[numero_inimigos],1);
                    }
                }else
                    if(calcular_probabilidade(20)){
                        criar_inimigo(render,&inimigos[numero_inimigos],1);
                    }
                tentar_criar_inimigo = 0;
            }else
                if(calcular_probabilidade(10)){
                        
                    criar_inimigo(render,&inimigos[numero_inimigos],1);
                    tentar_criar_inimigo = 0;
                }
        }else if(numero_inimigos == 0)
            criar_inimigo(render,&inimigos[numero_inimigos],1);

        actualizar_paredes_defensivas();
        actualizar_balas();
        verificar_atingidos();
        actualizar_inimigos();
        actualizar_meteoros();
        actualizar_explosoes();
        actualizar_pacotes();
    }