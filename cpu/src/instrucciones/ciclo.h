#ifndef CICLO_H
#define CICLO_H

#include <registers/registers.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <commons/string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <commons/collections/dictionary.h>

#include "instrucciones.h"
#include "conexion/memoria.h"
#include "estructuras.h"
#include "logger/logger.h"
#include "interrupcion/interrupcion.h"

char *fetch();
void (*decode(char *char_instruccion))(char **array);
void execute(void (*instruccion)(char **array), char *char_instruccion);
int check_interrupt();
int check_desalojo();
void *ciclo_instruccion(t_pcb *pcb_kernel);

/// FUNCIONES DECODE ////////////

char **instruccion_parametros(char *char_instruccion);
void set_diccionario_instrucciones(t_dictionary *);
void inicializar_diccionario_instrucciones();

//// FUNCIONES EXECUTE////////
char **eliminar_primer_elemento(char **array);
void aumentar_program_counter();

#endif // CICLO_H