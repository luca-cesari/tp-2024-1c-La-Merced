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
void modificar_bitmap(int nro_bloque, estado estado);

#endif