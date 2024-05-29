#ifndef UTILS_MEM_REQUEST_KERNEL_H
#define UTILS_MEM_REQUEST_KERNEL_H

#include <protocol/protocol.h>

typedef enum
{
   INICIAR_PROCESO,
   FINALIZAR_PROCESO,
   // Otros tipos de instrucciones si los hay
} kernel_mem_req_type;

typedef struct
{
   kernel_mem_req_type tipo;
   u_int32_t pid; // El PID está presente en todos los casos
   union
   {
      char *path; // Presente solo para INICIAR_PROCESO
                  // Otros parámetros específicos para otros tipos de instrucciones si los hay
   } parametros;
} t_kernel_mem_req;

t_kernel_mem_req *crear_kernel_mem_request(kernel_mem_req_type tipo, u_int32_t pid, char *path);
void enviar_kernel_mem_request(int32_t fd_memoria, t_kernel_mem_req *mem_request);
t_kernel_mem_req *recibir_kernel_mem_request(int32_t fd_kernel);
void destruir_kernel_mem_request(t_kernel_mem_req *mem_request);

#endif // UTILS_MEM_REQUEST_KERNEL_H