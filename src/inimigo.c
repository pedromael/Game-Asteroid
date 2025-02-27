#include "../header/verificador.h"

bool criar_inimigo(int nivel){
    //return false;
    if(numero_inimigos >= MAXIMO_INIMIGOS)
        return false;
    
    nivel = (rand() % 3) + 1;
    
    int tamanho = TAMANHO_NAVE + 10 * nivel;
    SDL_Rect rect;
    int tentativas = 0;
    while(true){
        rect.y = 0;
        rect.x = rand() % (LARGURA - tamanho); 
        rect.w = rect.h = tamanho;
        if(!item_colidiu(rect,"inimigo"))
            break;
        if (++tentativas >= 150)
            return false;
    }

    if(numero_inimigos >= capacidade_inimigos){
        if (capacidade_inimigos + 5 >= MAXIMO_INIMIGOS)
            capacidade_inimigos = MAXIMO_INIMIGOS;
        else
            capacidade_inimigos += 5;
        
        inimigos = realloc(inimigos,  capacidade_inimigos * sizeof(inimigos));
    }

    nave_inimiga *inimigo = &inimigos[numero_inimigos];
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

    if (!inimigo->textura){
        printf("falha ao carregar textura");
        return false; // nao foi possivel carregar a textura.
    }

    inimigo->Rect = rect;
    inimigo->dir.dx = 1;
    inimigo->dir.dy = 0;
    inimigo->vida = 50*nivel;
    inimigo->velocidade = VELOCIDADE_INIMIGA;
    inimigo->ultima_ronda = false;
    inimigo->tempo_ronda = TEMPO_REFLEXO_INIMIGO;
    inimigo->bala_raio = false;

    numero_inimigos++;
    return true;
}

int area_de_impacto_mira(int *i) {
    int margen_de_erro = (rand() % 31) - 15;

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

void remover_inimigo(int i){
    if (i != --numero_inimigos){
        criar_explosao(render, 3, inimigos[i].Rect);
        inimigos[i] = inimigos[numero_inimigos];
    }
}

void actualizar_inimigos(){
    for (int i = numero_inimigos - 1; i >= 0; i--)
    {
        if(inimigos[i].bala_raio){
            inimigos[i].vida =- DANOS_BALA_RAIO;
            if(inimigos[i].vida <= 0)
                remover_inimigo(i);
            continue;
        }

        if (area_de_impacto_mira(&i) < 100){
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