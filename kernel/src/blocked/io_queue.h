
#ifndef BLOCKED_IO_QUEUE_H
#define BLOCKED_IO_QUEUE_H

#include <pthread.h>
#include "estado/estado.h"

typedef struct
{
   char *nombre_interfaz;
   int32_t fd_conexion;
   q_estado *cola;
   pthread_t rutina_consumo;
} io_queue;

io_queue *crear_io_queue(char *nombre_interfaz, int32_t fd_conexion);
void destruir_io_queue(io_queue *cola_io);

#endif // BLOCKED_IO_QUEUE_H