#include "cliente.h"

int crear_conexion(char *ip, char *puerto)
{
   struct addrinfo hints;
   struct addrinfo *server_info;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   getaddrinfo(ip, puerto, &hints, &server_info);

   int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

   connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);

   freeaddrinfo(server_info);

   return socket_cliente;
}

void handshake(int fd_conexion)
{

   int32_t handshake = 1;
   int32_t result;

   send(fd_conexion, &handshake, sizeof(int32_t), 0);
   recv(fd_conexion, &result, sizeof(int32_t), MSG_WAITALL);

   if (result == 0)
   {
      printf("Ta todo OK \n");
   }
   else
   {
      printf("Ta todo como el orto \n");
   }
}

void liberar_conexion(int socket_cliente)
{
   close(socket_cliente);
}
