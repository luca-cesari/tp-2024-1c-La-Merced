#include "recursos.h"

static resource_queue *crear_recurso(char *nombre_recurso, u_int32_t instancias);
static resource_queue *buscar_recurso(q_blocked *estado, char *nombre_recurso);
static int32_t index_of_pid(resource_queue *estado, u_int32_t pid);

void inicializar_recursos(q_blocked *cola_recursos)
{
   t_dictionary *config_recursos = get_recursos();

   void agregar_recurso(char *nombre_recurso, void *ptr_instancias)
   {
      u_int32_t instancias = *(u_int32_t *)ptr_instancias;
      resource_queue *recurso = crear_recurso(nombre_recurso, instancias);
      mlist_add(cola_recursos->lista_colas, recurso);
   };

   dictionary_iterator(config_recursos, &agregar_recurso);
   dictionary_destroy_and_destroy_elements(config_recursos, &free);
}

void destruir_resource_queue(void *ptr_recurso)
{
   resource_queue *recurso = (resource_queue *)ptr_recurso;
   free(recurso->nombre_recurso);
   mlist_clean(recurso->asignados, &free);
   mlist_destroy(recurso->asignados);
   destruir_estado(recurso->cola_procesos);
   free(recurso);
}

respuesta_solicitud consumir_recurso(q_blocked *estado, u_int32_t pid, char *nombre_recurso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);

   if (recurso == NULL)
      return INVALID;

   // La consigna indica una verificacion de menor estricto
   // pero deberia menor o igual
   if (recurso->instancias <= 0)
      return ALL_RETAINED;

   pthread_mutex_lock(&(recurso->mutex_instancias));
   recurso->instancias -= 1;
   pthread_mutex_unlock(&(recurso->mutex_instancias));

   u_int32_t *pid_asignado = malloc(sizeof(u_int32_t));
   *pid_asignado = pid;
   mlist_add(recurso->asignados, pid_asignado);

   return ASSIGNED;
}

respuesta_solicitud liberar_recurso(q_blocked *estado, u_int32_t pid, char *nombre_recurso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);
   if (recurso == NULL)
      return INVALID;

   int32_t index = index_of_pid(recurso, pid);
   if (index == -1)
      return INVALID;

   pthread_mutex_lock(&(recurso->mutex_instancias));
   recurso->instancias += 1;
   pthread_mutex_unlock(&(recurso->mutex_instancias));

   u_int32_t *pid_previo = mlist_remove(recurso->asignados, index);
   free(pid_previo);

   return RELEASED;
}

void bloquear_para_recurso(q_blocked *estado, t_pcb *pcb)
{
   resource_queue *recurso = buscar_recurso(estado, pcb->resource);
   push_proceso(recurso->cola_procesos, pcb);
}

t_pcb *desbloquear_para_recurso(q_blocked *estado, char *nombre_recurso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);

   // es necesario verificar si hay procesos en la cola
   // porque esta funcion no debe ser bloqueante.
   // para el caso donde no hay procesos en la cola
   // la proxima vez que se intente consumir, la funcion
   // consumir_recurso aprobará la solicitud directamente.
   if (!hay_proceso(recurso->cola_procesos))
      return NULL;

   // el pop_proceso cuenta con semaforo
   // si llega a popear y pasa de linea, pcb existe
   // y puede consumir la instancia
   t_pcb *pcb = pop_proceso(recurso->cola_procesos);

   // No debería caer nunca en INVALID o ALL_RETAINED
   consumir_recurso(estado, pcb->pid, nombre_recurso);

   return pcb;
}

static resource_queue *crear_recurso(char *nombre_recurso, u_int32_t instancias)
{
   resource_queue *recurso = malloc(sizeof(resource_queue));
   recurso->nombre_recurso = strdup(nombre_recurso);
   recurso->instancias = instancias;
   pthread_mutex_init(&(recurso->mutex_instancias), NULL);
   recurso->asignados = mlist_create();
   recurso->cola_procesos = crear_estado(BLOCKED);

   return recurso;
}

static resource_queue *buscar_recurso(q_blocked *estado, char *nombre_recurso)
{
   int32_t _es_buscado(void *ptr_recurso)
   {
      resource_queue *recurso = (resource_queue *)ptr_recurso;
      return strcmp(recurso->nombre_recurso, nombre_recurso) == 0;
   };

   return mlist_find(estado->lista_colas, &_es_buscado);
}

static int32_t index_of_pid(resource_queue *estado, u_int32_t pid)
{
   int32_t _es_pid(void *ptr_pid)
   {
      u_int32_t *pid_asignado = (u_int32_t *)ptr_pid;
      return *pid_asignado == pid;
   };

   return mlist_index_of(estado->asignados, &_es_pid);
}