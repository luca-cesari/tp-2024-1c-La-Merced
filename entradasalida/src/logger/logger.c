#include "logger.h"

t_log *io_logger;

void iniciar_logger(void)
{
   io_logger = log_create(LOG_FILE, "IO", true, LOG_LEVEL_INFO);
}

void destruir_logger(void)
{
   log_destroy(io_logger);
}

void log_operacion(u_int32_t pid, char* operacion)
{
   log_info(io_logger, "PID: %d - Operacion: %s ", pid, operacion);
}

void log_crear_archivo(u_int32_t pid, char* nombre_archivo)
{
   log_info(io_logger, "PID: %d - Crear Archivo: %s ", pid, nombre_archivo);
}

void log_eliminar_archivo(u_int32_t pid, char* nombre_archivo)
{
   log_info(io_logger, "PID: %d - Eliminar Archivo: %s ", pid, nombre_archivo);
}

void log_truncar_archivo(u_int32_t pid, char* nombre_archivo, u_int32_t tamanio)
{
   log_info(io_logger, "PID: %d - Truncar Archivo: %s - Tamaño: %s", pid, nombre_archivo, tamanio);
}

void log_leer_archivo(u_int32_t pid, char* nombre_archivo, u_int32_t tamanio, FILE* puntero_a_archivo)
{
   log_info(io_logger, "PID: %d - Truncar Archivo: %s - Tamaño a Leer: %s - Puntero Archivo: %s", pid, nombre_archivo, tamanio, puntero_a_archivo);
}

void log_escribir_archivo(u_int32_t pid, char* nombre_archivo, u_int32_t tamanio, FILE* puntero_a_archivo)
{
   log_info(io_logger, "PID: %d - Truncar Archivo: %s - Tamaño a Escribir: %s - Puntero Archivo: %s", pid, nombre_archivo, tamanio, puntero_a_archivo);
}

void log_inicio_compactacion(u_int32_t pid)
{
   log_info(io_logger, "PID: %d - Inicio Compactación", pid);
}

void log_fin_compactacion(u_int32_t pid)
{
   log_info(io_logger, "PID: %d - Fin Compactación", pid);
}

