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


// Para obtener datos importantes de interfaz generica
generica_config get_generica_config()
{
   generica_config generica;
   generica.tipo_interfaz = config_get_string_value(entradasalida_config, "TIPO_INTERFAZ");
   generica.tiempo_unidad_trabajo = config_get_int_value(entradasalida_config, "TIEMPO_UNIDAD_TRABAJO");
   generica.ip_kernel = config_get_string_value(entradasalida_config, "IP_KERNEL");
   generica.puerto_kernel = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");

   return generica;
}

// Para obtener datos importantes de interfaz stdin
stdin_config get_stdin_config()
{
   stdin_config stdin;
   stdin.tipo_interfaz = config_get_string_value(entradasalida_config, "TIPO_INTERFAZ");
   stdin.tiempo_unidad_trabajo = config_get_int_value(entradasalida_config, "TIEMPO_UNIDAD_TRABAJO");
   stdin.ip_kernel = config_get_string_value(entradasalida_config, "IP_KERNEL");
   stdin.puerto_kernel = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");
   stdin.ip_memoria = config_get_string_value(entradasalida_config, "IP_MEMORIA");
   stdin.puerto_memoria = config_get_string_value(entradasalida_config, "PUERTO_MEMORIA");

   return stdin;
}

// Para obtener datos importantes de interfaz stdout
stdout_config get_stdout_config()
{
   stdout_config stdout;
   stdout.tipo_interfaz = config_get_string_value(entradasalida_config, "TIPO_INTERFAZ");
   stdout.tiempo_unidad_trabajo = config_get_int_value(entradasalida_config, "TIEMPO_UNIDAD_TRABAJO");
   stdout.ip_kernel = config_get_string_value(entradasalida_config, "IP_KERNEL");
   stdout.puerto_kernel = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");
   stdout.ip_memoria = config_get_string_value(entradasalida_config, "IP_MEMORIA");
   stdout.puerto_memoria = config_get_string_value(entradasalida_config, "PUERTO_MEMORIA");

   return stdout;
}

// Para obtener datos importantes de interfaz dialfs
dialfs_config get_dialfs_config()
{
   dialfs_config dialfs;
   dialfs.create = config_get_string_value(entradasalida_config, "IO_FS_CREATE");
   dialfs.delete = config_get_string_value(entradasalida_config, "IO_FS_DELETE");
   dialfs.truncate = config_get_string_value(entradasalida_config, "IO_FS_TRUNCATE");
   dialfs.write = config_get_string_value(entradasalida_config, "IO_FS_WRITE");
   dialfs.read = config_get_string_value(entradasalida_config, "IO_FS_READ");

   return dialfs;
}



//Para conectarme al kernel (ESTO ES SOLO POR AHORA, LUEGO DEPENDERA DEL TIPO DE INTERFAZ)
kernel_config get_kernel_config(void) 
{
   kernel_config kernel_cfg;

   kernel_cfg.ip = config_get_string_value(entradasalida_config, "IP_KERNEL");
   kernel_cfg.puerto = config_get_string_value(entradasalida_config, "PUERTO_KERNEL");

   return kernel_cfg;
}

//Para conectarme a la memoria (ESTO ES SOLO POR AHORA, LUEGO DEPENDERA DEL TIPO DE INTERFAZ)
mem_config get_memoria_config(void)
{
   mem_config mem_cfg;

   mem_cfg.ip = config_get_string_value(entradasalida_config, "IP_MEMORIA");
   mem_cfg.puerto = config_get_string_value(entradasalida_config, "PUERTO_MEMORIA");

   return mem_cfg;
}



void destruir_config(void)
{
   config_destroy(entradasalida_config);
}