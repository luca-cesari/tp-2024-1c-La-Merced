#ifndef MEMORIA_LOGGER_H
#define MEMORIA_LOGGER_H

#include <stdlib.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/log.h>

#define LOG_FILE "memoria.log"

// Funciones de inicialización y liberación de recursos
void iniciar_logger(void);
void destruir_logger(void);

// Funciones de log
void log_evento(char *evento);
void log_creacion_tabla_paginas(u_int32_t pid, u_int32_t cantidad_paginas);
void log_destruccion_tabla_paginas(u_int32_t pid, u_int32_t cantidad_paginas);
void log_acceso_tabla_paginas(u_int32_t pid, u_int32_t pagina, u_int32_t marco);
void log_ampliacion_proceso(u_int32_t pid, u_int32_t tamanio_actual, u_int32_t tamanio_a_ampliar);
void log_reduccion_proceso(u_int32_t pid, u_int32_t tamanio_actual, u_int32_t tamanio_a_reducir);
void log_acceso_espacio_usuario(u_int32_t pid, char *accion, u_int32_t direccion_fisica, u_int32_t tamanio);

#endif /* MEMORIA_LOGGER_H */