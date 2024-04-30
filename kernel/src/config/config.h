#ifndef KERNEL_CONFIG_H
#define KERNEL_CONFIG_H

#include <commons/config.h>

#define CONFIG_FILE "kernel.config"

struct cpu_config
{
   char *ip;
   char *puerto_dispatch;
   char *puerto_interrupt;
};

struct mem_config
{
   char *ip;
   char *puerto;
};

void iniciar_config(void);
char *get_puerto_escucha(void);
struct cpu_config get_cpu_config(void);
struct mem_config get_memoria_config(void);
void destruir_config(void);

#endif // KERNEL_CONFIG_H