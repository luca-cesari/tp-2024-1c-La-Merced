#include "config.h"

t_config *cpu_config;

void iniciar_config(void)
{
   cpu_config = config_create(CONFIG_FILE);
}

mem_config get_memoria_config(void)
{
   mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(cpu_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(cpu_config, "PUERTO_MEMORIA");

   return mem_cfg;
}
kernel_config get_kernel_config(void)
{
   kernel_config kernel_cfg;

   kernel_cfg.puerto_dispatch = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_DISPATCH");
   kernel_cfg.puerto_interrupt = config_get_string_value(cpu_config, "PUERTO_ESCUCHA_INTERRUPT");

   return kernel_cfg;
}

u_int32_t get_cant_entradas_tlb(void)
{
   return config_get_int_value(cpu_config, "CANTIDAD_ENTRADAS_TLB");
}

algoritmo_tlb get_algoritmo_tlb(void)
{
   char *algoritmo = config_get_string_value(cpu_config, "ALGORITMO_TLB");

   if (strcmp(algoritmo, "FIFO") == 0)
      return FIFO;

   if (strcmp(algoritmo, "LRU") == 0)
      return RR;

   return -1;
}

void destruir_config(void)
{
   config_destroy(cpu_config);
}