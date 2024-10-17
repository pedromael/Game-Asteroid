#define LARGURA 800
#define ALTURA 600

typedef struct
{
    int danos;
    int bps; // balas por segundo
    int x,y;
    int dx,dy;
} arma;

typedef struct
{
    int x,y;
    int dx,dy;
    arma arma;
    int upgrade;
    int vida;
    int velocidade;
} nave_player;

typedef struct
{
    int x,y;
    int sizeX,sizeY;
    int vida;
} abstaculos;

typedef struct
{
    int x,y;
    int dx,dy;
    int vida;
    int tipo;
    int velocidade;
} nave_inimiga;

nave_player player;
nave_inimiga *inimigos;