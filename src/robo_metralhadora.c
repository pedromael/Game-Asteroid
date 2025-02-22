#include "../header/verificador.h"

bool criar_robo_metralhadora(){
    if(player.robos_metralhadora <= 0)
        return false;

    if (numero_robos_metralhadora >= capacidade_robos_metralhadora){
        capacidade_robos_metralhadora += 5;
        robos_metralhadora = realloc(robos_metralhadora, capacidade_robos_metralhadora * sizeof(robo_metralhadora));
    }
    int i = numero_robos_metralhadora;
    robos_metralhadora[i].vida = 150;
    robos_metralhadora[i].rect.x = player.rect.x + (player.dir.dx * 50);
    robos_metralhadora[i].rect.y = player.rect.y + (player.dir.dy * 50);
    robos_metralhadora[i].rect.w = TAMANHO_NAVE;
    robos_metralhadora[i].rect.h = TAMANHO_NAVE;
    robos_metralhadora[i].angulo = 0;
    robos_metralhadora[i].velocidade_giro = 1;
    robos_metralhadora[i].arma = &arsenal[4];

    numero_robos_metralhadora++;
    player.robos_metralhadora--;
    return true;
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