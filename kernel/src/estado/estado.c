#include "estado.h"

static void destruir_proceso(void *);

q_estado *crear_estado(state codigo_estado)
{
   q_estado *estado = malloc(sizeof(q_estado));

   estado->lista = mlist_create();
   estado->hay_proceso = malloc(sizeof(sem_t));
   estado->cod_estado = codigo_estado;
   sem_init(estado->hay_proceso, 0, 0);

   return estado;
}

void push_proceso(q_estado *estado, t_pcb *pcb)
{
   mlist_push_as_queue(estado->lista, pcb);

   state anterior = pcb->estado;
   set_estado_pcb(pcb, estado->cod_estado);
   log_cambio_de_estado(pcb->pid, anterior, pcb->estado);

   sem_post(estado->hay_proceso);
}

t_pcb *pop_proceso(q_estado *estado)
{
   sem_wait(estado->hay_proceso);
   return (t_pcb *)mlist_pop_as_queue(estado->lista);
}

t_pcb *peek_proceso(q_estado *estado)
{
   sem_wait(estado->hay_proceso);
   return (t_pcb *)mlist_peek(estado->lista);
}

void remover_primer_proceso(q_estado *estado)
{
   sem_wait(estado->hay_proceso);
   mlist_remove(estado->lista, 1);
}

int8_t hay_proceso(q_estado *estado)
{
   return !mlist_is_empty(estado->lista);
}

void destruir_estado(q_estado *estado)
{
   mlist_clean(estado->lista, &destruir_proceso);
   mlist_destroy(estado->lista);
   sem_destroy(estado->hay_proceso);
   free(estado);
}

static void destruir_proceso(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   destruir_pcb(pcb);
}
