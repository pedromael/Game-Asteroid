#include "../header/verificador.h"

bool criar_meteoro(SDL_Renderer *render)
{
    if(numero_meteoros >= capacidade_meteoros)
        meteoros = realloc(meteoros, ++capacidade_meteoros * sizeof(meteoro));
    
    int size = (rand() % 180) + 20;
    char pos[8];
    int bufferX,bufferY;

    if (calcular_probabilidade(50))
    {
        if (calcular_probabilidade(50)){
            meteoros[numero_meteoros].rect.x = -size;
            bufferY = rand() % ALTURA;
            meteoros[numero_meteoros].rect.y = bufferY;
            sprintf(pos,"left");
        }else{
            meteoros[numero_meteoros].rect.y = -size;
            bufferX = rand() % LARGURA;
            meteoros[numero_meteoros].rect.x = bufferX;
            sprintf(pos,"up");
        }
    }else{
        if (calcular_probabilidade(50)){
            meteoros[numero_meteoros].rect.x = LARGURA;
            bufferY = rand() % ALTURA;
            meteoros[numero_meteoros].rect.y = bufferY;
            sprintf(pos,"rigth");
        }else{
            meteoros[numero_meteoros].rect.y = ALTURA;
            bufferX = rand() % LARGURA;
            meteoros[numero_meteoros].rect.x = bufferX;
            sprintf(pos,"down");
        }
    }
    
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