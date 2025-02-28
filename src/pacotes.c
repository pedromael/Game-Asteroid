#include "../header/verificador.h"

bool criar_pacote(meteoro *met){
    if (numero_pacotes >= MAXIMO_PACOTES)
       return false;
    
    pacotes[numero_pacotes].rect.x = met->rect.x + (met->rect.w/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.y = met->rect.y + (met->rect.h/2) - (TAMANHO_PACOTE/2);
    pacotes[numero_pacotes].rect.w = TAMANHO_PACOTE;
    pacotes[numero_pacotes].rect.h = TAMANHO_PACOTE;

    if (colidiu_nas_bordas(pacotes[numero_pacotes].rect))
        return false;

    pacotes[numero_pacotes].tipo = rand()%4;

    if(pacotes[numero_pacotes].tipo == 0){ // para vida
        pacotes[numero_pacotes].valor = (rand()%70) + 20;
        pacotes[numero_pacotes].textura = textura_pacotes[0];
    }
    if(pacotes[numero_pacotes].tipo == 1){ // para armas
        pacotes[numero_pacotes].valor = (rand()%(NUMERO_DE_ARMAS-1)) + 1;
        pacotes[numero_pacotes].textura = textura_pacotes[1];
    }
    if(pacotes[numero_pacotes].tipo == 2){ // para scudo
        pacotes[numero_pacotes].valor = (rand()%200) + 100;
        pacotes[numero_pacotes].textura = textura_pacotes[2];
    }
    if(pacotes[numero_pacotes].tipo == 3){
        if(calcular_probabilidade(75))
            pacotes[numero_pacotes].valor = 1;
        else
            pacotes[numero_pacotes].valor = (rand()%3) + 1;
        pacotes[numero_pacotes].textura = textura_pacotes[3];
    }
    numero_pacotes++;

    //printf("pacote criado: \n");

    return true;
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