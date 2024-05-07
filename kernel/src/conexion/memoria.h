#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <protocols/pcb.h>

#include "config/config.h"

/**
 * @brief Realiza la conexion y el handshake con el módulo Memoria
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 * @note Se guarda el file descriptor en una variable global fd_memoria
 */
int8_t conectar_con_memoria();

/**
 * @brief En principio sera un send y recive simple
 *        donde la memoria responda segun si la reserva
 *        fue exitosa o no.
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 */
int8_t reservar_paginas(t_pcb *);

/**
 * @brief Notifica a la memoria que se liberen las paginas
 *        reservadas, operacion a realizar cuando finaliza
 *        un proceso.
 *
 * @return int32_t : 0 si la conexion fue exitosa, -1 si hubo un error
 */
int8_t liberar_memoria(t_pcb *);

/**
 * @brief Libera la conexion con el módulo Memoria
 */
void liberar_conexion_memoria();

#endif // CONEXION_MEMORIA_H