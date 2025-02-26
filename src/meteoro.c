#include "../header/verificador.h"

bool criar_meteoro(SDL_Renderer *render)
{
    if(numero_meteoros >= capacidade_meteoros)
        meteoros = realloc(meteoros, ++capacidade_meteoros * sizeof(meteoro));
    
    int size = (rand() % 180) + 20;
    char pos[8];

    const char* direcoes[] = {"left", "up", "right", "down"};
    int direcao = rand() % 4;

    switch (direcao) {
        case 0: // Esquerda
            meteoros[numero_meteoros].rect.x = -size;
            meteoros[numero_meteoros].rect.y = rand() % ALTURA;
            break;
        case 1: // Cima
            meteoros[numero_meteoros].rect.y = -size;
            meteoros[numero_meteoros].rect.x = rand() % LARGURA;
            break;
        case 2: // Direita
            meteoros[numero_meteoros].rect.x = LARGURA;
            meteoros[numero_meteoros].rect.y = rand() % ALTURA;
            break;
        case 3: // Baixo
            meteoros[numero_meteoros].rect.y = ALTURA;
            meteoros[numero_meteoros].rect.x = rand() % LARGURA;
            break;
    }

    sprintf(pos, "%s", direcoes[direcao]);
    
    meteoros[numero_meteoros].rect.w = size;
    meteoros[numero_meteoros].rect.h = size;
    meteoros[numero_meteoros].danos = size + 50;
    meteoros[numero_meteoros].vida = (rand() % size) + 75;
    meteoros[numero_meteoros].velocidade = (rand() % 8) + 2;
    meteoros[numero_meteoros].status = true;

    meteoros[numero_meteoros].textura = textura_meteoro;

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

bool meteoro_colidio_nas_borda(SDL_Rect rect){
    int folga = 10;
    if (rect.x < -(rect.w + folga) || rect.y < -(rect.h + folga))
        return true;
    
    if (rect.x > LARGURA + folga || rect.y > ALTURA + folga)
        return true;
    return false;
}

void destroir_meteoro(int *i)
{
    if (calcular_probabilidade(15))
        criar_pacote(&meteoros[*i]);
    
    if (meteoros[*i].status)
    {
        meteoros[*i].status = false;
        meteoros[*i].tempo_partiu = segundos;
        meteoros[*i].velocidade = 1;
        meteoros[*i].textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho_d1.png"));
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