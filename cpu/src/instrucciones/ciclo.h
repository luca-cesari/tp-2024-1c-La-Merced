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

char *fetch();
void (*decode(char *char_instruccion))(Parametros);
void execute(void (*instruccion)(Parametros), char *char_instruccion);
int check_interrupt();
int check_desalojo();
void *ciclo_instruccion(t_pcb *pcb_kernel);

/// FUNCIONES DECODE ////////////

char **instruccion_parametros(char *char_instruccion);
int es_numero(char *parametro);
int char_a_numero(char *parametro);
Parametros obtener_parametros(char **parametros);
Parametro buscar_operando(char *parametro);
void set_diccionario_instrucciones(t_dictionary *);
void set_diccionario_registros(t_dictionary *);
void aumentar_program_counter();
void inicializar_diccionarios_inst_reg();



#endif // CICLO_H