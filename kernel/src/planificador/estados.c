#include "estados.h"

q_estado *crear_estado(state codigo_estado)
{
   q_estado *estado = malloc(sizeof(q_estado));

   estado->queue = crear_mutex_queue();
   estado->hay_proceso = malloc(sizeof(sem_t));
   estado->cod_estado = codigo_estado;
   sem_init(estado->hay_proceso, 0, 0);

   return estado;
}

void push_proceso(q_estado *estado, t_pcb *pcb)
{
   push_mutex_queue(estado->queue, pcb);

   state anterior = pcb->estado;
   pcb->estado = estado->cod_estado;
   log_cambio_de_estado(pcb->pid, anterior, pcb->estado);

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
