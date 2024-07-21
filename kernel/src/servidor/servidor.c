#include "servidor.h"

int32_t fd_escucha;

void iniciar_servidor()
{
   char *puerto_escucha = get_puerto_escucha();

   fd_escucha = crear_servidor(puerto_escucha);

   pthread_t hilo_escucha;
   pthread_create(&hilo_escucha, NULL, &escuchar_conexiones, &fd_escucha);
   pthread_detach(hilo_escucha);
}

void *escuchar_conexiones(void *fd_escucha)
{
   while (1)
   {
      esperar_cliente(*((int32_t *)fd_escucha), &atender_interfaz_es);
   }
   return NULL;
}

void *atender_interfaz_es(void *fd_ptr)
{
   int32_t fd_conexion = *((int32_t *)fd_ptr);
   uint32_t modulo_cliente = recibir_cliente(fd_conexion);

   if (modulo_cliente != E_S)
   {
      printf("Error de Cliente \n");
      return NULL;
   }

   char *nombre_interfaz = recibir_mensaje(fd_conexion);
   conectar_entrada_salida(nombre_interfaz, fd_conexion);

   log_evento(string_from_format("Se conecta la interfaz %s", nombre_interfaz));
   return NULL;
}

void finalizar_servidor()
{
   close(fd_escucha);
}
