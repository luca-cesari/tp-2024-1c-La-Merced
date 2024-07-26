#include "recursos.h"

static resource_queue *crear_recurso(char *nombre_recurso, u_int32_t instancias);
static resource_queue *buscar_recurso(q_blocked *estado, char *nombre_recurso);
static void _bloquear_recurso(void *ptr_recurso);
static void _desbloquear_recurso(void *ptr_recurso);

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
   pthread_mutex_destroy(&(recurso->mutex_recurso));
   destruir_estado(recurso->cola_procesos);
   free(recurso);
}

respuesta_solicitud consumir_recurso(q_blocked *estado, char *nombre_recurso, t_pcb *proceso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);
   if (recurso == NULL)
      return INVALID;

   // La consigna indica una verificacion de menor estricto
   // pero deberia menor o igual
   pthread_mutex_lock(&(recurso->mutex_recurso));
   if (recurso->instancias <= 0)
   {
      pthread_mutex_unlock(&(recurso->mutex_recurso));
      push_proceso(recurso->cola_procesos, proceso);
      return QUEUED;
   }

   recurso->instancias -= 1;

   u_int32_t *asignado = malloc(sizeof(u_int32_t));
   *asignado = proceso->pid;
   list_add(recurso->asignados, asignado);
   pthread_mutex_unlock(&(recurso->mutex_recurso));

   return ASSIGNED;
}

respuesta_solicitud liberar_recurso(q_blocked *estado, char *nombre_recurso, u_int32_t pid)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);
   if (recurso == NULL)
      return INVALID;

   int8_t _es_buscado(void *ptr_pid)
   {
      u_int32_t *pid_asignado = (u_int32_t *)ptr_pid;
      return *pid_asignado == pid;
   };

   pthread_mutex_lock(&(recurso->mutex_recurso));
   u_int32_t *pid_asignado = list_remove_by_condition(recurso->asignados, (void *)_es_buscado);
   if (pid_asignado == NULL)
   {
      pthread_mutex_unlock(&(recurso->mutex_recurso));
      return INVALID;
   }

   recurso->instancias += 1;
   pthread_mutex_unlock(&(recurso->mutex_recurso));

   return RELEASED;
}

t_pcb *desbloquear_encolado(q_blocked *estado, char *nombre_recurso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);
   // no se hace manejo de NULL
   // porque el recurso deberia existir (por contexto)

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

   // No debería caer nunca en INVALID o QUEUED
   consumir_recurso(estado, nombre_recurso, pcb);

   return pcb;
}

// void bloquear_para_recurso(q_blocked *estado, t_pcb *pcb)
// {
//    resource_queue *recurso = buscar_recurso(estado, pcb->resource);
//    push_proceso(recurso->cola_procesos, pcb);
// }

void bloquear_colas_de_recursos(q_blocked *estado)
{
   mlist_iterate(estado->lista_colas, &_bloquear_recurso);
}

void desbloquear_colas_de_recursos(q_blocked *estado)
{
   mlist_iterate(estado->lista_colas, &_desbloquear_recurso);
}

t_pcb *remove_proceso_cola_recurso(q_blocked *estado, char *nombre_recurso, u_int32_t pid)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);
   if (recurso == NULL)
      return NULL;

   return remove_proceso(recurso->cola_procesos, pid);
}

static resource_queue *crear_recurso(char *nombre_recurso, u_int32_t instancias)
{
   resource_queue *recurso = malloc(sizeof(resource_queue));
   recurso->nombre_recurso = strdup(nombre_recurso);
   recurso->instancias = instancias;
   recurso->cola_procesos = crear_estado(BLOCKED);
   recurso->asignados = list_create();
   pthread_mutex_init(&(recurso->mutex_recurso), NULL);

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

void _bloquear_recurso(void *ptr_recurso)
{
   resource_queue *recurso = (resource_queue *)ptr_recurso;
   bloquear_estado(recurso->cola_procesos);
};

void _desbloquear_recurso(void *ptr_recurso)
{
   resource_queue *recurso = (resource_queue *)ptr_recurso;
   desbloquear_estado(recurso->cola_procesos);
};
