#include "cpu.h"

static t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation, u_int32_t, u_int32_t, u_int32_t, u_int32_t, char *, u_int32_t, void *);

static t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation operacion,
                                            u_int32_t pid,
                                            u_int32_t program_counter,
                                            u_int32_t nro_pag,
                                            u_int32_t tamanio_nuevo,
                                            char *direcciones_fisicas,
                                            u_int32_t tamanio_buffer,
                                            void *buffer)
{
   t_cpu_mem_req *mem_request = malloc(sizeof(t_cpu_mem_req));
   mem_request->operacion = operacion;
   mem_request->pid = pid;

   switch (operacion)
   {
   case FETCH_INSTRUCCION:
      mem_request->program_counter = program_counter;
      break;
   case OBTENER_MARCO:
      mem_request->nro_pag = nro_pag;
      break;
   case LEER:
      mem_request->direcciones_fisicas = strdup(direcciones_fisicas);
      mem_request->tamanio_buffer = tamanio_buffer;
      break;
   case ESCRIBIR:
      mem_request->direcciones_fisicas = strdup(direcciones_fisicas);
      mem_request->tamanio_buffer = tamanio_buffer;
      mem_request->buffer = malloc(tamanio_buffer);
      memcpy(mem_request->buffer, buffer, tamanio_buffer);
      break;
   case RESIZE:
      mem_request->tamanio_nuevo = tamanio_nuevo;
      break;
   }

   return mem_request;
}

t_cpu_mem_req *crear_instruccion_request(u_int32_t pid, u_int32_t program_counter)
{
   return crear_cpu_mem_request(FETCH_INSTRUCCION, pid, program_counter, 0, 0, NULL, 0, NULL);
}

t_cpu_mem_req *crear_nro_frame_request(u_int32_t pid, u_int32_t nro_pag)
{
   return crear_cpu_mem_request(OBTENER_MARCO, pid, 0, nro_pag, 0, NULL, 0, NULL);
}

t_cpu_mem_req *crear_resize_request(u_int32_t pid, u_int32_t tamanio_nuevo)
{
   return crear_cpu_mem_request(RESIZE, pid, 0, 0, tamanio_nuevo, NULL, 0, NULL);
}

t_cpu_mem_req *crear_cpu_read_request(u_int32_t pid, char *direcciones_fisicas, u_int32_t tamanio_buffer)
{
   return crear_cpu_mem_request(LEER, pid, 0, 0, 0, direcciones_fisicas, tamanio_buffer, NULL);
}

t_cpu_mem_req *crear_cpu_write_request(u_int32_t pid, char *direcciones_fisicas, u_int32_t tamanio_buffer, void *buffer)
{
   return crear_cpu_mem_request(ESCRIBIR, pid, 0, 0, 0, direcciones_fisicas, tamanio_buffer, buffer);
}

void enviar_cpu_mem_request(int32_t fd_memoria, t_cpu_mem_req *mem_request)
{
   t_packet *paquete = crear_paquete();
   crear_buffer(paquete);
   agregar_a_paquete(paquete, &(mem_request->operacion), sizeof(t_cpu_mem_req));
   agregar_a_paquete(paquete, &(mem_request->pid), sizeof(u_int32_t));

   switch (mem_request->operacion)
   {
   case FETCH_INSTRUCCION:
      agregar_a_paquete(paquete, &(mem_request->program_counter), sizeof(u_int32_t));
      break;
   case OBTENER_MARCO:
      agregar_a_paquete(paquete, &(mem_request->nro_pag), sizeof(u_int32_t));
      break;
   case LEER:
      agregar_a_paquete(paquete, mem_request->direcciones_fisicas, string_full_lenght(mem_request->direcciones_fisicas));
      agregar_a_paquete(paquete, &(mem_request->tamanio_buffer), sizeof(u_int32_t));
      break;
   case ESCRIBIR:
      agregar_a_paquete(paquete, mem_request->direcciones_fisicas, string_full_lenght(mem_request->direcciones_fisicas));
      agregar_a_paquete(paquete, &(mem_request->tamanio_buffer), sizeof(u_int32_t));
      agregar_a_paquete(paquete, mem_request->buffer, mem_request->tamanio_buffer);
      break;
   case RESIZE:
      agregar_a_paquete(paquete, &(mem_request->tamanio_nuevo), sizeof(u_int32_t));
      break;
   }
   enviar_paquete(paquete, fd_memoria);
   eliminar_paquete(paquete);
}

t_cpu_mem_req *recibir_cpu_mem_request(int32_t fd_cpu)
{
   t_list *paquete = recibir_paquete(fd_cpu);
   t_cpu_mem_req *mem_request = malloc(sizeof(t_cpu_mem_req));

   mem_request->operacion = *(cpu_req_operation *)list_get(paquete, 0);
   mem_request->pid = *(u_int32_t *)list_get(paquete, 1);

   switch (mem_request->operacion)
   {
   case FETCH_INSTRUCCION:
      mem_request->program_counter = *(int32_t *)list_get(paquete, 2);
      break;
   case OBTENER_MARCO:
      mem_request->nro_pag = *(u_int32_t *)list_get(paquete, 2);
      break;
   case LEER:
      mem_request->direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      break;
   case ESCRIBIR:
      mem_request->direcciones_fisicas = strdup(list_get(paquete, 2));
      mem_request->tamanio_buffer = *(u_int32_t *)list_get(paquete, 3);
      mem_request->buffer = malloc(mem_request->tamanio_buffer);
      memcpy(mem_request->buffer, list_get(paquete, 4), mem_request->tamanio_buffer);
      break;
   case RESIZE:
      mem_request->tamanio_nuevo = *(u_int32_t *)list_get(paquete, 2);
      break;
   default:
      break;
   }

   list_clean_and_destroy_elements(paquete, &free);
   return mem_request;
}

t_list *convertir_a_lista_de_direcciones_fisicas(char *direcciones_fisicas)
{
   t_list *lista_direcciones = list_create();
   char **direcciones = string_split(direcciones_fisicas, " ");
   for (int i = 0; direcciones[i] != NULL; i++)
   {
      list_add(lista_direcciones, direcciones[i]);
   }
   return lista_direcciones;
}

void destruir_cpu_mem_request(t_cpu_mem_req *mem_request)
{
   switch (mem_request->operacion)
   {
   case LEER:
      free(mem_request->direcciones_fisicas);
      break;
   case ESCRIBIR:
      free(mem_request->direcciones_fisicas);
      free(mem_request->buffer);
      break;
   default: // cualquier otro caso
      break;
   }
   free(mem_request);
}