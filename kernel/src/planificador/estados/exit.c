#include "estados.h"

q_exit *crear_estado_exit()
{
   q_exit *cola = malloc(sizeof(q_exit));

   cola->queue_exit = crear_mutex_queue();
   sem_init(&(cola->hay_proceso_exit), 0, 0);

   return cola;
}

void push_proceso_exit(q_exit *cola, t_pcb *pcb)
{
   push_mutex_queue(cola->queue_exit, pcb);
   sem_post(&(cola->hay_proceso_exit));
}

void *pop_proceso_exit(q_exit *cola)
{
   sem_wait(&(cola->hay_proceso_exit));
   return pop_mutex_queue(cola->queue_exit);
}

void destruir_estado_exit(q_exit *cola)
{
   destruir_mutex_queue(cola->queue_exit);
   sem_destroy(&(cola->hay_proceso_exit));

   free(cola);
}