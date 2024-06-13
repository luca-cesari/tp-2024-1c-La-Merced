#include "mem_response.h"

t_mem_buffer_response *crear_buffer_response(t_mem_response resultado, void *buffer, u_int32_t tamanio_buffer)
{
   // manejar el caso de que el buffer sea NULL
   return NULL;
}

t_packet *serializar_buffer_response(t_mem_buffer_response *buffer_response)
{
   return NULL;
}

void enviar_buffer_response(int32_t fd_conexion, t_mem_buffer_response *buffer_response)
{
}

t_mem_buffer_response *recibir_buffer_response(int32_t fd_conexion)
{
   return NULL;
}

void destruir_buffer_response(t_mem_buffer_response *buffer_response)
{
}