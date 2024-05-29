#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>

#define CONFIG_FILE "memoria.config"

typedef struct
{
    char *puerto_escucha;
    u_int32_t tam_memoria;
    u_int32_t tam_pagina;
    char *path_instrucciones;
    u_int32_t retardo_respuesta;
} mem_config;

void iniciar_config(void);
void destruir_config(void);

char *get_puerto_escucha(void);
u_int32_t get_tamanio_memoria(void);
u_int32_t get_tamanio_pagina(void);
char *get_path_instrucciones(void);
u_int32_t get_retardo_respuesta(void);

#endif // MEMORIA_CONFIG_H