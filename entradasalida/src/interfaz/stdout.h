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
#include "conexion/memoria.h"
#include "mem_request/entradasalida.h"
#include "conexion/memoria.h"

void inicializar_interfaz_stdout(void);
void io_stdout_write(char *argumentos, u_int32_t pid);
char *array_a_string(char **array);

#endif // STDOUT_H