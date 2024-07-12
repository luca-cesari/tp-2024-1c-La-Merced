#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <sockets/sockets.h>
#include <extended/string.h>
#include <mem_request/entradasalida.h>

#include "config/config.h"
#include "metadata.h"
#include "bitmap.h"

void inicializar_dicc_instrucciones();
void (*get_funcion_instruccion(char *instruccion))(char *, u_int32_t);
/**
 * @brief Crea un archivo en el sistema de archivos
 *
 * @note Recibe como argumento el path del archivo a crear
 */
void io_fs_create(char *argumentos, u_int32_t pid);
/**
 * @brief Crea un archivo en el sistema de archivos
 *
 * @note Recibe como argumento el path del archivo a eliminar
 */
void io_fs_delete(char *argumentos, u_int32_t pid);
/**
 * @brief Aumenta o disminuye el tamaño de un archivo
 *
 * @note Recibe como argumentos el path del archivo a truncar y el nuevo tamaño
 */
void io_fs_truncate(char *argumentos, u_int32_t pid);
// void io_fs_write(char *argumentos, u_int32_t pid);
// void io_fs_read(char *argumentos, u_int32_t pid);

/**
 * @brief Mueve un archivo a la primera posicion donde encuentre espacio contiguo disponible
 *
 * @note Recibe como argumentos el path del archivo a mover
 */
void compactar(char *path);

#endif // DIALFS_H