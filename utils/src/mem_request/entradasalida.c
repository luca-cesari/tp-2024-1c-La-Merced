#include "entradasalida.h"

t_io_mem_req *crear_io_mem_request(io_req_operation operacion, u_int32_t pid, char *direcciones_fisicas, u_int32_t tamanio_buffer, void *buffer)
{
   t_io_mem_req *mem_request = malloc(sizeof(t_io_mem_req));
   mem_request->operacion = operacion;
   mem_request->pid = pid;
   mem_request->direcciones_fisicas = direcciones_fisicas;
   mem_request->tamanio_buffer = tamanio_buffer;
   mem_request->buffer = NULL;

   if (buffer != NULL)
   {
      mem_request->buffer = malloc(tamanio_buffer);
      memcpy(mem_request->buffer, buffer, tamanio_buffer);
   }

   return mem_request;
}

void enviar_io_mem_request(int32_t fd_memoria, t_io_mem_req *mem_request)
{
   t_packet *paquete = crear_paquete();
   crear_buffer(paquete);
   agregar_a_paquete(paquete, &(mem_request->operacion), sizeof(t_io_mem_req));
   agregar_a_paquete(paquete, &(mem_request->pid), sizeof(u_int32_t));
   switch (mem_request->operacion)
   {
   case LEER_IO:
      agregar_a_paquete(paquete, mem_request->direcciones_fisicas, strlen(mem_request->direcciones_fisicas) + 1);
      agregar_a_paquete(paquete, &(mem_request->tamanio_buffer), sizeof(u_int32_t));
      break;
   case ESCRIBIR_IO:
      agregar_a_paquete(paquete, mem_request->direcciones_fisicas, strlen(mem_request->direcciones_fisicas) + 1);
      agregar_a_paquete(paquete, &(mem_request->tamanio_buffer), sizeof(u_int32_t));
      agregar_a_paquete(paquete, mem_request->buffer, mem_request->tamanio_buffer);
      break;
   }

   enviar_paquete(paquete, fd_memoria);
   eliminar_paquete(paquete);
}

t_io_mem_req *recibir_io_mem_request(int32_t fd_io)
{
   t_list *paquete = recibir_paquete(fd_io);
   if (paquete == NULL)
      return NULL;

   t_io_mem_req *mem_request = malloc(sizeof(t_io_mem_req));

   mem_request->operacion = *(io_req_operation *)list_get(paquete, 0);
   mem_request->pid = *(u_int32_t *)list_get(paquete, 1);

   switch (mem_request->operacion)
   {
   case LEER_IO:
      mem_request->direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      break;
   case ESCRIBIR_IO:
      mem_request->direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      mem_request->buffer = malloc(mem_request->tamanio_buffer);
      memcpy(mem_request->buffer, list_get(paquete, 4), mem_request->tamanio_buffer);
      break;
   }

   list_destroy_and_destroy_elements(paquete, free);
   return mem_request;
}

void destruir_io_mem_request(t_io_mem_req *mem_request)
{
   free(mem_request->direcciones_fisicas);
   if (mem_request->buffer != NULL)
      free(mem_request->buffer);
   free(mem_request);
}