#include "config.h"

t_config *kernel_config;

void iniciar_config(void)
{
   kernel_config = config_create(CONFIG_FILE);
}

char *get_puerto_escucha(void)
{
   return config_get_string_value(kernel_config, "PUERTO_ESCUCHA");
}

cpu_config get_cpu_config(void)
{
   cpu_config cpu_cfg;

   cpu_cfg.ip = config_get_string_value(kernel_config, "IP_CPU");
   cpu_cfg.puerto_dispatch = config_get_string_value(kernel_config, "PUERTO_CPU_DISPATCH");
   cpu_cfg.puerto_interrupt = config_get_string_value(kernel_config, "PUERTO_CPU_INTERRUPT");

   return cpu_cfg;
}

mem_config get_memoria_config(void)
{
   mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(kernel_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(kernel_config, "PUERTO_MEMORIA");

   return mem_cfg;
}

algoritmo_planificacion get_algoritmo_planificacion(void)
{
   char *algoritmo = config_get_string_value(kernel_config, "ALGORITMO_PLANIFICACION");

   if (strcmp(algoritmo, "FIFO") == 0)
      return FIFO;

   if (strcmp(algoritmo, "RR") == 0)
      return RR;

   if (strcmp(algoritmo, "VRR") == 0)
      return VRR;

   return -1;
}

u_int32_t get_quantum(void)
{
   return config_get_int_value(kernel_config, "QUANTUM");
}

t_dictionary *get_recursos(void)
{
   t_dictionary *recursos = dictionary_create();

   char **nombres = config_get_array_value(kernel_config, "RECURSOS");
   char **instancias = config_get_array_value(kernel_config, "INSTANCIAS_RECURSOS");

   for (int i = 0; nombres[i] != NULL; i++)
   {
      u_int32_t *valor_instancia = malloc(sizeof(u_int32_t));
      *valor_instancia = atoi(instancias[i]);

      dictionary_put(recursos, nombres[i], valor_instancia); // a checkear
   }

   return recursos;
}

u_int32_t get_grado_multiprogramacion(void)
{
   return config_get_int_value(kernel_config, "GRADO_MULTIPROGRAMACION");
}

void destruir_config(void)
{
   config_destroy(kernel_config);
}