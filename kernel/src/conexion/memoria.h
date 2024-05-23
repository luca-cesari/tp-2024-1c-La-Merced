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
 * @brief Envia la instruccion de iniciar proceso a la memoria
 *
 * @param u_int32_t : pid del proceso a iniciar
 * @param char * : path del archivo a ejecutar
 *
 * @return int8_t : 0 si fue exitosa, -1 si hubo un error
 */
int8_t memoria_iniciar_proceso(u_int32_t, char *);

/**
 * @brief Notifica a la memoria que se liberen las paginas
 *        reservadas por el proceso
 *
 * @param u_int32_t : pid del proceso a finalizar
 *
 * @return int8_t : 0 si fue exitosa, -1 si hubo un error
 */
int8_t memoria_finalizar_proceso(u_int32_t pid);

/**
 * @brief Libera la conexion con el módulo Memoria
 */
void liberar_conexion_memoria();

#endif // CONEXION_MEMORIA_H