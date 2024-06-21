#ifndef STDOUT_H
#define STDOUT_H

#include <stdlib.h>
#include <stdio.h>
#include <extended/string.h>
#include <mem_request/entradasalida.h>

#include "logger/logger.h"
#include "conexion/kernel.h"
#include "conexion/memoria.h"

void inicializar_interfaz_stdout(void);
int8_t io_stdout_write(char *argumentos, u_int32_t pid);

#endif // STDOUT_H