
#ifndef PLANIFICADOR_BLOCKED_H
#define PLANIFICADOR_BLOCKED_H

#include "estados.h"

typedef struct
{
   char *nombre_interfaz;
   int32_t fd_conexion;
   q_estado *cola;
} io_queue;

typedef struct
{
   t_list *io_queues;
   pthread_mutex_t io_queues_mutex;
} q_blocked;

q_blocked *crear_estado_blocked(void);
void conectar_interfaz(q_blocked *, char *, int32_t);
int32_t bloquear_proceso(q_blocked *, t_pcb *);
q_estado *desconectar_interfaz(q_blocked *, int32_t);
void destruir_estado_blocked(q_blocked *);

#endif // PLANIFICADOR_BLOCKED_H