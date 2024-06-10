#ifndef CPU_LOGGER_H
#define CPU_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>

#define LOG_FILE "cpu.log"

// faltan instrucciones, estas con las del primer checkpoint, ver si se implementa asi lo de pedir la insrtuccion o no
#define SET "SET"
#define SUM "SUM"
#define SUB "SUB"
#define JNZ "JNZ"
#define IO_GEN_SLEEP "IO_GEN_SLEEP"
typedef enum
{
   READ,
   WRITE
} accion;

void iniciar_logger(void);
void destruir_logger(void);

void log_fetch_instruccion(u_int32_t, u_int32_t);
void log_instruccion_ejecutada(u_int32_t pid, char *instruccion, char *parametros);
void log_tlb_hit(u_int32_t, int);
void log_tlb_miss(u_int32_t, int); // ver si se abstrae con el hit
void log_obtener_marco(u_int32_t, int, int);
void log_escritura_lectura_memoria(u_int32_t, accion, u_int32_t, char *); // ver si la implementaicon de leer esbribir queda asi

#endif // KERNEL_LOGGER_H