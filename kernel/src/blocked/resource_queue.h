#ifndef BLOCKED_RESOURCE_QUEUE_H
#define BLOCKED_RESOURCE_QUEUE_H

#include <pthread.h>
#include "estado/estado.h"

typedef struct
{
   /* data */
} resource_queue;

resource_queue *crear_resource_queue(char* nombre_recurso, int32_t instancias);
void destruir_resource_queue(resource_queue *cola_recurso);

#endif // BLOCKED_RESOURCE_QUEUE_H