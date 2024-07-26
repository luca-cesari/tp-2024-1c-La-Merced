#ifndef IO_LOGGER_H
#define IO_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>

#define LOG_FILE ".log"

void iniciar_logger(char *nombre_interfaz);
void destruir_logger(void);

void log_peticion(u_int32_t pid);
void log_operacion(u_int32_t pid, char *operacion);
void log_crear_archivo(u_int32_t pid, char *nombre_archivo);
void log_eliminar_archivo(u_int32_t pid, char *nombre_archivo);
void log_truncar_archivo(u_int32_t pid, char *nombre_archivo, u_int32_t tamanio);
void log_leer_archivo(u_int32_t pid, char *nombre_archivo, u_int32_t tamanio, u_int32_t puntero_a_archivo);
void log_escribir_archivo(u_int32_t pid, char *nombre_archivo, u_int32_t tamanio, u_int32_t puntero_a_archivo);
void log_inicio_compactacion(u_int32_t pid);
void log_fin_compactacion(u_int32_t pid);

#endif // IO_LOGGER_H