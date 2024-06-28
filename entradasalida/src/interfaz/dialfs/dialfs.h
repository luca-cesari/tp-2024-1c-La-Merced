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
#include <conexion/kernel.h>

#include "instrucciones.h"
#include "bloques.h"

void inicializar_interfaz_dialfs(void);

#endif // DIALFS_H