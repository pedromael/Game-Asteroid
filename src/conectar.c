#include "../header/conectar.h"

bool conectar(int *sock, struct sockaddr_in *serv_addr) {
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (*sock < 0) {
        perror("Erro ao criar socket");
        return false;
    }

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr->sin_addr);

    if (connect(*sock, (struct sockaddr*)serv_addr, sizeof(*serv_addr)) < 0) {
        perror("Erro ao conectar");
        close(*sock);
        return false;
    }

    printf("Conectado ao servidor!\n");
    return true;
}

bool mandar_dados(int sock, nave_player *player) {
    // player->id = htonl(player->id);
    // player->vida = htonl(player->vida);

    if (send(sock, player, sizeof(nave_player), 0) < 0) {
        perror("Erro ao enviar dados");
        return false;
    }

    return true;
}
