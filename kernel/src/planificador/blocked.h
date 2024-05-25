
#ifndef PLANIFICADOR_BLOCKED_H
#define PLANIFICADOR_BLOCKED_H

#include "estados.h"

typedef struct
{
   char *nombre_interfaz;
   int32_t fd_conexion;
   q_estado *cola;
   pthread_t rutina_consumo;
} io_queue;

typedef struct
{
   t_list *io_queues;
   pthread_mutex_t io_queues_mutex;
} q_blocked;

q_blocked *crear_estado_blocked(void);
io_queue *crear_io_queue(char *, int32_t);

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
void destruir_estado_blocked(q_blocked *);
void destruir_io_queue(void *);

#endif // PLANIFICADOR_BLOCKED_H