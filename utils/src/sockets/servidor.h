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

int32_t iniciar_servidor(char *puerto);
void esperar_cliente(int32_t fd_escucha, void *(*atender_cliente)(void *));
int32_t recibir_cliente(int32_t fd_conexion);
void recibir_mensaje(int32_t fd_conexion);
