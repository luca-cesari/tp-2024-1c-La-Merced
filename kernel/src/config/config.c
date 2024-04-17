#include "config.h"

t_config *iniciar_config(void)
{
   return config_create(CONFIG_FILE);
}

char *get_puerto_escucha(t_config *config)
{
   return config_get_string_value(config, "PUERTO_ESCUCHA");
}

struct CPU get_cpu_config(t_config *config)
{
   struct CPU cpu_config;

   cpu_config.ip = config_get_string_value(config, "IP_CPU");
   cpu_config.puerto_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
   cpu_config.puerto_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");

   return cpu_config;
}

struct MEM get_memoria_config(t_config *config)
{
   struct MEM mem_config;

   mem_config.ip = config_get_string_value(config, "IP_MEMORIA");
   mem_config.puerto = config_get_string_value(config, "PUERTO_MEMORIA");

   return mem_config;
}

