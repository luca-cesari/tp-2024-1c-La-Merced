#ifndef PROCESOS_H
#define PROCESOS_H

#include <stdlib.h>
#include <mlist/mlist.h>
#include <pcb/pcb.h>

/**
 * @brief
 *
 */
void inicializar_lista_procesos(void);

/**
 * @brief
 *
 * @param pcb
 */
void registrar_proceso(t_pcb *pcb);

/**
 * @brief Busca un proceso por su pid en la lista de procesos,
 *        y retorna su estado. Si no lo encuentra, retorna -1.
 *
 * @param pid
 * @return `state`
 */
int8_t obtener_estado_por_pid(u_int32_t pid);

/**
 * @brief
 *
 * @param pid
 * @return `t_pcb*`
 */
t_pcb *obtener_proceso_por_pid(u_int32_t pid);

/**
 * @brief Remueve un proceso de la lista de procesos por su pid.
 *        pero no hace nada con el proceso, tampoco lo retorna.
 *
 * @param pid
 *
 * @note No se encarga de destruir el proceso.
 * @note Asegurar tener la referencia (t_pcb*) del proceso previo a la llamada.
 */
void quitar_proceso_por_pid(u_int32_t pid);

/**
 * @brief
 *
 */
void destruir_lista_procesos(void);

#endif // PROCESOS_H