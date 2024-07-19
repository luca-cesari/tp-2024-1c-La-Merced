#ifndef BITMAP_H
#define BITMAP_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>
#include <sockets/sockets.h>
#include <config/config.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <conexion/kernel.h>

typedef enum
{
    LIBRE,
    OCUPADO
} estado;

void inicializar_archivo_bitmap(void);
/**
 * @brief Modifica el estado de un bloque en el bitmap
 *
 * @note Se puede setear el estado en LIBRE u OCUPADO
 */
void modificar_bitmap(u_int32_t nro_bloque, estado estado);
/**
 * @brief Obtiene el indice del primer bloque libre que encuentra en el bitmap
 *
 * @note Es el primero que encuentra, no el siguiente a partir de una posicion
 */
u_int32_t get_siguiente_bloque_libre();

u_int32_t hay_bloques_libres_contiguos(u_int32_t bloque_inicial, u_int32_t cantidad_bloques);

void liberar_bitmap_a_partir_de(u_int32_t indice);

#endif