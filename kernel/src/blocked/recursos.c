#include "recursos.h"

static resource_queue *crear_recurso(char *, u_int32_t);
static resource_queue *buscar_recurso(q_blocked *, char *nombre_recurso);

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
   destruir_estado(recurso->cola_procesos);
   free(recurso);
}

respuesta_solicitud consumir_recurso(q_blocked *estado, char *nombre_recurso)
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

   return ASSIGNED;
}

respuesta_solicitud liberar_recurso(q_blocked *estado, char *nombre_recurso)
{
   resource_queue *recurso = buscar_recurso(estado, nombre_recurso);

   if (recurso == NULL)
      return INVALID;

   pthread_mutex_lock(&(recurso->mutex_instancias));
   recurso->instancias += 1;
   pthread_mutex_unlock(&(recurso->mutex_instancias));

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

   // el pop_proceso cuenta con semaforo
   // si llega a popear y pasa de linea, pcb existe,
   // por lo tanto no es necesario verificar si es NULL
   // y puede consumir la instancia

   t_pcb *pcb = pop_proceso(recurso->cola_procesos);

   pthread_mutex_lock(&(recurso->mutex_instancias));
   recurso->instancias -= 1;
   pthread_mutex_unlock(&(recurso->mutex_instancias));

   return pcb;
}

static resource_queue *crear_recurso(char *nombre_recurso, u_int32_t instancias)
{
   resource_queue *recurso = malloc(sizeof(resource_queue));
   recurso->nombre_recurso = strdup(nombre_recurso);
   recurso->instancias = instancias;
   pthread_mutex_init(&(recurso->mutex_instancias), NULL);
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