#include "mem_response.h"

t_mem_buffer_response *crear_buffer_response(t_mem_response resultado, void *buffer, u_int32_t tamanio_buffer)
{
   t_mem_buffer_response *mem_buffer_response = malloc(sizeof(t_mem_buffer_response));
   mem_buffer_response->resultado = resultado;
   mem_buffer_response->tamanio_buffer = tamanio_buffer;
   mem_buffer_response->buffer = NULL;
   if (buffer != NULL)
   {
      mem_buffer_response->buffer = malloc(tamanio_buffer);
      memcpy(mem_buffer_response->buffer, buffer, tamanio_buffer);
   }

   return mem_buffer_response;
}

t_packet *serializar_buffer_response(t_mem_buffer_response *buffer_response)
{
   t_packet *paquete = crear_paquete();

   agregar_a_paquete(paquete, &(buffer_response->resultado), sizeof(t_mem_response));
   if (buffer_response->buffer != NULL)
   {
      agregar_a_paquete(paquete, &(buffer_response->tamanio_buffer), sizeof(u_int32_t));
      agregar_a_paquete(paquete, buffer_response->buffer, buffer_response->tamanio_buffer);
   }

   return paquete;
}

void enviar_buffer_response(int32_t fd_conexion, t_mem_buffer_response *buffer_response)
{
   t_packet *paquete = serializar_buffer_response(buffer_response);
   enviar_paquete(paquete, fd_conexion);
   eliminar_paquete(paquete);
}

t_mem_buffer_response *recibir_buffer_response(int32_t fd_conexion)
{
   t_list *paquete = recibir_paquete(fd_conexion);
   t_mem_buffer_response *buffer_response = malloc(sizeof(t_mem_buffer_response));

   buffer_response->resultado = *(t_mem_response *)list_get(paquete, 0);

   if (buffer_response->resultado == OPERATION_SUCCEED)
   {
      buffer_response->tamanio_buffer = *(u_int32_t *)list_get(paquete, 1);
      buffer_response->buffer = malloc(buffer_response->tamanio_buffer);
      memcpy(buffer_response->buffer, list_get(paquete, 2), buffer_response->tamanio_buffer);
   }
   else
   {
      buffer_response->buffer = NULL;
      buffer_response->tamanio_buffer = 0;
   }

   list_destroy_and_destroy_elements(paquete, &free);
   return buffer_response;
}

void destruir_buffer_response(t_mem_buffer_response *buffer_response)
{
   free(buffer_response->buffer);
   free(buffer_response);
}