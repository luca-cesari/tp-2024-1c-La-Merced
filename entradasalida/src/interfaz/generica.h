#ifndef IO_GENERICA_H
#define IO_GENERICA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sockets/sockets.h>

#include "config/config.h"
#include "logger/logger.h"
#include "conexion/kernel.h"

void iniciar_rutina_interfaz_generica(void);
void gen_sleep(char *);

#endif // IO_GENERICA_H