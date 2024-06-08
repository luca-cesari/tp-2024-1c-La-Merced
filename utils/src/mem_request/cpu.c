#include "cpu.h"

t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation operacion, t_pcb pcb, u_int32_t pid, u_int32_t nro_pag)
{
   t_cpu_mem_req *mem_request = malloc(sizeof(t_cpu_mem_req));
   mem_request->operacion = operacion;
   switch (operacion)
   {
   case FETCH_INSTRUCCION:
      mem_request->parametros.pcb = pcb;
      break;
   case OBTENER_MARCO:
      mem_request->parametros.obt_marco_parametros.pid = pid;
      mem_request->parametros.obt_marco_parametros.nro_pag = nro_pag;
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
   agregar_a_paquete(paquete, &(mem_request->parametros.pcb), sizeof(t_pcb));
   agregar_a_paquete(paquete, &(mem_request->parametros.obt_marco_parametros.pid), sizeof(u_int32_t));
   agregar_a_paquete(paquete, &(mem_request->parametros.obt_marco_parametros.nro_pag), sizeof(u_int32_t));

   enviar_paquete(paquete, fd_memoria);
   eliminar_paquete(paquete);
}

t_cpu_mem_req *recibir_cpu_mem_request(int32_t fd_cpu)
{
   t_list *paquete = recibir_paquete(fd_cpu);
   t_cpu_mem_req *mem_request = malloc(sizeof(t_cpu_mem_req));

   mem_request->operacion = *(cpu_req_operation *)list_get(paquete, 0);

   switch (mem_request->operacion)
   {
   case FETCH_INSTRUCCION:
      mem_request->parametros.pcb = *(t_pcb *)list_get(paquete, 1);
      break;
   case OBTENER_MARCO:
      mem_request->parametros.obt_marco_parametros.pid = *(u_int32_t *)list_get(paquete, 2);
      mem_request->parametros.obt_marco_parametros.nro_pag = *(u_int32_t *)list_get(paquete, 3);
      break;
   default:
      break;
   }

   list_destroy(paquete);
   return mem_request;
}

void destruir_cpu_mem_request(t_cpu_mem_req *mem_request)
{
   // EN PRINCIPIO NO SE USA
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