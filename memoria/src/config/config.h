#ifndef MEMORIA_CONFIG_H
#define MEMORIA_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>

#define CONFIG_FILE "memoria.config"

void iniciar_config(void);
void destruir_config(void);

char *get_puerto_escucha(void);
u_int32_t get_tamanio_memoria(void);
u_int32_t get_tamanio_pagina(void);
char *get_path_instrucciones(void);
u_int32_t get_retardo_respuesta(void);

#endif // MEMORIA_CONFIG_H