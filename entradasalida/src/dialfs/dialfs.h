#ifndef DIALFS_H
#define DIALFS_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>
#include <sockets/sockets.h>

#include "logger/logger.h"
#include "config/config.h"
#include "conexion/kernel.h"
#include "instrucciones.h"
#include "bloques.h"

void inicializar_interfaz_dialfs(void);

#endif // DIALFS_H