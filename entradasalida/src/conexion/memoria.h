#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <protocol/pcb.h>

#include "config/config.h"

/**
 * @brief Realiza la conexion y el handshake con el módulo Memoria
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_memoria
 */
int8_t conectar_con_memoria();

/**
 * @brief Libera la conexion con el módulo Memoria
 */
void liberar_conexion_memoria();

#endif // CONEXION_MEMORIA_H