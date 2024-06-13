#include "memoria.h"

int32_t fd_memoria;

int8_t conectar_con_memoria()
{
   mem_config mem = get_memoria_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto);
   int32_t response = handshake(fd_memoria, E_S);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   return 0;
}

void enviar_mem_request(t_io_mem_req *mem_request)
{
   enviar_io_mem_request(fd_memoria, mem_request);
}

u_int32_t recibir_valor()
{
   return recibir_senial(fd_memoria);
}

void *recibir_mem_buffer()
{
   t_mem_buffer_response *response = recibir_buffer_response(fd_memoria);
   return response == NULL ? NULL : response->buffer;
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}
