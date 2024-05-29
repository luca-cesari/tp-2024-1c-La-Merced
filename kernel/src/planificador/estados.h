#ifndef PLANIFICADOR_ESTADOS_H
#define PLANIFICADOR_ESTADOS_H

#include <stdlib.h>
#include <semaphore.h>
#include <pcb/pcb.h>
#include <mqueue/mqueue.h>

#include "logger/logger.h"

typedef struct
{
   t_mutex_queue *queue;
   sem_t *hay_proceso;
   state cod_estado;
} q_estado;

q_estado *crear_estado(state coigo_estado);
void push_proceso(q_estado *, t_pcb *);
void *pop_proceso(q_estado *);
void destruir_estado(q_estado *);

#endif // PLANIFICADOR_ESTADOS_H