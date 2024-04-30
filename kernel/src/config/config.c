#include "config.h"

t_config *kernel_config;

void iniciar_config()
{
   kernel_config = config_create(CONFIG_FILE);
}

char *get_puerto_escucha()
{
   return config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
}

struct cpu_config get_cpu_config()
{
   struct cpu_config cpu_cfg;

   cpu_cfg.ip = config_get_string_value(kernel_config, "IP_CPU");
   cpu_cfg.puerto_dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
   cpu_cfg.puerto_interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");

   return cpu_cfg;
}

struct mem_config get_memoria_config()
{
   struct mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(kernel_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(kernel_config, "PUERTO_MEMORIA");

   return mem_cfg;
}

void destruir_config()
{
   config_destroy(kernel_config);
}