#include <stdlib.h>





void crear_proceso(char *ruta_ejecutable);
void pasar_a_ready();
void pasar_a_exec();
void pasar_a_block();
void pasar_a_exit();

void iniciar_planificacion();
void detener_planificacion();

void planificar_por_rr();
void planificar_por_vrr();
void planificar_por_fifo();
