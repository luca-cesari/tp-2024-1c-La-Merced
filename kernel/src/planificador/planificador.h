#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <commons/temporal.h>
#include <pcb/pcb.h>

#include "logger/logger.h"
#include "sem_mp.h"
#include "procesos/procesos.h"
#include "estado/estado.h"
#include "blocked/interfaces.h"
#include "blocked/recursos.h"
#include "conexion/cpu.h"
#include "conexion/memoria.h"

/**
 * @brief Inicializa el planificador del kernel.
 *
 * @note Crea los estados.
 * @note Crea los semaforos necesarios.
 * @note Lanza las rutinas de planificacion (crea los hilos).
 */
void inicializar_planificador();

/**
 * @brief Destruye el planificador del kernel.
 *
 * @note Destruye los estados.
 * @note Destruye los semaforos creados.
 */
void destruir_planificador();

/**
 * @brief Inicia la planificación de los procesos.
 */
void iniciar_planificacion();

/**
 * @brief Detiene la planificación de corto y largo plazo.
 *        Impide que se creen nuevos procesos y pase a ready.
 *
 * @note El proceso que se encuentra en ejecución NO es desalojado, pero una vez que salga de EXEC se va a pausar el manejo de su motivo de desalojo.
 * @note Los procesos bloqueados van a pausar su transición a la cola de Ready.
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
 *
 * @param ruta_ejecutable Ruta del archivo ejecutable.
 */
void crear_proceso(char *ruta_ejecutable);

/**
 * @brief Función encargada de pasar un proceso a EXIT,
 *        sin importar en que estado se encuentre.
 *        Es una interrupción de usuario.
 *
 * @param pid PID del proceso a finalizar.
 */
void matar_proceso(u_int32_t pid);

/**
 * @brief Conecta una interfaz de entrada/salida a la cola de BLOCKED.
 *
 * @param nombre_interfaz nombre de la interfaz a conectar, único en todo el sistema.
 * @param fd_conexion file descriptor de la conexión a la interfaz de entrada/salida
 */
void conectar_entrada_salida(char *nombre_interfaz, int32_t fd_conexion);

#endif // PLANIFICADOR_H