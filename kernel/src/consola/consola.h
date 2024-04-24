#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <commons/string.h>

#define EJECUTAR_SCRIPT "EJECUTAR_SCRIPT"
#define INICIAR_PROCESO "INICIAR_PROCESO"
#define FINALIZAR_PROCESO "FINALIZAR_PROCESO"
#define INICIAR_PLANIFICACION "INICIAR_PLANIFICACION"
#define DETENER_PLANIFICACION "DETENER_PLANIFICACION"
#define MULTIPROGRAMACION "MULTIPROGRAMACION"
#define PROCESO_ESTADO "PROCESO_ESTADO"

void iniciar_consola();
void ejecutar_comando(char *operacion, char *argumento);