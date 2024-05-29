#include "kernel.h"

t_kernel_mem_req *crear_kernel_mem_request(kernel_mem_req_type tipo, u_int32_t pid, char *path)
{
   t_kernel_mem_req *mem_request = malloc(sizeof(t_kernel_mem_req));
   mem_request->tipo = tipo;
   mem_request->pid = pid;

   switch (tipo)
   {
   case INICIAR_PROCESO:
      mem_request->parametros.path = strdup(path);
      break;
   case FINALIZAR_PROCESO:
      break;
   default:
      break;
   }

   return mem_request;
}

void enviar_kernel_mem_request(int32_t fd_memoria, t_kernel_mem_req *mem_request)
{
   t_packet *paquete = crear_paquete();
   crear_buffer(paquete);
   agregar_a_paquete(paquete, &mem_request->tipo, sizeof(t_kernel_mem_req));
   agregar_a_paquete(paquete, &mem_request->pid, sizeof(u_int32_t));
   agregar_a_paquete(paquete, mem_request->parametros.path, strlen(mem_request->parametros.path) + 1);

   enviar_paquete(paquete, fd_memoria);
   eliminar_paquete(paquete);
}

t_kernel_mem_req *recibir_kernel_mem_request(int32_t fd_kernel)
{
   t_list *paquete = recibir_paquete(fd_kernel);
   t_kernel_mem_req *mem_request = malloc(sizeof(t_kernel_mem_req));

   mem_request->tipo = *(kernel_mem_req_type *)list_get(paquete, 0);
   mem_request->pid = *(u_int32_t *)list_get(paquete, 1);

   switch (mem_request->tipo)
   {
   case INICIAR_PROCESO:
      mem_request->parametros.path = strdup((char *)list_get(paquete, 2));
      break;
   case FINALIZAR_PROCESO:
      break;
   default:
      break;
   }

   list_destroy(paquete);
   return mem_request;
}

void destruir_kernel_mem_request(t_kernel_mem_req *mem_request)
{
   switch (mem_request->tipo)
   {
   case INICIAR_PROCESO:
      free(mem_request->parametros.path);
      break;
   case FINALIZAR_PROCESO:
      break;
   default:
      break;
   }

   free(mem_request);
}
