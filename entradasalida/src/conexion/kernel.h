#ifndef CONEXION_KERNEL_H
#define CONEXION_KERNEL_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <pcb/pcb.h>
#include <io_request/io_request.h>

#include "config/config.h"

int8_t conectar_con_kernel(char *);
t_io_request *esperar_instruccion();
void enviar_respuesta(t_io_response);
void liberar_conexion_kernel();

#endif // CONEXION_KERNEL_H