
#ifndef BLOCKED_INTERFACES_H
#define BLOCKED_INTERFACES_H

#include <pthread.h>

#include "blocked.h"
#include "estado/estado.h"

typedef struct
{
   char *nombre_interfaz;
   int32_t fd_conexion;
   q_estado *cola_procesos;
   pthread_t rutina_consumo;
} io_queue;

/**
 * @brief
 *
 * @param nombre_interfaz
 * @param fd_conexion
 * @return io_queue*
 */
io_queue *crear_io_queue(char *nombre_interfaz, int32_t fd_conexion);

/**
 * @brief
 *
 * @param cola_io
 */
void destruir_io_queue(void *cola_io);

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
 */
void bloquear_colas_io(q_blocked *estado);

/**
 * @brief 
 * 
 * @param estado 
 */
void desbloquear_colas_io(q_blocked *estado);

#endif // BLOCKED_IO_QUEUE_H