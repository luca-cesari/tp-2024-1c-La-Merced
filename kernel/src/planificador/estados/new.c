#include "estados.h"

q_new *crear_estado_new()
{
   q_new *cola = malloc(sizeof(q_new));

   cola->queue_new = crear_mutex_queue();
   sem_init(&(cola->hay_proceso_nuevo), 0, 0);

   return cola;
}

void push_proceso_nuevo(q_new *cola, t_pcb *pcb)
{
   push_mutex_queue(cola->queue_new, pcb);
   sem_post(&(cola->hay_proceso_nuevo));
}

void *pop_proceso_nuevo(q_new *cola)
{
   sem_wait(&(cola->hay_proceso_nuevo));
   return pop_mutex_queue(cola->queue_new);
}

void destruir_estado_new(q_new *cola)
{
   destruir_mutex_queue(cola->queue_new);
   sem_destroy(&(cola->hay_proceso_nuevo));

   free(cola);
}
