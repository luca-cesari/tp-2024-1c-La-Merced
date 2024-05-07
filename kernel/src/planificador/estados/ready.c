#include "estados.h"

q_ready *crear_estado_ready()
{
   q_ready *cola = malloc(sizeof(q_ready));

   cola->queue_ready = crear_mutex_queue();
   sem_init(&(cola->hay_proceso_ready), 0, 0);

   return cola;
};

void push_proceso_ready(q_ready *cola, t_pcb *pcb)
{
   push_mutex_queue(cola->queue_ready, pcb);
   sem_post(&(cola->hay_proceso_ready));
};

void *pop_proceso_ready(q_ready *cola)
{
   sem_wait(&(cola->hay_proceso_ready));
   return pop_mutex_queue(cola->queue_ready);
};

void destruir_estado_ready(q_ready *cola)
{
   destruir_mutex_queue(cola->queue_ready);
   sem_destroy(&(cola->hay_proceso_ready));

   free(cola);
};