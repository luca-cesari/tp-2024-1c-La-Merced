#include "memoria.h"

int32_t fd_memoria;

int8_t conectar_con_memoria()
{
   mem_config mem = get_mem_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto;
   int32_t response = handshake(fd_memoria, CPU);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   return 0;
}

void hablar_con_memoria(int32_t fd_memoria)
{
   printf("Memoria conectada \n");
   enviar_mensaje(fd_memoria, 21); // mensaje de prueba

   recibir_mensaje(fd_memoria); // para bloquear nd mas
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}