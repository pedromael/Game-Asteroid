#include "../header/inicializador.h"

void inicializar_player(){
    SDL_Surface* surface = IMG_Load("files/img/navePlayer.png");
    player.textura = SDL_CreateTextureFromSurface(render,surface);
    player.rect.x = LARGURA/2;
    player.rect.y = ALTURA/2;
    player.rect.w = TAMANHO_NAVE;
    player.rect.h = TAMANHO_NAVE;
    player.dir.dx = 0;
    player.dir.dy = -1;
    player.numero_armas = 1;
    player.arma_select = 0;
    player.arma = malloc(player.numero_armas * sizeof(armas));
    player.arma[0] = arsenal[4];
    player.velocidade = VELOCIDADE_INICIAL;
    player.bolas_raio = NUMERO_DE_BALAS_RAIOS;
    player.modo_construtivo = false;
    player.robos_metralhadora = NUMERO_ROBO_METRALHADORA_INICIAL;
    player.vida = VIDA_INICIAL_PLAYER; 
    player.scudo.vida = 2000;
    player.scudo.textura = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/scudo1.png"));
}

bool inicializar_texturas() {
    textura_explosao_bala = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao/explosion1.png"));
    if (!textura_explosao_bala) return false;

    textura_explosao_parede = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao/explosion1.png"));
    if (!textura_explosao_parede) return false;

    textura_explosao_inimigo = SDL_CreateTextureFromSurface(render, IMG_Load("files/img/explosao/explosion4.png"));
    if (!textura_explosao_inimigo) return false;

    textura_meteoro = SDL_CreateTextureFromSurface(render,IMG_Load("files/img/meteoro/castanho1.png"));
    if (!textura_meteoro) return false;

    for (size_t i = 0; i < TIPOS_DE_INIMIGOS; i++){
        char buffer[25];
        sprintf(buffer, "files/img/nave%d.png", (int)i);
        textura_inimigos[i] = SDL_CreateTextureFromSurface(render, IMG_Load(buffer));
        if (!textura_inimigos[i]) return false;
    }

    textura_pacotes[0] = SDL_CreateTextureFromSurface(render, IMG_Load("files/Power-ups/pill_red.png"));
    textura_pacotes[1] = SDL_CreateTextureFromSurface(render, IMG_Load("files/Power-ups/pill_gold.png"));
    textura_pacotes[2] = SDL_CreateTextureFromSurface(render, IMG_Load("files/Power-ups/pill_green.png"));
    textura_pacotes[3] = SDL_CreateTextureFromSurface(render, IMG_Load("files/Power-ups/pill_blue.png"));

    return true;
}

void inicializar_sons() {
    som_explosao_inimigo = Mix_LoadWAV("files/DeathFlash.flac");
}

void inicializar_variaveis_globais(){
    textura_inimigos = malloc(TIPOS_DE_INIMIGOS * sizeof(SDL_Texture*));
    textura_pacotes = malloc(4 * sizeof(SDL_Texture*));
    inimigos = malloc(capacidade_inimigos * sizeof(nave_inimiga));
    paredes_defensiva = malloc(capacidade_paredes_defensivas * sizeof(parede_defensiva));
    obstaculos = malloc(numero_obstaculos * sizeof(obstaculo));
    arsenal = malloc(NUMERO_DE_ARMAS * sizeof(armas));
    balas = malloc(capacidade_balas * sizeof(bala));
    meteoros = malloc(capacidade_meteoros * sizeof(meteoro));
    pacotes = malloc(MAXIMO_PACOTES * sizeof(pacote));
    explosoes = malloc(capacidades_explosoes * sizeof(meteoro));
    robos_metralhadora = malloc(capacidade_robos_metralhadora * sizeof(robo_metralhadora));
    balas_raio = malloc(capacidade_bala_raio * sizeof(bala_raio));
}

void inicializar_arsenal(){
    int distanciaBase = 500;
    //danos - bps - bala_velocidade - pente_max - tempo_carga - bala_size - alcance

    int conf_armas[NUMERO_DE_ARMAS][7] = {
        { 3,  8, 13, 30,  3,  3, 180 + distanciaBase},
        {7,  8, 11, 20,  1,  3, 10 + distanciaBase},
        {12,   5, 13, 15,  3,  4, 10 + distanciaBase},
        {70,   3,  9, 8,  5,  7, distanciaBase},
        {25,  18, 10, 30,  1,  5, 100 + distanciaBase}
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
        arsenal[j].danos =              conf_armas[j][0];
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

bool inicializar(){
    inicializar_variaveis_globais();
    inicializar_arsenal();
    inicializar_obstaculos();
    inicializar_player();
    if(!inicializar_texturas())
        return false;
    inicializar_sons();

    return true;
}

void reiniciar(){
    player_status = true;
    
    numero_inimigos =           0;
    numero_balas =              0;
    numero_explosoes =          0;
    numero_pacotes =            0;
    numero_paredes_defensivas = 0;
    numero_robos_metralhadora = 0;

    inicializar_obstaculos();
    inicializar_player();

    for (size_t i = 0; i <= INIMIGOS_INICIAL; i++) criar_inimigo(1);
}