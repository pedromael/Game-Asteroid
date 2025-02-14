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
        
        for (int j = numero_paredes_defensivas - 1; j >= 0; j--)
            if (colidiram(&paredes_defensiva[j].rect, &rect))
                return true;
        
        
        // if (strcmp(item, "player") != 0)
        // {
        //     if (colidiram(&player.rect ,&rect))
        //         return 1;
        // }

        return 0;
    }

    bool inimigo_colidiu(int i){
        if (colidiu_nas_bordas(inimigos[i].Rect))
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
        
        for (int j = numero_paredes_defensivas - 1; j >= 0; j--)
            if (colidiram(&paredes_defensiva[j].rect, &inimigos[i].Rect))
                return true;

        return false;
    }

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

    int area_de_impacto_mira(int *i) {
        int margen_de_erro = (rand() % 41) - 20;
    
        // Calcula a distância do inimigo até o jogador
        int x = inimigos[*i].Rect.x - (player.rect.x + margen_de_erro);
        int y = inimigos[*i].Rect.y - (player.rect.y + margen_de_erro);
    
        // Verifica se é hora de recalcular a direção
        if (inimigos[*i].ultima_ronda == 0 || 
            inimigos[*i].ultima_ronda + (inimigos[*i].tempo_ronda * 1000) <= SDL_GetTicks()) {
            
            inimigos[*i].ultima_ronda = SDL_GetTicks();
    
            // Prioriza o movimento na direção de maior distância
            if (abs(x) > abs(y)) {
                inimigos[*i].dir.dx = (x > 0) ? -1 : 1;
                inimigos[*i].dir.dy = 0;
    
                // 20% de chance de inverter a prioridade para o eixo Y
                if (calcular_probabilidade(20)) {
                    inimigos[*i].dir.dx = 0;
                    inimigos[*i].dir.dy = (y > 0) ? -1 : 1;
                    return abs(y);
                }
                return abs(x);
            } else {
                inimigos[*i].dir.dy = (y > 0) ? -1 : 1;
                inimigos[*i].dir.dx = 0;
    
                if (calcular_probabilidade(20)) {
                    inimigos[*i].dir.dy = 0;
                    inimigos[*i].dir.dx = (x > 0) ? -1 : 1;
                    return abs(x);
                }
                return abs(y);
            }
        }
    
        // Mantém a direção caso não seja hora de recalcular
        if (inimigos[*i].dir.dx != 0) {
            return abs(x);
        } else {
            return abs(y);
        }
    }
    
    void remover_inimigo(int i){
        if (i != --numero_inimigos){
            criar_explosao(render, 3, inimigos[i].Rect);
            inimigos[i] = inimigos[numero_inimigos];
        }
    }

    void actualizar_inimigos(){
        for (int i = numero_inimigos - 1; i >= 0; i--)
        {
            if (area_de_impacto_mira(&i) < 150){
                if (inimigos[i].dir.dx != 0)
                {
                    int buffer_dx = inimigos[i].dir.dx;
                    inimigos[i].dir.dx = 0;
                    inimigos[i].dir.dy = inimigos[i].Rect.y - player.rect.y > 0 ? -1 : 1;
                    if(calcular_probabilidade(10))
                        disparar(inimigos[i].Rect, inimigos[i].dir, &inimigos[i].arma, true);
                    inimigos[i].dir.dy = 0;
                    inimigos[i].dir.dx = buffer_dx;
                }else{
                    int buffer_dy = inimigos[i].dir.dy;
                    inimigos[i].dir.dy = 0;
                    inimigos[i].dir.dx = inimigos[i].Rect.x - player.rect.x > 0 ? -1 : 1;
                    if(calcular_probabilidade(10))
                        disparar(inimigos[i].Rect, inimigos[i].dir, &inimigos[i].arma, true);
                    inimigos[i].dir.dx = 0;
                    inimigos[i].dir.dy = buffer_dy; 
                }
            }

            bool mover = true;
            if (area_de_impacto_mira(&i) < TAMANHO_NAVE)
                mover = calcular_probabilidade(25); // chance de se mover se estiver na rea de impacto

            if(mover){
                if (inimigos[i].dir.dx != 0)
                {
                    inimigos[i].Rect.x += VELOCIDADE_INIMIGA * inimigos[i].dir.dx;
                    if (inimigo_colidiu(i)){
                        inimigos[i].Rect.x -= VELOCIDADE_INIMIGA * inimigos[i].dir.dx;
                    }

                }else{
                    inimigos[i].Rect.y += VELOCIDADE_INIMIGA * inimigos[i].dir.dy;
                    if (inimigo_colidiu(i))
                    {
                        inimigos[i].Rect.y -= VELOCIDADE_INIMIGA * inimigos[i].dir.dy;
                    }
                }
            }
        }
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
        }else if(pacotes[*i].tipo == 3)
            player.robos_metralhadora += pacotes[*i].valor;
        
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
            if ((SDL_GetTicks() - explosoes[i].milissegundo_inicio) > explosoes[i].tempo){
                if (i != --numero_explosoes)    
                    explosoes[i] = explosoes[numero_explosoes]; 
            }else{
                int tempoDecorrido = SDL_GetTicks() - explosoes[i].milissegundo_inicio;
                int tempoParaCadaStep = explosoes[i].tempo / explosoes[i].lastStep;

                explosoes[i].step = tempoDecorrido / tempoParaCadaStep;

                if (explosoes[i].step >= explosoes[i].lastStep) {
                    explosoes[i].step = explosoes[i].lastStep - 1; // Último frame válido
                }
            }
    }

    void destroir_parede_defensiva(int i){
        int j = --numero_paredes_defensivas;
        paredes_defensiva[i] = paredes_defensiva[j];
    }

    void actualizar_paredes_defensivas() {
        for (int i = numero_paredes_defensivas - 1; i >= 0; i--) {
            if (paredes_defensiva[i].vida <= 0 || 
                paredes_defensiva[i].tempo_posto + TEMPO_DE_PAREDE_DEFENSIVA <= segundos) {
                destroir_parede_defensiva(i);
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

    void actualizar_robos_metralhadora(){
        for (int i = numero_robos_metralhadora - 1; i >= 0; i--)
        {
            if (robos_metralhadora[i].vida <= 0)
            {
                if (i != --numero_robos_metralhadora)
                    robos_metralhadora[i] = robos_metralhadora[numero_robos_metralhadora];
            }else{
                direcao dir;
                double angulo_radianos = robos_metralhadora[i].angulo * (PI / 180.0);
                dir.dx = cos(angulo_radianos) * 5;
                dir.dy = sin(angulo_radianos) * 5;

                bool encontrou_alvo = false;

                for (int j = numero_inimigos - 1; j >= 0; j--)
                    if (na_mira(dir, robos_metralhadora[i].rect, inimigos[j].Rect, 75)) {
                        disparar(robos_metralhadora[i].rect, dir, robos_metralhadora[i].arma, false);
                        encontrou_alvo = true;
                        break;  // Sai do loop, pois já encontrou um alvo
                    }

                if(!encontrou_alvo)    
                    for (int j = numero_meteoros - 1; j >= 0; j--)
                        if (na_mira(dir, robos_metralhadora[i].rect, meteoros[j].rect, 50)) {
                            disparar(robos_metralhadora[i].rect, dir, robos_metralhadora[i].arma, false);
                            encontrou_alvo = true;
                            break;  // Sai do loop, pois já encontrou um alvo
                        }

                if (!encontrou_alvo) {
                    if (robos_metralhadora[i].angulo + robos_metralhadora[i].velocidade_giro <= 360)
                        robos_metralhadora[i].angulo += robos_metralhadora[i].velocidade_giro;
                    else
                        robos_metralhadora[i].angulo = 0;
                }
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
        actualizar_robos_metralhadora();
        actualizar_meteoros();
        actualizar_explosoes();
        actualizar_pacotes();
    }