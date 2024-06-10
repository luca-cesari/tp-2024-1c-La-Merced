#ifndef ESTADO_H
#define ESTADO_H

#include <stdlib.h>
#include <semaphore.h>
#include <pcb/pcb.h>
#include <mlist/mlist.h>

#include "logger/logger.h"

typedef struct
{
   t_mutext_list *lista;
   sem_t *hay_proceso;
   state cod_estado;
} q_estado;

/**
 * @brief Crea una cola de procesos para el estado dado.
 *
 * @param coigo_estado
 * @return `q_estado*`
 */
q_estado *crear_estado(state coigo_estado);

/**
 * @brief Encola un proceso en el estado dado.
 *
 * @param estado
 * @param pcb
 */
void push_proceso(q_estado *estado, t_pcb *pcb);

/**
 * @brief Desencola un proceso del estado dado.
 *        El puntero retornado nunca será NULL.
 *
 * @param estado
 * @return `t_pcb*`
 */
t_pcb *pop_proceso(q_estado *estado);

/**
 * @brief Devuelve el primer proceso encolado en el estado dado.
 *        El puntero retornado nunca será NULL.
 *
 * @param estado
 * @return `t_pcb*`
 */
t_pcb *peek_proceso(q_estado *estado);

/**
 * @brief
 *
 * @param estado
 * @param pid
 * @return `t_pcb*`
 *
 * @note Si el proceso no existe, retorna NULL.
 */
t_pcb *remove_proceso(q_estado *estado, u_int32_t pid);

/**
 * @brief Indica si hay algún proceso encolado en el estado dado.
 *
 * @param estado
 * @return `int8_t`
 */
int8_t hay_proceso(q_estado *estado);

/**
 * @brief Destruye un estado.
 *        Limpia la cola de procesos y libera la memoria,
 *        si había algún proceso encolado, lo destruye.
 *
 * @param estado
 */
void destruir_estado(q_estado *estado);

#endif // ESTADO_H
