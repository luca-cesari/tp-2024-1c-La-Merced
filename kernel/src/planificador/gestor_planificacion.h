#ifndef PLANIFICADOR_GESTOR_PLANIFICACION_H
#define PLANIFICADOR_GESTOR_PLANIFICACION_H

#include <stdlib.h>
#include <semaphore.h>

typedef enum
{
   EJECUTANDO,
   DETENIDO
} estado_planificador;

typedef struct
{
   sem_t *sem_ejecucion_proceso;
   sem_t *sem_creacion_proceso;
   sem_t *sem_manejo_desalojo;
   sem_t *sem_entrada_ready;
} t_gestor_planificacion;

/**
 * @brief Inicializa el gestor de planificación.
 *
 * @note Inicializa los semáforos necesarios.
 * @note Inicializa el estado del planificador como DETENIDO.
 */
void inicializar_gestor_planificacion(void);

/**
 * @brief Destruye el gestor de planificación.
 *
 * @note Destruye los semáforos.
 */
void destruir_gestor_planificacion(void);

/**
 * @brief Habilita la planificación haciendo post
 *        a los semáforos y marca el estado como EJECUTANDO.
 *
 * @note Si el planificador ya estaba en ejecución, no hace nada.
 * @note Realiza los sem_post en el siguiente orden:
 *      `sem_entrada_ready` >> `sem_manejo_desalojo` >> `sem_ejecucion_proceso` >> `sem_creacion_proceso`.
 */
void habilitar_planificador(void);

/**
 * @brief Detiene la planificación haciendo wait
 *        a los semáforos y marca el estado como DETENIDO.
 *
 * @note Si el planificador ya estaba detenido, no hace nada.
 * @note Realiza los sem_wait en el siguiente orden:
 *      `sem_creacion_proceso` >> `sem_ejecucion_proceso` >> `sem_manejo_desalojo` >> `sem_entrada_ready`.
 */
void deshabilitar_planificador(void);

/**
 * @brief Función bloqueante.
 *        Se desbloquea cuando la creación de un proceso esta permitida.
 */
void puede_crear_proceso(void);

/**
 * @brief Función bloqueante.
 *        Se desbloquea cuando la ejecución de un proceso esta permitida.
 */
void puede_ejecutar_proceso(void);

/**
 * @brief Función bloqueante.
 *        Se desbloquea cuando el manejo de desalojo esta permitido.
 */
void puede_manejar_desalojo(void);

/**
 * @brief Función bloqueante.
 *        Se desbloquea cuando un proceso puede entrar a la cola de READY.
 */
void puede_entrar_a_ready(void);

#endif // PLANIFICADOR_GESTOR_PLANIFICACION_H
