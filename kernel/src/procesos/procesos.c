#include "procesos.h"

t_mutext_list *procesos;

static t_pcb *buscar_proceso_por_pid(u_int32_t pid);
static void destruir_proceso(void *proceso);

void inicializar_lista_procesos()
{
   procesos = mlist_create();
}

void registrar_proceso(t_pcb *pcb)
{
   mlist_add(procesos, pcb);
}

int8_t obtener_estado_por_pid(u_int32_t pid)
{
   t_pcb *proceso = buscar_proceso_por_pid(pid);
   return proceso == NULL ? -1 : proceso->estado;
}

void quitar_proceso_por_pid(u_int32_t pid)
{
}

void destruir_lista_procesos()
{
   mlist_clean(procesos, &destruir_proceso);
   mlist_destroy(procesos);
}

static t_pcb *buscar_proceso_por_pid(u_int32_t pid)
{
   int32_t _es_proceso(void *pcb)
   {
      t_pcb *proceso = (t_pcb *)pcb;
      return proceso->pid == pid;
   };

   return (t_pcb *)mlist_find(procesos, &_es_proceso);
}

static void destruir_proceso(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   destruir_pcb(pcb);
}