#ifndef UTILS_SOCKETS_H
#define UTILS_SOCKETS_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

enum modulo
{
    KERNEL,
    CPU,
    MEMORIA,
    E_S
};

int32_t crear_servidor(char *puerto);
void esperar_cliente(int32_t fd_escucha, void *(*atender_cliente)(void *));
int32_t recibir_cliente(int32_t fd_conexion);
int32_t crear_conexion(char *ip, char *puerto);
int32_t handshake(int32_t fd_conexion, int32_t id_modulo);
void liberar_conexion(int32_t socket_cliente);

#endif // UTILS_SOCKETS_H