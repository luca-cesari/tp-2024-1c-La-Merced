#ifndef BLOCKED_H
#define BLOCKED_H

#include "io_queue.h"
#include "resource_queue.h"

/**
 * MODIFICAR:
 * - usar mlist.
 * - agregar resource_queues
 *
 */
typedef struct
{
   t_list *io_queues;
   pthread_mutex_t io_queues_mutex;
} q_blocked;

q_blocked *crear_estado_blocked(void);
void destruir_estado_blocked(q_blocked *);

/**
 * @brief
 *
 * @param estado
 * @param cola_io
 * @param rutina_consumo
 */
void conectar_nueva_interfaz(q_blocked *estado, io_queue *cola_io, void *(*rutina_consumo)(void *));
int32_t bloquear_proceso(q_blocked *, t_pcb *);
q_estado *desconectar_interfaz(q_blocked *, int32_t);

#endif // BLOCKED_H
