#ifndef CICLO_H
#define CICLO_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <mem_request/cpu.h>
#include <registers/registers.h>

#include "instrucciones.h"
#include "conexion/memoria.h"
#include "logger/logger.h"
#include "interrupcion/interrupcion.h"

void *ciclo_instruccion(t_pcb *pcb_kernel);

#endif // CICLO_H