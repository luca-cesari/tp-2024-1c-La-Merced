#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

int iniciar_servidor(char *puerto);
int esperar_cliente(int socket_servidor);
// int recibir_operacion(int socket_cliente);
// void *recibir_buffer(int *size, int socket_cliente);
void recibir_mensaje(int fd_conexion);
