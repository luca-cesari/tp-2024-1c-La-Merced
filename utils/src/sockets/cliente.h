#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

int crear_conexion(char *ip, char *puerto);
void handshake(int fd_conexion);
void liberar_conexion(int socket_cliente);
