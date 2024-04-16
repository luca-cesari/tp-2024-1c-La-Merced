#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

int32_t crear_conexion(char *ip, char *puerto);
int32_t handshake(int32_t fd_conexion, int32_t id_modulo);
void liberar_conexion(int32_t socket_cliente);
