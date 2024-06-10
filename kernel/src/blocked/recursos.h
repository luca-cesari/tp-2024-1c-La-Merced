#ifndef RECURSOS_H
#define RECURSOS_H

#include <stdlib.h>
#include <pthread.h>
#include <mlist/mlist.h>
#include <pcb/pcb.h>

#include "config/config.h"
#include "estado/estado.h"
#include "blocked.h"

/**
 * @note `INVALID` indica que el recurso no existe o que no se puede operar sobre el.
 * @note `ALL_RETAINED` indica que no hay instancias disponibles.
 * @note `ASSIGNED` indica que se asigno una instancia con exito.
 * @note `RELEASED` indica que se libero una instancia con exito
 */
typedef enum
{
   INVALID,
   ALL_RETAINED,
   ASSIGNED,
   RELEASED
} respuesta_solicitud;

typedef struct
{
   char *nombre_recurso;
   u_int32_t instancias;
   pthread_mutex_t mutex_instancias;
   t_mutext_list *asignados;
   q_estado *cola_procesos;
} resource_queue;

/**
 * @brief Inicializa las estructuras de recursos segun la configuracion
 *        y los agrega a la cola bloqueados de recursos.
 *
 * @param cola_recursos
 */
void inicializar_recursos(q_blocked *cola_recursos);

/**
 * @brief Destructor de la estructura resource_queue.
 *        Libera la memoria de la estructura y sus campos,
 *        si en la cola de procesos habian pcbs, los destruye.
 *
 * @param ptr_recurso
 *
 * @note Se utiliza como callback en mlist_destroy_and_destroy_elements,
 *       es por esta razón que recibe un `void*`.
 */
void destruir_resource_queue(void *ptr_recurso);

/**
 * @brief Busca un recurso en la cola de recursos bloqueados y respode segun el caso.
 *        Si el recurso esta disponible, se considera asignado y se decrementa la cantidad de instancias.
 *        Para cualquier otro caso, simplemente responde, no opera.
 *
 * @param estado
 * @param pid
 * @param nombre_recurso
 * @return `respuesta_solicitud` : `INVALID`, `ALL_RETAINED`, `ASSIGNED` son los posibles valores.
 */
respuesta_solicitud consumir_recurso(q_blocked *estado, u_int32_t pid, char *nombre_recurso);

/**
 * @brief Busca un recurso en la cola de recursos bloqueados y respode segun el caso.
 *        Si el recurso esta existe, se considera liberado y se incrementa la cantidad de instancias.
 *        Si un proceso intenta liberar un recurso que no tiene asignado, se considera inválido.
 *
 * @param estado
 * @param pid
 * @param nombre_recurso
 * @return `respuesta_solicitud` : `INVALID`, `RELEASED` son los posibles valores.
 */
respuesta_solicitud liberar_recurso(q_blocked *estado, u_int32_t pid, char *nombre_recurso);

/**
 * @brief Encola un pcb en la cola de procesos bloqueados para determinado recurso.
 *        El recurso en custion es el que se encuentra en el campo `resource` del pcb.
 *
 * @param estado
 * @param pcb
 *
 * @note El pcb debe tener un recurso asignado y se asume que el recurso existe.
 * @note Se deberia usar en conjunto con `consumir_recurso`. Bloquear unicamente en caso de `ALL_RETAINED`.
 */
void bloquear_para_recurso(q_blocked *estado, t_pcb *pcb);

/**
 * @brief Desencola un pcb de la cola de procesos bloqueados para determinado recurso.
 *        Implica que dicho proceso retiene el recurso.
 *
 * @param estado
 * @param nombre_recurso
 * @return `t_pcb*`
 *
 * @note Se deberia usar en conjunto con `liberar_recurso`. Desbloquear unicamente en caso de `RELEASED`.
 * @note Si no hay procesos en la cola, retorna NULL, y puede ser ignorado.
 */
t_pcb *desbloquear_para_recurso(q_blocked *estado, char *nombre_recurso);

/**
 * @brief Bloquea todas las colas de recursos para que no puedan ser operadas.
 *        No se puede consumir ni liberar recursos.
 *
 * @param estado
 */
void bloquear_colas_de_recursos(q_blocked *estado);

/**
 * @brief Desbloquea todas las colas de recursos para que puedan ser operadas.
 *        Se puede consumir y liberar recursos.
 *
 * @param estado
 */
void desbloquear_colas_de_recursos(q_blocked *estado);

#endif // RECURSOS_H
