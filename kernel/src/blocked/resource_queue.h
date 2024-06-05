#ifndef BLOCKED_RESOURCE_QUEUE_H
#define BLOCKED_RESOURCE_QUEUE_H

#include <pthread.h>
#include "estado/estado.h"

typedef struct
{
   /* data */
} resource_queue;

resource_queue *crear_resource_queue(void);
void destruir_resource_queue(resource_queue *);

#endif // BLOCKED_RESOURCE_QUEUE_H