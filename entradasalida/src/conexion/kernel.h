#ifndef CONEXION_KERNEL_H
#define CONEXION_KERNEL_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <protocol/pcb.h>

#include "config/config.h"



int8_t conectar_con_kernel();

void liberar_conexion_kernel();

#endif // CONEXION_KERNEL_H