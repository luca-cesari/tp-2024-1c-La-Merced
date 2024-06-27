#ifndef IO_CONFIG_H
#define IO_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>

typedef enum
{
   GENERICA,
   STDIN,
   STDOUT,
   DIALFS
} tipo_interfaz;

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

void iniciar_config(char *);
tipo_interfaz get_tipo_interfaz(void);
kernel_config get_kernel_config(void);
mem_config get_memoria_config(void);
u_int32_t get_tiempo_unidad_trabajo(void);
char *get_path_base_dialfs(void);
u_int32_t get_block_size(void);
u_int32_t get_block_count(void);
u_int32_t get_retraso_compactacion(void);
void destruir_config(void);

#endif // IO_CONFIG_H