#include "cliente.h"

int32_t crear_conexion(char *ip, char *puerto)
{
   struct addrinfo hints;
   struct addrinfo *server_info;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   getaddrinfo(ip, puerto, &hints, &server_info);

   int32_t socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

   connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

   freeaddrinfo(server_info);

   return socket_cliente;
}

// No estan manejados los posibles casos de error
int32_t handshake(int32_t fd_conexion, int32_t id_modulo)
{
   int32_t resultado;

   send(fd_conexion, &id_modulo, sizeof(int32_t), 0);
   recv(fd_conexion, &resultado, sizeof(int32_t), MSG_WAITALL);

   return resultado;
}

void liberar_conexion(int32_t socket_cliente)
{
   close(socket_cliente);
}
