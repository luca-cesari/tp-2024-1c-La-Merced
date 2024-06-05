#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <commons/temporal.h>
#include <mqueue/mqueue.h>
#include <pcb/pcb.h>

#include "logger/logger.h"
#include "estado/estado.h"
#include "blocked/blocked.h"
#include "conexion/cpu.h"
#include "conexion/memoria.h"

/**
 * @brief Inicializa el planificador del kernel.
 *
 * * Crea los estados.
 * * Crea los semaforos necesarios.
 * * Lanza las rutinas de planificacion (crea los hilos).
 */
void inicializar_planificador();

/**
 * @brief Destruye el planificador del kernel.
 *
 * * Destruye los estados.
 * * Destruye los semaforos creados.
 */
void destruir_planificador();

/**
 * @brief Inicia la planificación de los procesos.
 */
void iniciar_planificacion();

/**
 * @brief Detiene la planificación a largo plazo.
 *        Impide que se creen nuevos procesos y pase a ready.
 */
void detener_planificacion();

/**
 * En teoría es redefinir el semáforo de grado_multiprogramación.
 * Pero capaz hace falta que tener en cuenta:
 * 1. Que hacer con los procesos encolados (ready, exec y blocked)?
 * 2. Supuestamente grado_multiprogramación es un recurso compartido que varios hilos consumen, si lo quiero modificar en tiempo de ejecución, debería usar mutex?
 */
void modificar_grado_multiprogramacion(u_int32_t nuevo_grado);

/**
 * @brief Función encargada de generar un PCB y encolarlo para ser planificado.
 * @param ruta_ejecutable Ruta del archivo ejecutable.
 */
void ingresar_proceso(char *ruta_ejecutable);

/**
 * @brief Conecta una interfaz de entrada/salida a la cola de BLOCKED.
 *
 * @param nombre_interfaz nombre de la interfaz a conectar, único en todo el sistema.
 * @param fd_conexion file descriptor de la conexión a la interfaz de entrada/salida
 */
void conectar_entrada_salida(char *nombre_interfaz, int32_t fd_conexion);

#endif // PLANIFICADOR_H