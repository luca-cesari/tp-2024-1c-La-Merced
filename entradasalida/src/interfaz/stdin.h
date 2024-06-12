#ifndef STDIN_H
#define STDIN_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <string.h>
#include <sockets/sockets.h>

#include "config/config.h"
#include "logger/logger.h"
#include "conexion/kernel.h"
#include "conexion/memoria.h"
#include "consola/consola.h"
#include "mem_request/entradasalida.h"

void iniciar_rutina_interfaz_stdin(void);
void io_stdin_read(char *argumentos, u_int32_t pid);
char *array_a_string(char **array);

#endif // STDIN_H