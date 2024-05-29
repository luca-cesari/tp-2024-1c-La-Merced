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

char *get_puerto_escucha(void)
{
   return config_get_string_value(memoria_config, "PUERTO_ESCUCHA");
}

u_int32_t get_tamanio_memoria(void)
{
   return config_get_int_value(memoria_config, "TAM_MEMORIA");
}

u_int32_t get_tamanio_pagina(void)
{
   return config_get_int_value(memoria_config, "TAM_PAGINA");
}

char *get_path_instrucciones(void)
{
   return config_get_string_value(memoria_config, "PATH_INSTRUCCIONES");
}

u_int32_t get_retardo_respuesta(void)
{
   return config_get_int_value(memoria_config, "RETARDO_RESPUESTA");
}
