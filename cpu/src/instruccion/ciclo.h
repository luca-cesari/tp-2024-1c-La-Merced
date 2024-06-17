#ifndef CICLO_H
#define CICLO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <mem_request/cpu.h>
#include <registers/registers.h>

#include "instrucciones.h"
#include "conexion/memoria.h"
#include "logger/logger.h"
#include "interrupcion/interrupcion.h"

void inicializar_diccionario_instrucciones();
void *ciclo_instruccion(t_pcb *pcb_kernel);

#endif // CICLO_H