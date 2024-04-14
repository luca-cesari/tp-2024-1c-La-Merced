cliente #include<stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

    // #define KERNEL 0
    // #define CPU 1
    // #define MEMORIA 2
    // #define E_S 3

    enum modules {
       KERNEL,
       CPU,
       MEMORIA,
       E_S
    };

int iniciar_servidor(char *puerto);
int esperar_cliente(int socket_servidor);
// int recibir_operacion(int socket_cliente);
// void *recibir_buffer(int *size, int socket_cliente);
void recibir_mensaje(int fd_conexion);
