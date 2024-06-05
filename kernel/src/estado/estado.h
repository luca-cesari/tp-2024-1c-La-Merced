#ifndef ESTADO_H
#define ESTADO_H

#include <stdlib.h>
#include <semaphore.h>
#include <pcb/pcb.h>
#include <mlist/mlist.h>
#include <mqueue/mqueue.h>

#include "logger/logger.h"

typedef struct
{
   t_mutext_list *lista;
   sem_t *hay_proceso;
   state cod_estado;
} q_estado;

q_estado *crear_estado(state coigo_estado);
void push_proceso(q_estado *, t_pcb *);
void *pop_proceso(q_estado *);

/**
 * @brief No tiene en cuenta si adentro del estado había procesos encolados
 *
 */
void destruir_estado(q_estado *);

#endif // ESTADO_H