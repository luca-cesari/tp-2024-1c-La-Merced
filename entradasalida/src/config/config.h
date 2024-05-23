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

// typedef struct
// {
//    u_int32_t tiempo_unidad_trabajo;
// } generica_config;

// typedef struct
// {
// } stdin_config;

// typedef struct
// {
//    u_int32_t tiempo_unidad_trabajo;
// } stdout_config;

typedef struct
{
   u_int32_t tiempo_unidad_trabajo;
   char *path_base_dialfs;
   u_int32_t block_size;
   u_int32_t block_count;
   u_int32_t retraso_compactacion;
} dialfs_config;

void iniciar_config(char *);
tipo_interfaz get_tipo_interfaz(void);
kernel_config get_kernel_config(void);
mem_config get_memoria_config(void);
u_int32_t get_tiempo_unidad_trabajo(void);
// generica_config get_generica_config();
// stdin_config get_stdin_config();
// stdout_config get_stdout_config();
dialfs_config get_dialfs_config();
void destruir_config(void);

#endif // IO_CONFIG_H