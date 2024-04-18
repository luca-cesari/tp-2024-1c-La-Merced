#include "config.h"

t_config *iniciar_config(void)
{
   return config_create(CONFIG_FILE);
}

char *get_puerto_escucha(t_config *config)
{
   return config_get_string_value(config, "PUERTO_ESCUCHA");
}

struct cpu_config get_cpu_config(t_config *config)
{
   struct cpu_config cpu_cfg;

   cpu_cfg.ip = config_get_string_value(config, "IP_CPU");
   cpu_cfg.puerto_dispatch = config_get_string_value(config, "PUERTO_CPU_DISPATCH");
   cpu_cfg.puerto_interrupt = config_get_string_value(config, "PUERTO_CPU_INTERRUPT");

   return cpu_cfg;
}

struct mem_config get_memoria_config(t_config *config)
{
   struct mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(config, "PUERTO_MEMORIA");

   return mem_cfg;
}

