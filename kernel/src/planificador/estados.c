#include "estados.h"

q_estado *crear_estado()
{
   q_estado *estado = malloc(sizeof(q_estado));

   estado->queue = crear_mutex_queue();
   estado->hay_proceso = malloc(sizeof(sem_t));
   sem_init(estado->hay_proceso, 0, 0);

   return estado;
}

void push_proceso(q_estado *estado, t_pcb *pcb)
{
   push_mutex_queue(estado->queue, pcb);
   sem_post(estado->hay_proceso);
}

void *pop_proceso(q_estado *estado)
{
   sem_wait(estado->hay_proceso);
   return pop_mutex_queue(estado->queue);
}

void destruir_estado(q_estado *estado)
{
   destruir_mutex_queue(estado->queue);
   sem_destroy(estado->hay_proceso);

   free(estado);
}
