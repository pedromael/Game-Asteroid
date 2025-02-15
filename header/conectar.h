#ifndef CONECTAR_H
#define CONECTAR_H

#include "index.h"
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

bool conectar(int *sock, struct sockaddr_in *serv_addr);
bool mandar_dados(int sock, nave_player *player);

#endif