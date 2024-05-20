#ifndef CPU_CONFIG_H
#define CPU_CONFIG_H

#include <stdlib.h>
#include <string.h>
#include <commons/cpu.h>
#include <commons/collections/dictionary.h>

#define CONFIG_FILE "cpu.config"

typedef struct
{
   char *puerto_dispatch;
   char *puerto_interrupt;
} kernel_config;

typedef struct
{
   char *ip;
   char *puerto;
} mem_config;

typedef enum
{
   FIFO,
   LRU
} algoritmo_tlb;

void iniciar_config(void);
mem_config get_memoria_config(void);
kernel_config get_kernel_config(void);
u_int32_t get_cant_entradas_tlb(void);
algoritmo_tlb get_algoritmo_tlb(void);
void destruir_config(void);

#endif // CPU_CONFIG_H