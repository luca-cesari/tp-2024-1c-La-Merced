#ifndef METADATA_H
#define METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>
#include <sockets/sockets.h>

#include "config/config.h"

void crear_archivo_metadata(char *path_archivo, u_int32_t bloque_inicial, u_int32_t tamanio_archivo);
/**
 * @brief Obtiene el bloque inicial de un archivo a partir de su metadata
 */
u_int32_t get_bloque_inicial(char *path_archivo);
/**
 * @brief Obtiene la cantidad de bloques ocupados por un archivo a partir de su metadata
 */
u_int32_t get_cantidad_bloques_ocupados(char *path_archivo);

#endif