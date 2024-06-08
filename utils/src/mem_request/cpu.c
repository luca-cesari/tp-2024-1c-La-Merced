#include "cpu.h"

t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation operacion, int32_t program_counter, u_int32_t pid, u_int32_t nro_pag)
{
   t_cpu_mem_req *mem_request = malloc(sizeof(t_cpu_mem_req));
   mem_request->operacion = operacion;
   mem_request->pid = pid;
   switch (operacion)
   {
   case FETCH_INSTRUCCION:
      mem_request->parametros.program_counter = program_counter;
      break;
   case OBTENER_MARCO:
      mem_request->parametros.nro_pag = nro_pag;
      break;
   default:
      break;
   }

   return mem_request;
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
      agregar_a_paquete(paquete, &(mem_request->parametros.program_counter), sizeof(u_int32_t));
      break;
   case OBTENER_MARCO:
      agregar_a_paquete(paquete, &(mem_request->parametros.nro_pag), sizeof(u_int32_t));
      break;
   default:
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
      mem_request->parametros.program_counter = *(int32_t *)list_get(paquete, 2);
      break;
   case OBTENER_MARCO:
      mem_request->parametros.nro_pag = *(u_int32_t *)list_get(paquete, 3);
      break;
   default:
      break;
   }

   list_destroy(paquete);
   return mem_request;
}

void destruir_cpu_mem_request(t_cpu_mem_req *mem_request)
{
   // VER SI SE USA
   // switch (mem_request->operacion)
   // {
   // case FETCH_INSTRUCCION:
   //    break;
   // case OBTENER_MARCO:
   //    break;
   // default:
   //    break;
   // }
   free(mem_request);
}