#include "servidor.h"

void iniciar_servidor()
{
   char *puerto_escucha = get_puerto_escucha();

   int32_t *fd_escucha = malloc(sizeof(int32_t));
   *fd_escucha = crear_servidor(puerto_escucha);

   pthread_t hilo_escucha;
   pthread_create(&hilo_escucha, NULL, &escuchar_conexiones, fd_escucha);
   pthread_detach(hilo_escucha);
}

void *atender_interfaz_es(void *fd_ptr)
{
   int32_t fd_conexion = *((int32_t *)fd_ptr);
   free(fd_ptr); // Hace falta?

   // atender handsake (para saber quienes el cliente)
   uint32_t modulo_cliente = recibir_cliente(fd_conexion);

   if (modulo_cliente != E_S)
   {
      printf("Error de Cliente \n");
      return NULL;
   }

   printf("Interfaz E/S conectada \n");
   recibir_mensaje(fd_conexion);

   return NULL;
}

void *escuchar_conexiones(void *fd_escucha)
{
   while (1)
   {
      // Revisar esto, esperar cliente crea un hilo, pero no se si esta bien eso
      esperar_cliente(*((int32_t *)fd_escucha), &atender_interfaz_es);
   }

   return NULL;
}
