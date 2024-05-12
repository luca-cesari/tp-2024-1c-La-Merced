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

// Estructura para datos importantes de interfaz generica
typedef struct {
   char* tipo_interfaz;
   int tiempo_unidad_trabajo;
   char* ip_kernel;
   char* puerto_kernel;
} generica_config;

// Estructura para datos importantes de interfaz stdin
typedef struct {
   char* tipo_interfaz;
   int tiempo_unidad_trabajo;
   char* ip_kernel;
   char* puerto_kernel;
   char* ip_memoria;
   char* puerto_memoria;
} stdin_config;

// Estructura para datos importantes de interfaz stdout
typedef struct {
   char* tipo_interfaz;
   int tiempo_unidad_trabajo;
   char* ip_kernel;
   char* puerto_kernel;
   char* ip_memoria;
   char* puerto_memoria;
} stdout_config;

// Estructura para datos importantes de interfaz dialfs
typedef struct {
   char* create;
   char* delete;
   char* truncate;
   char* write;
   char* read;
} dialfs_config;

void iniciar_config(void);
tipo_interfaz get_tipo_interfaz(void);
generica_config get_generica_config();
stdin_config get_stdin_config();
stdout_config get_stdout_config();
dialfs_config get_dialfs_config();
kernel_config get_kernel_config(void);
mem_config get_memoria_config(void);
void destruir_config(void);

#endif // IO_CONFIG_H