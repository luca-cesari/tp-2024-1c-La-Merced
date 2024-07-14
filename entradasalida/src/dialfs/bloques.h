#ifndef BLOQUES_H
#define BLQOUES_H

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

void inicializar_archivo_bloques(void);
void copiar_de_bloque_datos(char *buffer, u_int32_t bloque_inicial, u_int32_t tamanio_archivo);

#endif