#ifndef KERNEL_CONSOLA_H
#define KERNEL_CONSOLA_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <extended/string.h>

#include "planificador/planificador.h"

#define EJECUTAR_SCRIPT "EJECUTAR_SCRIPT"
#define INICIAR_PROCESO "INICIAR_PROCESO"
#define FINALIZAR_PROCESO "FINALIZAR_PROCESO"
#define INICIAR_PLANIFICACION "INICIAR_PLANIFICACION"
#define DETENER_PLANIFICACION "DETENER_PLANIFICACION"
#define MULTIPROGRAMACION "MULTIPROGRAMACION"
#define PROCESO_ESTADO "PROCESO_ESTADO"

void iniciar_consola();
void leer_script(char *);
void ejecutar_comando(char *, char *);

#endif // KERNEL_CONSOLA_H