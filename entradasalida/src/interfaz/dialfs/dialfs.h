#ifndef DIALFS_H
#define DIALFS_H

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

#include "instrucciones.h"

typedef enum
{
    LIBRE,
    OCUPADO
} estado;

void inicializar_interfaz_dialfs(void);
void inicializar_archivo_bloques(void);
void inicializar_archivo_bitmap(void);
void modificar_bitmap(int nro_bloque, estado estado);

#endif // DIALFS_H