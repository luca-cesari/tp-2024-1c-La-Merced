#ifndef UTILS_MEM_REQUEST_CPU_H
#define UTILS_MEM_REQUEST_CPU_H

#include <stdlib.h>
#include <protocol/protocol.h>
typedef enum
{
   FETCH_INSTRUCCION,
   OBTENER_MARCO,
   LEER,
   ESCRIBIR,
   RESIZE
} cpu_req_operation;

typedef struct
{
   u_int32_t direccion_fisica;
   u_int32_t tamanio_buffer;
} param_leer;

typedef struct
{
   u_int32_t direccion_fisica;
   u_int32_t tamanio_buffer;
   void *buffer;
} param_escribir;

typedef union
{
   int32_t program_counter; // Presente solo para FETCH_INSTRUCCION
   u_int32_t nro_pag;       // Presente solo para OBTENER_MARCO
   u_int32_t tamanio_nuevo; // Presente solo para RESIZE
   param_leer param_leer;
   param_escribir param_escribir;

} parametros;
typedef struct
{
   cpu_req_operation operacion;
   u_int32_t pid;
   parametros parametros;

} t_cpu_mem_req;

t_cpu_mem_req *crear_cpu_mem_request(cpu_req_operation operacion, u_int32_t pid, parametros parametro);
void enviar_cpu_mem_request(int32_t fd_memoria, t_cpu_mem_req *mem_request);
t_cpu_mem_req *recibir_cpu_mem_request(int32_t fd_cpu);
void destruir_cpu_mem_request(t_cpu_mem_req *mem_request);

#endif // UTILS_MEM_REQUEST_CPU_H