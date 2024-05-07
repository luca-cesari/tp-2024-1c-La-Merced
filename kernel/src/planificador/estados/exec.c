#include "estados.h"

q_exec *crear_estado_exec()
{
   q_exec *cola = malloc(sizeof(q_exec));

   cola->queue_exec = crear_mutex_queue();
   sem_init(&(cola->hay_proceso_exec), 0, 0);

   return cola;
}

void push_proceso_exec(q_exec *cola, t_pcb *pcb)
{
   push_mutex_queue(cola->queue_exec, pcb);
   sem_post(&(cola->hay_proceso_exec));
}

void *pop_proceso_exec(q_exec *cola)
{
   sem_wait(&(cola->hay_proceso_exec));
   return pop_mutex_queue(cola->queue_exec);
}

void destruir_estado_exec(q_exec *cola)
{
   destruir_mutex_queue(cola->queue_exec);
   sem_destroy(&(cola->hay_proceso_exec));

   free(cola);
}