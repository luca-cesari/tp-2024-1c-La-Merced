#include "estados.h"

q_ready *crear_estado_ready()
{
   q_ready *cola = malloc(sizeof(q_ready));

   cola->queue_ready = crear_mutex_queue();
   sem_init(&(cola->hay_proceso_ready), 0, 0);

   return cola;
};

void push_proceso_ready(q_ready *cola_ready, t_pcb *pcb)
{
   push_mutex_queue(cola_ready->queue_ready, pcb);
   sem_post(&(cola_ready->hay_proceso_ready));
};

void *pop_proceso_ready(q_ready *cola_ready)
{
   sem_wait(&(cola_ready->hay_proceso_ready));
   return pop_mutex_queue(cola_ready->queue_ready);
};

void destruir_estado_ready(q_ready *cola_ready)
{
   destruir_mutex_queue(cola_ready->queue_ready);
   sem_destroy(&(cola_ready->hay_proceso_ready));
};