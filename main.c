#include "header/index.h"

bool player_status = true;
nave_player player;
int numero_inimigos = 0;
int numero_inimigos_inicial = 5;
int capacidade_inimigos = 10;
nave_inimiga *inimigos;
int numero_obstaculos = 2;
obstaculo *obstaculos;
const int numero_armas = 5;
armas *arsenal;
int numero_balas;
int capacidade_balas = 50;
bala *balas;
int numero_meteoros = 0;
int capacidade_meteoros = 10;
meteoro *meteoros;
int numero_pacotes = 0;
pacote *pacotes;
int numero_explosoes = 0;
int capacidades_explosoes = 100;
explosao *explosoes;

SDL_Texture *textura_explosao_bala = NULL;
SDL_Texture *textura_explosao_parede = NULL;
SDL_Texture *textura_explosao_inimigo = NULL;

SDL_Renderer *render;
int segundos,quadros;

#include "header/inicializador.h"
#include "header/desenhar.h"
#include "header/control.h"
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

    inicializar(); // inicializar jogo

    for (size_t i = 0; i <= numero_inimigos_inicial; i++)
        if(!criar_inimigo(render,&inimigos[i],1)){
            i--;
        }

    SDL_Texture *fundo = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/fundo/blue.png"));
    SDL_Rect rect_fundo = {0,0,LARGURA,ALTURA};

    int segundos_corrente;
    int tentar_criar_inimigo = 0;
    int tentar_criar_meteoro = 0;
    bool run = true;
    while (run)
    {
        segundos = SDL_GetTicks() / 1000;

        if (segundos_corrente + 1 <= segundos){
            segundos_corrente = segundos;
            tentar_criar_inimigo++;
            tentar_criar_meteoro++;

            if (player.scudo.vida < MAXIMO_SCUDO)
                player.scudo.vida = (player.scudo.vida + 5 < MAXIMO_SCUDO)? 5: 1;
        }

        if (tentar_criar_meteoro >= 3)
            if (calcular_probabilidade(80))
            {
                criar_meteoro(render);
                tentar_criar_meteoro = 0;
            }

        if(tentar_criar_inimigo > 0){
            if (numero_inimigos < 20)
            {
                if(numero_inimigos < 10){
                    if(calcular_probabilidade(25)){
                        criar_inimigo(render,&inimigos[numero_inimigos],1);
                    }
                }else
                    if(calcular_probabilidade(10)){
                        
                        criar_inimigo(render,&inimigos[numero_inimigos],1);
                    }
                tentar_criar_inimigo = 0;
            }else
                if(calcular_probabilidade(5)){
                        
                    criar_inimigo(render,&inimigos[numero_inimigos],1);
                    tentar_criar_inimigo = 0;
                }
        }else if(numero_inimigos == 0)
            criar_inimigo(render,&inimigos[numero_inimigos],1);

        SDL_RenderClear(render);
        SDL_RenderCopy(render, fundo, NULL, &rect_fundo);

        if(!control()) return false;

        //printf("step 4: a actualizar\n");

        actualizar_jogo();

        //printf("step 5: actualizado\n");

        SDL_SetRenderDrawColor(render,0,0,0,255);
        desenhar_player(render);

        for (size_t i = 0; i < numero_inimigos; i++)
            desenhar_inimigo(render,&inimigos[i]);

        for (size_t i = 0; i < numero_obstaculos; i++)
            desenhar_obstaculo(render,&obstaculos[i]);

        for (size_t i = 0; i < numero_balas; i++)
            desenhar_bala(render, &balas[i]);

        for (size_t i = 0; i < numero_explosoes; i++)
            desenhar_explosao(render, &explosoes[i]);

        for (size_t i = 0; i < numero_meteoros; i++)
            desenhar_meteoro(render,&meteoros[i]);
        
        if (player.scudo.ativo)
            desenhar_scudo();

        desenhar_status(render,font);

        //printf("step 6: desenhos feitos\n");

        SDL_RenderPresent(render);
        SDL_Delay(16);

        if (!player_status)
        {
            printf("vc perdeu, aperte SPACO para reiniciar jogo\n");
            while (1)
            {
                SDL_Event evento;
                while (SDL_PollEvent(&evento)){
                    if (evento.type == SDL_QUIT)
                        return 0;
                    
                    if (evento.type == SDL_KEYDOWN)
                        if (evento.key.keysym.sym == SDLK_SPACE){
                            player_status = true;
                            player.vida = 200;
                            break;
                        }
                }
            }
        }
    }

    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}