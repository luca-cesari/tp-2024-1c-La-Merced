#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <mqueue/mqueue.h>
#include <protocols/pcb.h>

#include "logger/logger.h"
#include "estados/estados.h"

/**
 * @fn inicializar_planificador
 * @brief Inicializa el planificador del kernel.
 *
 * * Crea los estados.
 * * Crea los semaforos necesarios.
 * * Lanza las rutinas de planificacion (crea los hilos).
 */
void inicializar_planificador();
void destruir_planificador();

void iniciar_planificacion();
void detener_planificacion();

/**
 * En teoría es redefinir el semáforo de grado_multiprogramación.
 * Pero capaz hace falta que tener en cuenta:
 * 1. Que hacer con los procesos encolados (ready, exec y blocked)?
 * 2. Supuestamente grado_multiprogramación es un recurso compartido que varios hilos consumen, si lo quiero modificar en tiempo de ejecución, debería usar mutex?
 */
void modificar_grado_multiprogramacion(u_int8_t);

/**
 * @brief Función encargada de generar un PCB y encolarlo para ser planificado.
 * @param ruta_ejecutable Ruta del archivo ejecutable.
 */
void ingresar_proceso(char *);

/**
 * @fn crear_proceso
 * @brief Planificador largo plazo, pasa de new a ready.
 *        No se ocupa de la transicion de bloqueado a ready
 */
void *crear_proceso();
void pasar_a_exit(u_int32_t);
// ...
void *finalizar_proceso();

void planificar_por_fifo();
void planificar_por_rr();
void planificar_por_vrr();

#endif // PLANIFICADOR_H