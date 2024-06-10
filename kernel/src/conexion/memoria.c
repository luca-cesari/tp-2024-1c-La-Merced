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

   pthread_mutex_lock(&mutex_memoria);
   int32_t respuesta = recibir_senial(fd_memoria);
   pthread_mutex_unlock(&mutex_memoria);

   return respuesta;
}

void memoria_finalizar_proceso(u_int32_t pid)
{
   t_kernel_mem_req *mem_request = crear_kernel_mem_request(FINALIZAR_PROCESO, pid, NULL);

   pthread_mutex_lock(&mutex_memoria);
   enviar_kernel_mem_request(fd_memoria, mem_request);
   pthread_mutex_unlock(&mutex_memoria);
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}