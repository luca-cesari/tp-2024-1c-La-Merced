#include "config.h"

t_config *entradasalida_config;


void iniciar_config(void)
{
   entradasalida_config = config_create(CONFIG_FILE);
}

//Para conocer tipo de interfaz
tipo_interfaz get_tipo_interfaz(void)
{
   char* interfaz = config_get_string_value(entradasalida_config, "TIPO_INTERFAZ");

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

//Aclaración: Lo hago con diccionarios mas que nada porque cada interfaz puede devolver una estructura distinta
//Para obtener datos importantes de configuración según tipo de interfaz
t_dictionary get_config_segun_interfaz(tipo_interfaz)
{
   switch (tipo_interfaz)
   {
   case GENERICA:
      return get_generica_config();
   case STDIN:
      /* TODO */
      break;
   case STDOUT:
      /* TODO */
      break;
   case DIALFS:
      /* TODO */
      break;
   
   default:
      return -1;
   }
}



//Para obtener datos importantes de interfaz generica
t_dictionary get_generica_config()
{
   t_dictionary * generica = dictionary_create();
   dictionary_put(generica, "tipo_interfaz", config_get_string_value(entradasalida_config, "TIPO_INTERFAZ"));
   dictionary_put(generica, "tiempo_unidad_trabajo", config_get_string_value(entradasalida_config, "TIEMPO_UNIDAD_TRABAJO"));
   dictionary_put(generica, "ip_kernel", config_get_string_value(entradasalida_config, "IP_KERNEL"));
   dictionary_put(generica, "puerto_kernel", config_get_string_value(entradasalida_config, "PUERTO_KERNEL"));


   return generica;
}



//Para conectarme al kernel (ESTO ES SOLO POR AHORA, LUEGO DEPENDERA DEL TIPO DE INTERFAZ)
kernel_config get_kernel_config(void) 
{
   kernel_config kernel_cfg;

   kernel_cfg.ip = config_get_string_value(entradasalida_config, "IP_KERNEL");
   kernel_cfg.puerto_dispatch = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");

   return kernel_cfg;
}

//Para conectarme a la memoria (ESTO ES SOLO POR AHORA, LUEGO DEPENDERA DEL TIPO DE INTERFAZ)
mem_config get_memoria_config(void)
{
   mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(kernel_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(kernel_config, "PUERTO_MEMORIA");

   return mem_cfg;
}



void destruir_config(void)
{
   config_destroy(kernel_config);
}