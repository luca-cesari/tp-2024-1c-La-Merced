#include <commons/config.h>

#define CONFIG_FILE "kernel.config"

struct CPU
{
   char *ip;
   char *puerto_dispatch;
   char *puerto_interrupt;
};

struct MEM
{
   char *ip;
   char *puerto;
};

t_config *iniciar_config(void);
char *get_puerto_escucha(t_config *config);
struct CPU get_cpu_config(t_config *config);
struct MEM get_memoria_config(t_config *config);