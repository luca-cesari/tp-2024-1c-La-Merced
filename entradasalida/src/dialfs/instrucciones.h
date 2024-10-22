#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <sockets/sockets.h>
#include <extended/string.h>
#include <mem_request/entradasalida.h>

#include "config/config.h"
#include "conexion/memoria.h"
#include "metadata.h"
#include "bitmap.h"
#include "bloques.h"
#include "logger/logger.h"

void inicializar_dicc_instrucciones();
int8_t (*get_funcion_instruccion(char *instruccion))(char *, u_int32_t);

/**
 * @brief Crea un archivo en el sistema de archivos
 *
 * @note Recibe como argumento el path del archivo a crear
 */
int8_t io_fs_create(char *argumentos, u_int32_t pid);

/**
 * @brief Crea un archivo en el sistema de archivos
 *
 * @note Recibe como argumento el path del archivo a eliminar
 */
int8_t io_fs_delete(char *argumentos, u_int32_t pid);

/**
 * @brief Aumenta o disminuye el tamaño de un archivo
 *
 * @note Recibe como argumentos el path del archivo a truncar y el nuevo tamaño
 */
int8_t io_fs_truncate(char *argumentos, u_int32_t pid);
int8_t io_fs_write(char *argumentos, u_int32_t pid);
int8_t io_fs_read(char *argumentos, u_int32_t pid);

/**
 * @brief Mueve un archivo a la primera posicion donde encuentre espacio contiguo disponible
 *
 * @note Recibe como argumentos el path del archivo a mover
 */
void compactar(char *path, u_int32_t tamanio_archivo, u_int32_t cantidad_bloques_ocupados);

void destruir_dicc_instrucciones();

#endif // DIALFS_H