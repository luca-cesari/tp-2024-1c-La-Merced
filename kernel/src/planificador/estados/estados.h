#ifndef ESTADOS_H
#define ESTADOS_H

#include <stdlib.h>
#include <semaphore.h>
#include <protocols/pcb.h>
#include <mqueue/mqueue.h>

// Ver si crear todos los structs aca
// o armar un archivo por aca estado
// (porque también hay operaciones asociadas a cada estado)

typedef struct
{
   t_mutex_queue *queue_new;
   sem_t hay_proceso_nuevo;
} q_new;

q_new *crear_estado_new();
void push_proceso_nuevo(q_new *, t_pcb *);
void *pop_proceso_nuevo(q_new *);
void destruir_estado_new(q_new *);

typedef struct
{
   t_mutex_queue *queue_ready;
   sem_t hay_proceso_ready; // para el planificador de corto plazo (pasar a exec)
} q_ready;

q_ready *crear_estado_ready();
void push_proceso_ready(q_ready *, t_pcb *);
void *pop_proceso_ready(q_ready *);
void destruir_estado_ready(q_ready *);

#endif // ESTADOS_H