#ifndef KERNEL_LOGGER_H
#define KERNEL_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <pcb/pcb.h>

#define LOG_FILE "kernel.log"

void iniciar_logger(void);
void destruir_logger(void);

void log_creacion_proceso(u_int32_t);
void log_finalizacion_proceso(u_int32_t, motivo_finalizacion);
void log_cambio_de_estado(u_int32_t, char *, char *);
void log_motivo_bloqueo(u_int32_t, motivo_bloqueo, char *);
void log_fin_de_quantum(u_int32_t);
void log_ingreso_a_ready(void); // void para q compile, hay q ver si usar list o array

#endif // KERNEL_LOGGER_H