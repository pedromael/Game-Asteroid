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
int numero_tiros;
int capacidade_tiros = 50;
tiro *tiros;
int numero_meteoros = 0;
int capacidade_meteoros = 10;
meteoro *meteoros;
int numero_pacotes = 0;
pacote *pacotes;

SDL_Renderer *render;
int segundos,quadros;

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

    printf("step 0\n");

    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    obstaculos = malloc(numero_obstaculos * sizeof(obstaculo));
    arsenal = malloc(numero_armas * sizeof(armas));
    tiros = malloc(capacidade_tiros * sizeof(tiro));
    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    meteoros = malloc(capacidade_meteoros * sizeof(meteoro));
    pacotes = malloc(MAXIMO_PACOTES * sizeof(pacote));

    printf("step 1\n");

    //danos - bps - bala_velocidade - pente_max - tempo_carga - bala_size
    int conf_armas[5][6] = {
        { 5,  10, 13, 30,  2,  3},
        {10,  10, 11, 25,  1,  3},
        {15,   6, 13, 20,  3,  4},
        {50,   2,  9, 10,  5,  7},
        {35,  25, 10, 35,  1,  5}
    };

    char img_som_armas[5][2][12] = {
        {"bala0.png", "shoot1.wav"},
        {"bala0.png", "shoot2.wav"},
        {"bala1.png", ""},
        {"bala1.png", ""},
        {"bala1.png", "shoot3.wav"}
    };

    for (size_t j = 0; j < numero_armas; j++){
        arsenal[j].danos =              conf_armas[j][0];
        arsenal[j].bps =                conf_armas[j][1];
        arsenal[j].bala_velocidade =    conf_armas[j][2];
        arsenal[j].pente_max =          conf_armas[j][3];
        arsenal[j].tempo_carregamento = conf_armas[j][4];
        arsenal[j].bala_size =          conf_armas[j][5];
        arsenal[j].no_pente =           arsenal[j].pente_max;
        arsenal[j].inicio_carregamento = false;
        arsenal[j].ultimo_tiro = false;
        char buffer[25];
        sprintf(buffer, "files/audio/%s", img_som_armas[j][1]);
        arsenal[j].som =  Mix_LoadWAV(buffer);
        sprintf(buffer, "files/img/%s", img_som_armas[j][0]);
        arsenal[j].textura = SDL_CreateTextureFromSurface(render, IMG_Load(buffer));
    }

    printf("step 2\n");

    obstaculos[0].rect.w = 20;
    obstaculos[0].rect.h = 730;
    obstaculos[0].rect.x = 640;
    obstaculos[0].rect.y = 87;
    obstaculos[0].vida = 6300;

    obstaculos[1].rect.w = 205;
    obstaculos[1].rect.h = 16;
    obstaculos[1].rect.x = 68;
    obstaculos[1].rect.y = 300;
    obstaculos[1].vida = 2890;

    SDL_Surface* surface = IMG_Load("files/img/navePlayer.png");
    player.textura = SDL_CreateTextureFromSurface(render,surface);
    player.rect.x = LARGURA/2;
    player.rect.y = ALTURA/2;
    player.rect.w = TAMANHO_NAVE;
    player.rect.h = TAMANHO_NAVE;
    player.dx = 0;
    player.dy = -1;
    player.numero_armas = 1;
    player.arma_select = 0;
    player.arma = malloc(player.numero_armas * sizeof(armas));
    player.arma[0] = arsenal[4];
    player.velocidade = VELOCIDADE_INICIAL;
    player.vida = 200;
    player.scudo.vida = 2000;
    player.scudo.textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/scudo1.png"));

    for (size_t i = 0; i <= numero_inimigos_inicial; i++){
        if(!criar_inimigo(render,&inimigos[i],1)){
            i--;
        }
    }

    SDL_Texture *fundo = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/fundo/blue.png"));
    SDL_Rect rect_fundo = {0,0,LARGURA,ALTURA};

    int quadros_corrente;
    int tentar_criar_inimigo = 0;
    int tentar_criar_meteoro = 0;
    bool run = true;
    printf("step 3: inicio do loop\n");
    while (run)
    {
        if (quadros_corrente >= 60){
            quadros_corrente = 0;
            segundos++;
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

        for (size_t i = 0; i < numero_tiros; i++)
            desenhar_tiro(render, &tiros[i]);

        for (size_t i = 0; i < numero_meteoros; i++)
            desenhar_meteoro(render,&meteoros[i]);
        
        if (player.scudo.ativo)
            desenhar_scudo();

        desenhar_status(render,font);

        //printf("step 6: desenhos feitos\n");

        SDL_RenderPresent(render);
        SDL_Delay(16);

        quadros++;
        quadros_corrente++;

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