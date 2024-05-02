#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/config.h>
#include <commons/collections/dictionary.h>

#define CONFIG_FILE "kernel.config"

typedef struct
{
   char *ip;
   char *puerto_dispatch;
   char *puerto_interrupt;
} cpu_config;

typedef struct
{
   char *ip;
   char *puerto;
} mem_config;

typedef enum
{
   FIFO,
   RR,
   VRR
} algoritmo_planificacion;

void iniciar_config(void);
char *get_puerto_escucha(void);
cpu_config get_cpu_config(void);
mem_config get_memoria_config(void);
algoritmo_planificacion get_algoritmo_planificacion(void);
u_int32_t get_quantum(void);
t_dictionary *get_recursos(void);
u_int32_t get_grado_multiprogramacion(void);
void destruir_config(void);

#endif // KERNEL_CONFIG_H