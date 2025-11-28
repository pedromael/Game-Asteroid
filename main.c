#include "header/index.h"

bool player_status = true;
nave_player player;
int numero_inimigos = 0;
int capacidade_inimigos = MAXIMO_INIMIGOS;
nave_inimiga *inimigos;
int numero_obstaculos = 2;
obstaculo *obstaculos;
armas *arsenal;
int numero_balas;
int capacidade_balas = 25;
bala *balas;
int numero_meteoros = 0;
int capacidade_meteoros = 10;
meteoro *meteoros;
int numero_pacotes = 0;
pacote *pacotes;
int numero_explosoes = 0;
int capacidades_explosoes = 25;
explosao *explosoes;
int numero_paredes_defensivas = 0;
int capacidade_paredes_defensivas = 15;
parede_defensiva *paredes_defensiva;
int numero_robos_metralhadora = 0;
int capacidade_robos_metralhadora = 5;
robo_metralhadora *robos_metralhadora;
int numero_balas_raio = 0;
int capacidade_bala_raio = 1000;
bala_raio *balas_raio;

SDL_Texture *textura_explosao_bala;
SDL_Texture *textura_explosao_parede;
SDL_Texture *textura_explosao_inimigo;
SDL_Texture *textura_meteoro;
SDL_Texture *textura_robo_metralhadora;
SDL_Texture *textura_bola_raio;
SDL_Texture **textura_inimigos;
SDL_Texture **textura_danos_bola_raio;
SDL_Texture **textura_pacotes;

Mix_Chunk *som_explosao_inimigo;

SDL_Renderer *render;
int segundos,quadros;

#include "header/inicializador.h"
#include "header/desenhar.h"
#include "header/control.h"

int segundos_corrente = 0;
int tentar_criar_inimigo = 0;
int tentar_criar_meteoro = 0;

#include "header/verificador.h"
#include "header/novos.h"

int main(int argc, char* argv[]){
    srand(time(NULL));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Init(SDL_INIT_AUDIO);
    IMG_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    SDL_Window *window = SDL_CreateWindow("Asteroid V0", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGURA, ALTURA, SDL_WINDOW_SHOWN);
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font* font = TTF_OpenFont("files/Roboto-Light.ttf", 14); 
    
    if(!inicializar()) // inicializar jogo
        return true; 

    for (size_t i = 0; i <= INIMIGOS_INICIAL; i++) criar_inimigo(1);

    SDL_Texture *fundo = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/fundo/blue.png"));
    SDL_Rect rect_fundo = {0,0,LARGURA,ALTURA};

    while (true)
    {
        segundos = SDL_GetTicks() / 1000;

        if(!control()) return false; // detecao de entrada do jogador
        actualizar_jogo(); // actualizar jogo 
        desenhar(font,fundo,rect_fundo); // desenhar jogo

        if (!player_status)
        {
            printf("vc perdeu, aperte SPACO para reiniciar jogo\n");
            if(!control_perdeu()) return false;
            else reiniciar();
        }

        SDL_RenderPresent(render);
        SDL_Delay(15); // 16 e o mais proximo de 60fps
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}