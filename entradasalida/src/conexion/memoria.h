#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <pcb/pcb.h>

#include "config/config.h"
#include "mem_request/entradasalida.h"

/**
 * @brief Realiza la conexion y el handshake con el módulo Memoria
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_memoria
 */
int8_t conectar_con_memoria();

void enviar_mem_request(t_io_mem_req *mem_request);

u_int32_t recibir_valor();

void *recibir_mem_buffer();

/**
 * @brief Libera la conexion con el módulo Memoria
 */
void liberar_conexion_memoria();

#endif // CONEXION_MEMORIA_H