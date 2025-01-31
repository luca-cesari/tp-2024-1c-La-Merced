#ifndef CONEXION_CPU_H
#define CONEXION_CPU_H

#include <stdlib.h>
#include <sockets/sockets.h>
#include <pcb/pcb.h>

#include "config/config.h"

typedef enum
{
   USER_INT,    // 0
   QUANTUM_INT, // 1
} interrupt_code;

/**
 * @brief Realiza la conexion y el handshake con el módulo CPU
 *        por el puerto de dispatch
 *
 * @return int8_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_dispatch
 */
int8_t conectar_por_dispatch();

/**
 * @brief Realiza la conexion y el handshake con el módulo CPU
 *        por el puerto de interrupt
 *
 * @return int8_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_interrupt
 */
int8_t conectar_por_interrupt();

/**
 * @brief Envia un PCB al módulo CPU
 *
 * @param pcb Puntero al PCB a enviar
 */
void enviar_pcb_cpu(t_pcb *pcb);

/**
 * @brief Recibe un PCB del módulo CPU (lee un PCB del socket)
 *
 * @return t_pcb* : Puntero al PCB recibido
 */
t_pcb *recibir_pcb_cpu();

/**
 * @brief Envía señal por el puerto de interrupción al módulo CPU
 *        para desalojar el proceso en ejecución
 *
 * @param code Código de interrupción a enviar
 */
void enviar_interrupcion(interrupt_code code);

/**
 * @brief Libera la conexion con el módulo CPU,
 *        cerrando los file descriptors tanto de dispatch como de interrupt
 */
void liberar_conexion_cpu();

#endif // CONEXION_CPU_H
