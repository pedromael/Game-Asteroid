#include "../header/inicializador.h"

void Inicializar_player(){
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
}

bool inicializar_texturas(SDL_Renderer *render) {
    textura_explosao_bala = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao0.png"));
    if (!textura_explosao_bala) return false;

    textura_explosao_parede = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao0.png"));
    if (!textura_explosao_parede) return false;

    textura_explosao_inimigo = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao0.png"));
    if (!textura_explosao_inimigo) return false;

    textura_meteoro = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho1.png"));
    if (!textura_meteoro) return false;

    for (size_t i = 0; i < TIPOS_DE_INIMIGOS; i++){
        char buffer[25];
        sprintf(buffer, "files/img/nave%d.png", (int)i);
        textura_inimigos[i] = SDL_CreateTextureFromSurface(render, IMG_Load(buffer));
        if (!textura_inimigos[i]) return false;
    }

    return true;
}

void inicializar_variaveis_globais(){
    textura_inimigos = malloc(TIPOS_DE_INIMIGOS * sizeof(SDL_Texture*));

    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    obstaculos = malloc(numero_obstaculos * sizeof(obstaculo));
    arsenal = malloc(NUMERO_DE_ARMAS * sizeof(armas));
    balas = malloc(capacidade_balas * sizeof(bala));
    meteoros = malloc(capacidade_meteoros * sizeof(meteoro));
    pacotes = malloc(MAXIMO_PACOTES * sizeof(pacote));
    explosoes = malloc(capacidades_explosoes * sizeof(meteoro));
}

void inicializar_arsenal(){
    //danos - bps - bala_velocidade - pente_max - tempo_carga - bala_size - alcance

    int conf_armas[NUMERO_DE_ARMAS][7] = {
        { 3,  8, 13, 30,  3,  3, 580},
        {7,  8, 11, 20,  1,  3, 470},
        {12,   5, 13, 15,  3,  4, 470},
        {40,   2,  9, 8,  5,  7, 450},
        {25,  18, 10, 30,  1,  5, 500}
    };

    char img_som_armas[NUMERO_DE_ARMAS][2][12] = {
        {"bala0.png", "shoot1.wav"},
        {"bala0.png", "shoot2.wav"},
        {"bala1.png", ""},
        {"bala1.png", ""},
        {"bala1.png", "shoot3.wav"}
    };

    for (size_t j = 0; j < NUMERO_DE_ARMAS; j++){
        arsenal[j].indice =             j;
        arsenal[j].danos =              0;//conf_armas[j][0];
        arsenal[j].bps =                conf_armas[j][1];
        arsenal[j].bala_velocidade =    conf_armas[j][2];
        arsenal[j].pente_max =          conf_armas[j][3];
        arsenal[j].tempo_carregamento = conf_armas[j][4];
        arsenal[j].bala_size =          conf_armas[j][5];
        arsenal[j].alcance =            conf_armas[j][6];
        arsenal[j].no_pente =           arsenal[j].pente_max;
        arsenal[j].inicio_carregamento = false;
        arsenal[j].ultimo_bala = false;
        char buffer[25];
        sprintf(buffer, "files/audio/%s", img_som_armas[j][1]);
        arsenal[j].som =  Mix_LoadWAV(buffer);
        sprintf(buffer, "files/img/%s", img_som_armas[j][0]);
        arsenal[j].textura = SDL_CreateTextureFromSurface(render, IMG_Load(buffer));
    }
}

void inicializar_obstaculos(){
    obstaculos[0].rect.w = 20;
    obstaculos[0].rect.h = 330;
    obstaculos[0].rect.x = 640;
    obstaculos[0].rect.y = 87;
    obstaculos[0].vida = 7500;

    obstaculos[1].rect.w = 205;
    obstaculos[1].rect.h = 16;
    obstaculos[1].rect.x = 68;
    obstaculos[1].rect.y = 300;
    obstaculos[1].vida = 5890;
}

void inicializar(){
    inicializar_variaveis_globais();
    inicializar_arsenal();
    inicializar_obstaculos();
    Inicializar_player();
    inicializar_texturas(render);
}