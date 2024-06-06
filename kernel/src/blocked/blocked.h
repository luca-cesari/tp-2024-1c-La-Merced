#ifndef BLOCKED_H
#define BLOCKED_H

#include <mlist/mlist.h>

#include "io_queue.h"
#include "resource_queue.h"

typedef struct
{
   t_mutext_list *io_interfaces;
   t_mutext_list *resources;
} q_blocked;

q_blocked *crear_estado_blocked(void);
void destruir_estado_blocked(q_blocked *);

/**
 * @brief
 *
 * @param estado
 * @param cola_io
 * @param rutina_consumo
 */
void conectar_nueva_interfaz(q_blocked *estado, io_queue *cola_io, void *(*rutina_consumo)(void *));

/**
 * @brief
 *
 * @param estado
 * @param pcb
 *
 * @return int32_t : 0 si fue exitosa, -1 si hubo un error
 */
int32_t bloquear_para_io(q_blocked *estado, t_pcb *pcb);

/**
 * @brief
 *
 * @param estado
 * @param fd_conexion
 * @return q_estado*
 */
q_estado *desconectar_interfaz(q_blocked *estado, int32_t fd_conexion);

/**
 * @brief
 *
 * @param estado
 * @param cola_recurso
 * @param rutina_consumo
 */
void cargar_recurso(q_blocked *estado, resource_queue *cola_recurso, void *(*rutina_consumo)(void *));

/**
 * @brief
 *
 * @param estado
 * @param pcb
 *
 * @return int32_t : 0 si fue exitosa, -1 si hubo un error
 */
int32_t bloquear_para_recurso(q_blocked *estado, t_pcb *pcb);

#endif // BLOCKED_H
