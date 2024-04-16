#include "servidor.h"

int32_t iniciar_servidor(char *puerto)
{
   int32_t socket_servidor, error;

   struct addrinfo hints, *servinfo;

   memset(&hints, 0, sizeof(hints));
   hints.ai_family = AF_INET;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE;

   // Hacer algo en caso de error (devolverlo capaz, o un exit())
   error = getaddrinfo(NULL, puerto, &hints, &servinfo);

   socket_servidor = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
   bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
   listen(socket_servidor, SOMAXCONN);

   freeaddrinfo(servinfo);

   return socket_servidor;
}

void esperar_cliente(int32_t fd_escucha, (void *)atender_cliente(int32_t))
{
   pthread_t thread;
   int32_t *fd_conexion_ptr = malloc(sizeof(int32_t));

   *fd_conexion_ptr = accept(fd_escucha, NULL, NULL);
   pthread_create(&thread, NULL, atender_cliente, fd_conexion_ptr);

   pthread_detach(thread);
}

int32_t recibir_cliente(int32_t fd_conexion)
{
   int32_t id_modulo;
   int32_t resultOk = 0;
   int32_t resultError = -1;

   recv(fd_conexion, &id_modulo, sizeof(int32_t), MSG_WAITALL);

   if (id_modulo < 0 || id_modulo > 3) // se escapa de los modulos
   {
      send(fd_conexion, &resultError, sizeof(int32_t), 0);
      return -1;
   }

   return id_modulo;
}

