#include "../header/verificador.h"

void disparar_bola_raio(){
    if(player.bolas_raio <= 0)
        return;

    if(numero_balas_raio >= capacidade_bala_raio){
        capacidade_bala_raio =+ 10;
        balas_raio = realloc(balas_raio, capacidade_bala_raio * sizeof(bala_raio));
    }

    int i = numero_balas_raio;

    balas_raio[i].x = player.rect.x + (player.rect.w / 2);
    balas_raio[i].y = player.rect.y + (player.rect.h / 2);
    balas_raio[i].raio = 15;
    balas_raio[i].danos = TOTAL_DANOS_BALA_RAIO;
    
    player.bolas_raio--;
    numero_balas_raio++;
}

void remover_bala_raio(int i){
    if (i != --numero_balas_raio){
        balas_raio[i] = balas_raio[numero_balas_raio];
    }
}

// Função para limitar um valor entre um mínimo e um máximo.
float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void verificar_atingidos_por_bala_raio(int i) {
    for (int i = numero_inimigos - 1; i >= 0; i--) {
        if (inimigos[i].bala_raio)
            continue;
        
        float cx = balas_raio[i].x;
        float cy = balas_raio[i].y;
        float raio = balas_raio[i].raio;
        
        // Supondo que o retângulo do inimigo tenha os campos x, y, width e height.
        float rx = inimigos[i].Rect.x;
        float ry = inimigos[i].Rect.y;
        float rwidth = inimigos[i].Rect.w;
        float rheight = inimigos[i].Rect.h;
        
        // Calcula o ponto mais próximo do centro da bala que esteja dentro do retângulo.
        float closestX = clamp(cx, rx, rx + rwidth);
        float closestY = clamp(cy, ry, ry + rheight);
        
        // Calcula a distância entre o centro da bala e o ponto mais próximo.
        float dx = cx - closestX;
        float dy = cy - closestY;
        float distance = sqrtf(dx * dx + dy * dy);

        if (distance >= (raio - 20) && distance <= raio) {
                inimigos[i].bala_raio = true;
        }
    }
}

void actualizar_bolas_raio(){
    for(int i = numero_balas_raio - 1; i >= 0; i--){
        balas_raio[i].raio += VELOCIDADE_BALA_RAIO;

        verificar_atingidos_por_bala_raio(i);
        //printf("Bala raio %d: x = %d, y = %d, raio = %f\n", i, balas_raio[i].x, balas_raio[i].y, balas_raio[i].raio);
        if(balas_raio[i].raio > MAXIMO_RAIO_BALA_RAIO){
            remover_bala_raio(i);
        }
    }
}