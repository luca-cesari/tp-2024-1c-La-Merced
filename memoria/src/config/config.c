#include "config.h"

t_config *memoria_config;

void iniciar_config(void)
{
   memoria_config = config_create(CONFIG_FILE);
}

void destruir_config(void)
{
   config_destroy(memoria_config);
}

mem_config get_memoria_config(void){
    mem_config config;
    config.puerto_escucha = config_get_string_value(memoria_config, "PUERTO_ESCUCHA");
    config.tam_memoria = config_get_int_value(memoria_config, "TAM_MEMORIA");
    config.tam_pagina = config_get_int_value(memoria_config, "TAM_PAGINA");
    config.path_instrucciones = config_get_string_value(memoria_config, "PATH_INSTRUCCIONES");
    config.retardo_respuesta = config_get_int_value(memoria_config, "RETARDO_RESPUESTA");
    return config;
}
