#ifndef STDOUT_H
#define STDOUT_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <string.h>
#include <sockets/sockets.h>

#include "config/config.h"
#include "logger/logger.h"
#include "conexion/kernel.h"

void inicializar_interfaz_stdout(void);
void crear_instrucciones();
void io_stdout_write(char *parametros);

#endif // STDOUT_H