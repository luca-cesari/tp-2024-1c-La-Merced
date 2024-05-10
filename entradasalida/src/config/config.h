#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>

#define CONFIG_FILE "entradasalida.config"

typedef struct
{
   char *ip;
   char *puerto;
} kernel_config;

typedef struct
{
   char *ip;
   char *puerto;
} mem_config;

typedef enum
{
   GENERICA,
   STDIN,
   STDOUT,
   DIALFS
} tipo_interfaz;

void iniciar_config(void);
tipo_interfaz get_tipo_interfaz(void);
kernel_config get_kernel_config(void);
mem_config get_memoria_config(void);
void destruir_config(void);

#endif // IO_CONFIG_H