#include "config.h"

t_config *entradasalida_config;

void iniciar_config(char *archivo_config)
{
   entradasalida_config = config_create(archivo_config);
}

tipo_interfaz get_tipo_interfaz(void)
{
   char *interfaz = config_get_string_value(entradasalida_config, "TIPO_INTERFAZ");

   if (strcmp(interfaz, "GENERICA") == 0)
      return GENERICA;

   else if (strcmp(interfaz, "STDIN") == 0)
      return STDIN;

   else if (strcmp(interfaz, "STDOUT") == 0)
      return STDOUT;

   else if (strcmp(interfaz, "DIALFS") == 0)
      return DIALFS;

   return -1;
}

kernel_config get_kernel_config(void)
{
   kernel_config kernel_cfg;

   kernel_cfg.ip = config_get_string_value(entradasalida_config, "IP_KERNEL");
   kernel_cfg.puerto = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");

   return kernel_cfg;
}

mem_config get_memoria_config(void)
{
   mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(entradasalida_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(entradasalida_config, "PUERTO_MEMORIA");

   return mem_cfg;
}

u_int32_t get_tiempo_unidad_trabajo(void)
{
   return config_get_int_value(entradasalida_config, "TIEMPO_UNIDAD_TRABAJO");
}

dialfs_config get_dialfs_config()
{
   dialfs_config dialfs_cfg;

   dialfs_cfg.tiempo_unidad_trabajo = get_tiempo_unidad_trabajo();
   dialfs_cfg.path_base_dialfs = config_get_string_value(entradasalida_config, "PATH_BASE_DIALFS");
   dialfs_cfg.block_size = config_get_int_value(entradasalida_config, "BLOCK_SIZE");
   dialfs_cfg.block_count = config_get_int_value(entradasalida_config, "BLOCK_COUNT");
   dialfs_cfg.retraso_compactacion = config_get_int_value(entradasalida_config, "RETRASO_COMPACTACION");

   return dialfs_cfg;
}

void destruir_config(void)
{
   config_destroy(entradasalida_config);
}