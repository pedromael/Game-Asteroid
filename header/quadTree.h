#ifndef QUADTREE_H
#define QUADTREE_H

#include "index.h"

// Configurações da QuadTree
#define MAX_OBJETOS 610 // Máximo de objetos antes de dividir
#define MAX_NIVEIS 5   // Nível máximo de profundidade

// Estrutura da QuadTree
typedef struct QuadTree {
    int nivel;            // Nível de profundidade deste nó
    SDL_Rect limite;      // Área coberta pelo nó
    SDL_Rect *objetos;    // Objetos contidos neste nó
    int numObjetos;       // Número atual de objetos no nó
    struct QuadTree *no[4]; // no filhos (subdivisões)
} QuadTree;

// Cria uma nova QuadTree
static QuadTree* criarQuadTree(int nivel, SDL_Rect limite) {
    QuadTree *arvore = (QuadTree *)malloc(sizeof(QuadTree));
    if (!arvore) {
        fprintf(stderr, "Erro ao alocar memória para QuadTree\n");
        return NULL;
    }

    arvore->nivel = nivel;
    arvore->limite = limite;
    arvore->objetos = (SDL_Rect *)malloc(MAX_OBJETOS * sizeof(SDL_Rect));
    if (!arvore->objetos) {
        free(arvore);
        fprintf(stderr, "Erro ao alocar memória para objetos da QuadTree\n");
        return NULL;
    }

    arvore->numObjetos = 0;
    for (int i = 0; i < 4; i++) {
        arvore->no[i] = NULL;
    }
    return arvore;
}

// Divide o nó em 4 quadrantes
static void dividir(QuadTree *arvore) {
    int subLargura = arvore->limite.w / 2;
    int subAltura = arvore->limite.h / 2;
    int x = arvore->limite.x;
    int y = arvore->limite.y;

    arvore->no[0] = criarQuadTree(arvore->nivel + 1, (SDL_Rect){x + subLargura, y, subLargura, subAltura});
    arvore->no[1] = criarQuadTree(arvore->nivel + 1, (SDL_Rect){x, y, subLargura, subAltura});
    arvore->no[2] = criarQuadTree(arvore->nivel + 1, (SDL_Rect){x, y + subAltura, subLargura, subAltura});
    arvore->no[3] = criarQuadTree(arvore->nivel + 1, (SDL_Rect){x + subLargura, y + subAltura, subLargura, subAltura});
}

// Obtém o índice do quadrante que contém o retângulo
static int obterIndice(SDL_Rect *retangulo, QuadTree *arvore) {
    int indice = -1;
    double pontoMedioVertical = arvore->limite.x + (arvore->limite.w / 2);
    double pontoMedioHorizontal = arvore->limite.y + (arvore->limite.h / 2);

    int quadranteSuperior = (retangulo->y < pontoMedioHorizontal && retangulo->y + retangulo->h < pontoMedioHorizontal);
    int quadranteInferior = (retangulo->y > pontoMedioHorizontal);
    int quadranteEsquerdo = (retangulo->x < pontoMedioVertical && retangulo->x + retangulo->w < pontoMedioVertical);
    int quadranteDireito = (retangulo->x > pontoMedioVertical);

    if (quadranteSuperior) {
        if (quadranteDireito) {
            indice = 0;
        } else if (quadranteEsquerdo) {
            indice = 1;
        }
    } else if (quadranteInferior) {
        if (quadranteDireito) {
            indice = 2;
        } else if (quadranteEsquerdo) {
            indice = 3;
        }
    }
    return indice;
}

// Insere um objeto na QuadTree
static void inserir(QuadTree *arvore, SDL_Rect *retangulo) {
    if (arvore->no[0] != NULL) {
        int indice = obterIndice(retangulo, arvore);
        if (indice != -1) {
            inserir(arvore->no[indice], retangulo);
            return;
        }
    }

    arvore->objetos[arvore->numObjetos++] = *retangulo;

    if (arvore->numObjetos > MAX_OBJETOS && arvore->nivel < MAX_NIVEIS) {
        if (arvore->no[0] == NULL) {
            dividir(arvore);
        }

        for (int i = 0; i < arvore->numObjetos; i++) {
            int indice = obterIndice(&arvore->objetos[i], arvore);
            if (indice != -1) {
                inserir(arvore->no[indice], &arvore->objetos[i]);
            }
        }

        arvore->numObjetos = 0;
    }
}

// Recupera objetos de um nó da QuadTree
static void recuperar(QuadTree *arvore, SDL_Rect *retangulo, SDL_Rect **resultado, int *contagem) {
    int indice = obterIndice(retangulo, arvore);
    if (indice != -1 && arvore->no[0] != NULL) {
        recuperar(arvore->no[indice], retangulo, resultado, contagem);
    }

    for (int i = 0; i < arvore->numObjetos; i++) {
        resultado[*contagem] = &arvore->objetos[i];
        (*contagem)++;
    }
}

// Limpa a memória usada pela QuadTree
static void limpar(QuadTree *arvore) {
    if (!arvore) return;

    for (int i = 0; i < 4; i++) {
        if (arvore->no[i]) {
            limpar(arvore->no[i]);
            free(arvore->no[i]);
        }
    }

    free(arvore->objetos);
    free(arvore);
}

#endif // QUADTREE_H
