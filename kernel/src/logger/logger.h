#ifndef KERNEL_LOGGER_H
#define KERNEL_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <pcb/pcb.h>

#define LOG_FILE "kernel.log"

typedef enum
{
   INTERFAZ,
   RECURSO
} motivo_bloqueo;

typedef enum
{
   NORMAL,
   PRIORIDAD
} tipo_cola_ready;

void iniciar_logger(void);
void destruir_logger(void);

void log_evento(char *);
void log_creacion_proceso(u_int32_t);
void log_finalizacion_proceso(u_int32_t, motivo_finalizacion);
void log_cambio_de_estado(u_int32_t, state, state);
void log_motivo_bloqueo(u_int32_t, motivo_bloqueo, char *);
void log_fin_de_quantum(u_int32_t);
void log_envio_de_interrupcion(u_int32_t);
void log_ingreso_a_ready(t_list *, tipo_cola_ready);
void log_comando_ingresado(char *);

#endif // KERNEL_LOGGER_H