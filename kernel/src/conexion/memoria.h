#ifndef KERNEL_CONEXION_MEM_H
#define KERNEL_CONEXION_MEM_H

#include <stdlib.h>
#include <sockets/sockets.h>

#include "config/config.h"

/**
 * @brief Realiza la conexion y el handshake con el módulo Memoria
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_memoria
 */
int8_t conectar_con_memoria();

// Ni idea todavía qué operaciones tiene que poder hacer con la memoria,
// serán cosas de pedir y recibir páginas, pero desconozco el formato y
// la implementación.

/**
 * @brief Libera la conexion con el módulo Memoria
 */
void liberar_conexion_memoria();

#endif // KERNEL_CONEXION_MEM_H