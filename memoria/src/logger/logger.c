#include "logger.h"

t_log *memoria_logger;

void iniciar_logger(void)
{
    memoria_logger = log_create(LOG_FILE, "Memoria", true, LOG_LEVEL_INFO);
}

void destruir_logger(void)
{
    log_destroy(memoria_logger);
}

void log_evento(char *evento)
{
    log_info(memoria_logger, "%s", evento);
}

void log_creacion_tabla_paginas(u_int32_t pid, u_int32_t cantidad_paginas)
{
    log_info(memoria_logger, "PID: %d - Tamaño: %d", pid, cantidad_paginas);
}

void log_destruccion_tabla_paginas(u_int32_t pid, u_int32_t cantidad_paginas)
{
    log_info(memoria_logger, "PID: %d - Tamaño: %d", pid, cantidad_paginas);
}

void log_acceso_tabla_paginas(u_int32_t pid, u_int32_t pagina, u_int32_t marco)
{
    log_info(memoria_logger, "PID: %d - Pagina: %d - Marco: %d", pid, pagina, marco);
}

void log_ampliacion_proceso(u_int32_t pid, u_int32_t tamanio_actual, u_int32_t tamanio_a_ampliar)
{
    log_info(memoria_logger, "PID: %d - Tamaño Actual: %d - Tamaño a Ampliar: %d", pid, tamanio_actual, tamanio_a_ampliar);
}

void log_reduccion_proceso(u_int32_t pid, u_int32_t tamanio_actual, u_int32_t tamanio_a_reducir)
{
    log_info(memoria_logger, "PID: %d - Tamaño Actual: %d - Tamaño a Reducir: %d", pid, tamanio_actual, tamanio_a_reducir);
}

void log_acceso_espacio_usuario(u_int32_t pid, char *accion, u_int32_t direccion_fisica, u_int32_t tamanio)
{
    log_info(memoria_logger, "PID: %d - Accion: %s - Direccion fisica: %d - Tamaño: %d", pid, accion, direccion_fisica, tamanio);
}
