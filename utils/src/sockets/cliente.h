#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

int crear_conexion(char *ip, char *puerto);
int handshake(int fd_conexion, int id_modulo);
void liberar_conexion(int socket_cliente);
