#ifndef UTILS_MEM_REQUEST_CPU_H
#define UTILS_MEM_REQUEST_CPU_H

#include <pcb/pcb.h>

typedef enum
{
   FETCH_INSTRUCCION,
   OBTENER_MARCO,
} cpu_req_operation;

typedef struct
{
   u_int32_t pid;
   u_int32_t nro_pag;
} obt_marco_parametros;

typedef struct
{
   cpu_req_operation operacion;
   union
   {
      t_pcb pcb;                                 // Presente solo para FETCH_INSTRUCCION
      obt_marco_parametros obt_marco_parametros; // Presente solo para OBTENER_MARCO
   } parametros;
} t_cpu_mem_req;

t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation operacion, t_pcb pcb, u_int32_t pid, u_int32_t nro_pag);
void enviar_cpu_mem_request(int32_t fd_memoria, t_cpu_mem_req *mem_request);
t_cpu_mem_req *recibir_cpu_mem_request(int32_t fd_cpu);
void destruir_cpu_mem_request(t_cpu_mem_req *mem_request);

#endif // UTILS_MEM_REQUEST_CPU_H