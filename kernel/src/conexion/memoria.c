#include "memoria.h"

int32_t fd_memoria;
pthread_mutex_t mutex_memoria;

int8_t conectar_con_memoria()
{
   mem_config mem = get_memoria_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto);
   int32_t response = handshake(fd_memoria, KERNEL);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   pthread_mutex_init(&mutex_memoria, NULL);
   return 0;
}

int8_t memoria_iniciar_proceso(u_int32_t pid, char *path)
{
   t_kernel_mem_req *mem_request = crear_kernel_mem_request(INICIAR_PROCESO, pid, path);

   pthread_mutex_lock(&mutex_memoria);
   enviar_kernel_mem_request(fd_memoria, mem_request);
   pthread_mutex_unlock(&mutex_memoria);

   // habria que esperar la respuesta de la memoria
   return 0;
}

// TODO
int8_t memoria_finalizar_proceso(u_int32_t pid)
{
   return 0;
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}