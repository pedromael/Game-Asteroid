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

    bool colidiram(SDL_Rect *rect1, SDL_Rect *rect2){
        bool coliX = (rect1->x < rect2->x + rect2->w) && (rect1->x + rect1->w > rect2->x);
        bool coliY = (rect1->y < rect2->y + rect2->h) && (rect1->y + rect1->h > rect2->y);
        if (coliX && coliY)
            return true;
        return false;
    }

    bool item_colidiu(SDL_Rect rect, char *item){
        if (strcmp(item, "meteoro") != 0){
            if(colidiu_nas_bordas(rect))
                return 1;
        }else
            if(meteoro_colidio_nas_borda(rect))
                return 1;

        for (int j = numero_obstaculos - 1; j >= 0; j--) // verificar se a colisao com obstaculos

            if (colidiram(&obstaculos[j].rect ,&rect))
                return 1;

        if (strcmp(item, "bala") != 0)
            for (int j = numero_inimigos - 1; j >= 0; j--) // verificar se a colisao com nave inimiga
                if (colidiram(&inimigos[j].Rect ,&rect))
                    return 2;

        if (strcmp(item, "meteoro") != 0)
            for (int j = numero_meteoros - 1; j >= 0; j--)
                if(meteoros[j].status){
                    if (colidiram(&meteoros[j].rect ,&rect))
                        return 1;
                }
        
        for (int j = numero_paredes_defensivas - 1; j >= 0; j--)
            if (colidiram(&paredes_defensiva[j].rect, &rect))
                return true;

        return 0;
    }

    void verificar_atingidos(){
        for (int j = numero_balas - 1; j >= 0; j--)
        {   
            bool balaRetirada = false;
            for (int i = numero_inimigos - 1; i >= 0; i--)
                if (colidiram(&balas[j].rect, &inimigos[i].Rect) && !balas[j].inimigo){
                    balaRetirada = true;
                    retirar_bala(&j, true);
                    if (inimigos[i].vida - balas[j].arma->danos > 0)
                        inimigos[i].vida -= balas[j].arma->danos;
                    else
                        remover_inimigo(i);
                    break;
                }

            if (colidiram(&balas[j].rect, &player.rect) && balas[j].inimigo){
                balaRetirada = true;
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
                    balaRetirada = true;
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
                    balaRetirada = true;
                    retirar_bala(&j, true);
                    if (meteoros[i].vida - balas[j].arma->danos > 0)
                        meteoros[i].vida -= balas[j].arma->danos;
                    else
                        destroir_meteoro(&i);
                    break;
                }
            
            for (int i = numero_paredes_defensivas -1; i >= 0; i--)
            {
                if(colidiram(&balas[j].rect, &paredes_defensiva[i].rect)){
                    balaRetirada = true;
                    retirar_bala(&j, true);
                    if (paredes_defensiva[i].vida - balas[j].arma->danos > 0)
                        paredes_defensiva[i].vida -= balas[j].arma->danos;
                    else
                        destroir_parede_defensiva(i);
                    break;
                }
            } 
            if(item_colidiu(balas[j].rect, "bala") && !balaRetirada)
                retirar_bala(&j, true);
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

                for (int i = numero_meteoros - 1; i >= 0; i--)
                    if (colidiram(&meteoros[i].rect, &meteoros[j].rect) && meteoros[i].status){
                        if (meteoros[i].vida - meteoros[j].danos <= 0)
                            destroir_meteoro(&i);
                        else if (meteoros[j].vida - meteoros[i].danos <= 0)
                            destroir_meteoro(&j);
                        break;
                    }

                for (int i = numero_paredes_defensivas -1; i >= 0; i--)
                {
                    if(colidiram(&meteoros[j].rect, &paredes_defensiva[i].rect)){
                        meteoros[j].vida -= (paredes_defensiva[i].vida);
                        if (meteoros[j].vida <= 0)
                        {
                            destroir_meteoro(&j);
                        }else{
                            destroir_parede_defensiva(i);
                        }
                        break;
                    }
                } 
            }    
        }
    }

    bool na_mira(direcao dir, SDL_Rect rectIndex, SDL_Rect rectRec, int errorMarge){
        int deltaX = rectRec.x - rectIndex.x;
        int deltaY = rectRec.y - rectIndex.y;
        int produtoEscalar = deltaX * dir.dx + deltaY * dir.dy;
        int magnitudeDir = dir.dx * dir.dx + dir.dy * dir.dy;
        int magnitudeDelta = deltaX * deltaX + deltaY * deltaY;

        if (produtoEscalar > 0 && abs(produtoEscalar * produtoEscalar - magnitudeDir * magnitudeDelta) <= errorMarge * errorMarge) {
            return true;
        }
        return false;
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
            if (calcular_probabilidade(100))
            {
                criar_meteoro(render);
                tentar_criar_meteoro = 0;
            }

        if(tentar_criar_inimigo > 0){
            if (numero_inimigos < 20)
            {
                if(numero_inimigos < 10){
                    if(calcular_probabilidade(30)){
                        criar_inimigo(1);
                    }
                }else
                    if(calcular_probabilidade(20)){
                        criar_inimigo(1);
                    }
                tentar_criar_inimigo = 0;
            }else
                if(calcular_probabilidade(10)){
                        
                    criar_inimigo(1);
                    tentar_criar_inimigo = 0;
                }
        }else if(numero_inimigos == 0)
            criar_inimigo(1);

        actualizar_paredes_defensivas();
        actualizar_balas();
        actualizar_bolas_raio();
        verificar_atingidos();
        actualizar_inimigos();
        actualizar_robos_metralhadora();
        actualizar_meteoros();
        actualizar_explosoes();
        actualizar_pacotes();
    }