#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>

#define CONFIG_FILE "memoria.config"

typedef struct
{
    u_int32_t puerto_escucha;
    u_int32_t tam_memoria;
    u_int32_t tam_pagina;
    char *path_instrucciones;
    u_int32_t retardo_respuesta;
} mem_config;

void iniciar_config(void);
void destruir_config(void);

mem_config get_memoria_config(void);

#endif // MEMORIA_CONFIG_H