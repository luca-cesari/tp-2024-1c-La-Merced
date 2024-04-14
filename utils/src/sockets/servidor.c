#include "servidor.h"

int iniciar_servidor(char *puerto)
{
   int socket_servidor;

   struct addrinfo hints, *servinfo;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   getaddrinfo(NULL, puerto, &hints, &servinfo);

   socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
   bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
   listen(socket_servidor, SOMAXCONN);

   freeaddrinfo(servinfo);

   return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{
   return accept(socket_servidor, NULL, NULL);
}

void recibir_mensaje(int fd_conexion)
{
   ssize_t bytes;

   int32_t handshake;
   int32_t resultOk = 0;
   int32_t resultError = -1;

   bytes = recv(fd_conexion, &handshake, sizeof(int32_t), MSG_WAITALL);
   if (handshake == 1)
   {
      bytes = send(fd_conexion, &resultOk, sizeof(int32_t), 0);
   }
   else
   {
      bytes = send(fd_conexion, &resultError, sizeof(int32_t), 0);
   }
}