#ifndef UTILS_MEM_REQUEST_ENTRADASALIDA_H
#define UTILS_MEM_REQUEST_ENTRADASALIDA_H

#include <stdlib.h>
#include <protocol/protocol.h>
typedef enum
{
   LEER_IO,
   ESCRIBIR_IO,
} io_req_operation;

typedef struct
{
   char *direcciones_fisicas;
   u_int32_t tamanio_buffer;
} param_leer;

typedef struct
{
   char *direcciones_fisicas;
   u_int32_t tamanio_buffer;
   void *buffer;
} param_escribir;

typedef union
{
   param_leer param_leer;
   param_escribir param_escribir;

} parametros;
typedef struct
{
   io_req_operation operacion;
   u_int32_t pid;
   parametros parametros;

} t_io_mem_req;

t_io_mem_req *crear_cpu_mem_request(io_req_operation operacion, u_int32_t pid, parametros parametro);
void enviar_cpu_mem_request(int32_t fd_memoria, t_io_mem_req *mem_request);
t_io_mem_req *recibir_cpu_mem_request(int32_t fd);
void destruir_cpu_mem_request(t_io_mem_req *mem_request);

#endif // UTILS_MEM_REQUEST_ENTRADASALIDA_H