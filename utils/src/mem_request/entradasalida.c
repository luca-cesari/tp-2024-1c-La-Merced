#include "entradasalida.h"

t_io_mem_req *crear_io_mem_request(io_req_operation operacion, u_int32_t pid, parametros_io parametro)
{
   t_io_mem_req *mem_request = malloc(sizeof(t_io_mem_req));
   mem_request->operacion = operacion;
   mem_request->pid = pid;
   switch (operacion)
   {
   case LEER_IO:
      mem_request->parametros.param_leer.direcciones_fisicas = strdup(parametro.param_leer.direcciones_fisicas);
      mem_request->parametros.param_leer.tamanio_buffer = parametro.param_leer.tamanio_buffer;
      break;
   case ESCRIBIR_IO:
      mem_request->parametros.param_escribir.direcciones_fisicas = strdup(parametro.param_escribir.direcciones_fisicas);
      mem_request->parametros.param_escribir.tamanio_buffer = parametro.param_escribir.tamanio_buffer;
      mem_request->parametros.param_escribir.buffer = parametro.param_escribir.buffer;
      break;
   default:
      break;
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
      agregar_a_paquete(paquete, mem_request->parametros.param_leer.direcciones_fisicas, strlen(mem_request->parametros.param_leer.direcciones_fisicas) + 1);
      agregar_a_paquete(paquete, &(mem_request->parametros.param_leer.tamanio_buffer), sizeof(u_int32_t));
      break;
   case ESCRIBIR_IO:
      agregar_a_paquete(paquete, mem_request->parametros.param_escribir.direcciones_fisicas, strlen(mem_request->parametros.param_escribir.direcciones_fisicas) + 1);
      agregar_a_paquete(paquete, &(mem_request->parametros.param_escribir.tamanio_buffer), sizeof(u_int32_t));
      agregar_a_paquete(paquete, mem_request->parametros.param_escribir.buffer, mem_request->parametros.param_escribir.tamanio_buffer);
      break;
   default:
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
      mem_request->parametros.param_leer.direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->parametros.param_leer.tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      break;
   case ESCRIBIR_IO:
      mem_request->parametros.param_escribir.direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->parametros.param_escribir.tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      mem_request->parametros.param_escribir.buffer = (void *)list_get(paquete, 4);
      break;
   default:
      break;
   }

   list_destroy(paquete);
   return mem_request;
}

void destruir_io_mem_request(t_io_mem_req *mem_request)
{
   // VER SI SE USA
   // switch (mem_request->operacion)
   // {
   // default:
   //    break;
   // }
   free(mem_request);
}