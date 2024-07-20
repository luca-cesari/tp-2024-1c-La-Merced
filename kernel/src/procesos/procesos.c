#include "procesos.h"

t_mutext_list *procesos;

static void destruir_proceso(void *proceso);

void inicializar_lista_procesos()
{
   procesos = mlist_create();
}

void registrar_proceso(t_pcb *pcb)
{
   mlist_add(procesos, pcb);
}

t_pcb *obtener_proceso_por_pid(u_int32_t pid)
{
   int32_t _es_proceso(void *pcb)
   {
      t_pcb *proceso = (t_pcb *)pcb;
      return proceso->pid == pid;
   };

   return (t_pcb *)mlist_find(procesos, &_es_proceso);
}

t_list *obtener_procesos()
{
   return mlist_to_list(procesos);
}

void quitar_proceso_por_pid(u_int32_t pid)
{
   int32_t _es_proceso(void *pcb)
   {
      t_pcb *proceso = (t_pcb *)pcb;
      return proceso->pid == pid;
   };

   mlist_remove_by_condition(procesos, &_es_proceso);
}

void destruir_lista_procesos()
{
   mlist_clean(procesos, &destruir_proceso);
   mlist_destroy(procesos);
}

static void destruir_proceso(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   destruir_pcb(pcb);
}