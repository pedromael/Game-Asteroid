#include "../header/desenhar.h"

#include <SDL2/SDL.h>

// Função para desenhar um círculo
void draw_circle(SDL_Renderer *renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int decision = 1 - x;  // Critério de decisão inicial

    // Enquanto y não ultrapassar x (cobrindo 1/8 do círculo)
    while (y <= x) {
        // Desenha os 8 pontos simétricos
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);

        y++;

        // Atualiza o critério de decisão
        if (decision < 0) {
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }
}


void desenhar_inimigo(nave_inimiga *nave){
    float angulo = atan2(nave->dir.dy, nave->dir.dx) * 180 / PI;
    angulo += 90;
    SDL_Point centro = { nave->Rect.w / 2, nave->Rect.h / 2 };
    SDL_RenderCopyEx(render, nave->textura, NULL, &nave->Rect, angulo, &centro, SDL_FLIP_NONE);
}

void desenhar_player(){
    float angulo = atan2(player.dir.dy, player.dir.dx) * 180 / PI;
    angulo += 90;
    // Centro da rotação (podemos usar o centro da imagem)
    SDL_Point centro = { player.rect.w / 2, player.rect.h / 2 };

    // Renderizar a textura com rotação
    SDL_RenderCopyEx(render, player.textura, NULL, &player.rect, angulo, &centro, SDL_FLIP_NONE);
}

void desenhar_explosao(explosao *item) {
    int total_colunas = 8;
    float proporcao = 1.0f / total_colunas;  // Cada frame ocupa 12.5% da textura

    int linha = item->step / total_colunas;
    int coluna = item->step % total_colunas;

    int textura_largura, textura_altura;
    SDL_QueryTexture(item->textura, NULL, NULL, &textura_largura, &textura_altura);
    // Calcula a posição e tamanho do frame baseado em percentagens
    SDL_Rect src_rect = {
        (int)(coluna * proporcao * textura_largura),  // X
        (int)(linha * proporcao * textura_altura),    // Y
        (int)(proporcao * textura_largura),           // Largura
        (int)(proporcao * textura_altura)             // Altura
    };

    SDL_RenderCopy(render, item->textura, &src_rect, &item->rect);
}

void desenhar_bala(bala *balas){
    SDL_RenderCopy(render, balas->arma->textura, NULL, &balas->rect);
}

void desenhar_meteoro(meteoro *meteoro){
    SDL_RenderCopy(render, meteoro->textura, NULL, &meteoro->rect);
}

void desenhar_obstaculo(obstaculo *obstacul){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &obstacul->rect);
}

void desenhar_parede_defensiva(parede_defensiva *parede){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &parede->rect);
}

void desenhar_pacote(pacote *pacote){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderCopy(render, pacote->textura, NULL, &pacote->rect);
}

void desenhar_robo_metralhadora(robo_metralhadora *robo){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    SDL_RenderDrawRect(render, &robo->rect);
}

void desenhar_bala_raio(bala_raio *bala){
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
    draw_circle(render, bala->x, bala->y, bala->raio);
}

void desenhar_scudo(){
    SDL_RenderDrawRect(render, &player.scudo.rect);
}

void desenhar_status(SDL_Renderer *render, TTF_Font *font){
    SDL_Color color = {0, 0, 0};

    char buffer[25];
    sprintf(buffer,"No Pente: %d",player.arma[player.arma_select].no_pente);
    SDL_Surface* surface = TTF_RenderText_Solid(font, buffer, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect destRect = {LARGURA - surface->w - 17, ALTURA - surface->h - 9, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &destRect);

    sprintf(buffer,"robos: %d",player.robos_metralhadora);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect RRect = {LARGURA - surface->w - 17, ALTURA - surface->h - 22, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &RRect);

    sprintf(buffer,"HP : %d",player.vida);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect Rect = {LARGURA - surface->w - 17, 0, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &Rect);

    sprintf(buffer,"Modo Construtivo : %s",player.modo_construtivo ? "true" : "false");
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect MCRect = {LARGURA - surface->w - 17, 17, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &MCRect);

    sprintf(buffer,"time: %d",segundos);
    surface = TTF_RenderText_Solid(font, buffer, color);
    texture = SDL_CreateTextureFromSurface(render, surface);
    SDL_Rect Rect1 = {10, 5, surface->w, surface->h};
    SDL_RenderCopy(render, texture, NULL, &Rect1);
}

void desenhar(TTF_Font* font, SDL_Texture *fundo, SDL_Rect rect_fundo){
    SDL_RenderClear(render);
    SDL_RenderCopy(render, fundo, NULL, &rect_fundo);

    desenhar_player();

    for (size_t i = 0; i < numero_inimigos; i++)
        desenhar_inimigo(&inimigos[i]);

    for (size_t i = 0; i < numero_obstaculos; i++)
        desenhar_obstaculo(&obstaculos[i]);

    for (size_t i = 0; i < numero_paredes_defensivas; i++)
        desenhar_parede_defensiva(&paredes_defensiva[i]);   

    for (size_t i = 0; i < numero_balas; i++)
        desenhar_bala(&balas[i]);

    for (size_t i = 0; i < numero_explosoes; i++)
        desenhar_explosao(&explosoes[i]);

    for (size_t i = 0; i < numero_meteoros; i++)
        desenhar_meteoro(&meteoros[i]);

    for (size_t i = 0; i < numero_pacotes; i++)
        desenhar_pacote(&pacotes[i]);

    for(size_t i = 0; i < numero_robos_metralhadora; i++)
        desenhar_robo_metralhadora(&robos_metralhadora[i]);
    
    for(size_t i = 0; i < numero_balas_raio; i++)
        desenhar_bala_raio(&balas_raio[i]);  

    if (player.scudo.ativo)
        desenhar_scudo();

    desenhar_status(render,font);
}