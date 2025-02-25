#include "../header/verificador.h"

bool criar_parede_defensiva() {
    if (numero_paredes_defensivas >= capacidade_paredes_defensivas) {
        capacidade_paredes_defensivas += 5;
        paredes_defensiva = realloc(paredes_defensiva, capacidade_paredes_defensivas * sizeof(parede_defensiva));
    }

    int i = numero_paredes_defensivas;
    paredes_defensiva[i].vida = VIDA_PAREDE;
    paredes_defensiva[i].tempo_posto = segundos;

    int dx = player.modo_construtivo ? player.olddx : player.dir.dx;
    int dy = player.modo_construtivo ? player.olddy : player.dir.dy;

    if (dx || dy) {
        paredes_defensiva[i].rect.x = player.rect.x + (DISTANCIA_PAREDE_PLAYER * dx);
        paredes_defensiva[i].rect.y = player.rect.y + (DISTANCIA_PAREDE_PLAYER * dy);
        paredes_defensiva[i].rect.w = (dx != 0) ? PAREDE_H : PAREDE_W;
        paredes_defensiva[i].rect.h = (dx != 0) ? PAREDE_W : PAREDE_H;
    } else
        return false;

    if (item_colidiu(paredes_defensiva[i].rect, ""))
        return false;
    numero_paredes_defensivas++;
    return true;
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