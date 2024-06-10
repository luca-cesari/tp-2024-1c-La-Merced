#include "estado.h"

static void destruir_proceso(void *);

q_estado *crear_estado(state codigo_estado)
{
   q_estado *estado = malloc(sizeof(q_estado));

   estado->lista = mlist_create();
   estado->hay_proceso = malloc(sizeof(sem_t));
   estado->cod_estado = codigo_estado;
   sem_init(estado->hay_proceso, 0, 0);
   estado->gate_entrada = pgate_create();
   estado->gate_salida = pgate_create();

   return estado;
}

void push_proceso(q_estado *estado, t_pcb *pcb)
{
   set_gate(estado->gate_entrada);

   mlist_push_as_queue(estado->lista, pcb);

   state anterior = pcb->estado;
   set_estado_pcb(pcb, estado->cod_estado);
   log_cambio_de_estado(pcb->pid, anterior, pcb->estado);

   sem_post(estado->hay_proceso);
}

t_pcb *pop_proceso(q_estado *estado)
{
   set_gate(estado->gate_salida);
   sem_wait(estado->hay_proceso);
   return (t_pcb *)mlist_pop_as_queue(estado->lista);
}

t_pcb *peek_proceso(q_estado *estado)
{
   set_gate(estado->gate_salida);
   sem_wait(estado->hay_proceso);
   t_pcb *peeked = (t_pcb *)mlist_peek(estado->lista);
   sem_post(estado->hay_proceso);
   return peeked;
}

t_pcb *remove_proceso(q_estado *estado, u_int32_t pid)
{
   set_gate(estado->gate_salida);
   int32_t _es_proceso(void *pcb)
   {
      t_pcb *proceso = (t_pcb *)pcb;
      return proceso->pid == pid;
   };

   sem_wait(estado->hay_proceso);
   return (t_pcb *)mlist_remove_by_condition(estado->lista, &_es_proceso);
}

int8_t hay_proceso(q_estado *estado)
{
   return !mlist_is_empty(estado->lista);
}

void bloquear_estado(q_estado *estado)
{
   close_gate(estado->gate_salida);
   close_gate(estado->gate_entrada);
}

void desbloquear_estado(q_estado *estado)
{
   open_gate(estado->gate_entrada);
   open_gate(estado->gate_salida);
}

void destruir_estado(q_estado *estado)
{
   mlist_clean(estado->lista, &destruir_proceso);
   mlist_destroy(estado->lista);
   sem_destroy(estado->hay_proceso);
   pgate_destroy(estado->gate_entrada);
   pgate_destroy(estado->gate_salida);
   free(estado);
}

static void destruir_proceso(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   destruir_pcb(pcb);
}
