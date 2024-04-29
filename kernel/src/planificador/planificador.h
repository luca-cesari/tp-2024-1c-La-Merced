#ifndef PLANIFICADOR_H
#define PLANIFICADOR_H

#include <stdlib.h>
#include <stdio.h>
#include <mqueue/mqueue.h>

void *crear_proceso(void *);
void pasar_a_new();
void pasar_a_ready();
void pasar_a_exec();
void pasar_a_block();
void pasar_a_exit();

void iniciar_planificacion();
void detener_planificacion();

void planificar_por_rr();
void planificar_por_vrr();
void planificar_por_fifo();

#endif // PLANIFICADOR_H