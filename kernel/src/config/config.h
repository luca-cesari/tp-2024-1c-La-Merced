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

t_config *iniciar_config(void);
char *get_puerto_escucha(t_config *config);
struct cpu_config get_cpu_config(t_config *config);
struct mem_config get_memoria_config(t_config *config);