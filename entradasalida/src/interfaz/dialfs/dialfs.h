#ifndef DIALFS_H
#define DIALFS_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <sockets/sockets.h>
#include <config/config.h>
#include "instrucciones.h"

void inicializar_interfaz_dialfs(void);
void inicializar_archivo_bloques(void);
void inicializar_archivo_bitmap(void);

#endif // DIALFS_H