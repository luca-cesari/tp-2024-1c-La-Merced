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
} param_leer_io;

typedef struct
{
   char *direcciones_fisicas;
   u_int32_t tamanio_buffer;
   void *buffer;
} param_escribir_io;

typedef union
{
   param_leer_io param_leer;
   param_escribir_io param_escribir;

} parametros_io;
typedef struct
{
   io_req_operation operacion;
   u_int32_t pid;
   parametros_io parametros;

} t_io_mem_req;

t_io_mem_req *crear_io_mem_request(io_req_operation operacion, u_int32_t pid, parametros_io parametro);
void enviar_io_mem_request(int32_t fd_memoria, t_io_mem_req *mem_request);
t_io_mem_req *recibir_io_mem_request(int32_t fd);
void destruir_io_mem_request(t_io_mem_req *mem_request);

#endif // UTILS_MEM_REQUEST_ENTRADASALIDA_H