#include "kernel.h"

t_kernel_mem_req *crear_kernel_mem_request(kernel_req_operation operacion, u_int32_t pid, char *path)
{
   t_kernel_mem_req *mem_request = malloc(sizeof(t_kernel_mem_req));
   mem_request->operacion = operacion;
   mem_request->pid = pid;
   mem_request->path = path == NULL ? NULL : strdup(path);

   return mem_request;
}

void enviar_kernel_mem_request(int32_t fd_memoria, t_kernel_mem_req *mem_request)
{
   t_packet *paquete = crear_paquete();

   agregar_a_paquete(paquete, &(mem_request->operacion), sizeof(t_kernel_mem_req));
   agregar_a_paquete(paquete, &(mem_request->pid), sizeof(u_int32_t));
   if (mem_request->path != NULL)
      agregar_a_paquete(paquete, mem_request->path, strlen(mem_request->path) + 1);

   enviar_paquete(paquete, fd_memoria);
   eliminar_paquete(paquete);
}

t_kernel_mem_req *recibir_kernel_mem_request(int32_t fd_kernel)
{
   t_list *paquete = recibir_paquete(fd_kernel);
   t_kernel_mem_req *mem_request = malloc(sizeof(t_kernel_mem_req));

   mem_request->operacion = *(kernel_req_operation *)list_get(paquete, 0);
   mem_request->pid = *(u_int32_t *)list_get(paquete, 1);
   mem_request->path = NULL;

   if (mem_request->operacion == INICIAR_PROCESO)
      mem_request->path = strdup((char *)list_get(paquete, 2));

   list_destroy_and_destroy_elements(paquete, &free);
   return mem_request;
}

void destruir_kernel_mem_request(t_kernel_mem_req *mem_request)
{
   free(mem_request->path);
   free(mem_request);
}
