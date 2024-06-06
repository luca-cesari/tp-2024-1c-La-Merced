#ifndef RECURSOS_H
#define RECURSOS_H

#include <stdlib.h>
#include <semaphore.h>
#include <mlist/mlist.h>
#include <pcb/pcb.h>

#include "config/config.h"
#include "blocked.h"

typedef struct
{
   char *nombre_recurso;
   u_int32_t instancias;
   sem_t sem_instancias;
} t_recurso;

void inicializar_recursos(void);
int32_t consumir_recurso(t_pcb *pcb, char *nombre_recurso);
int32_t liberar_recurso(t_pcb *pcb, char *nombre_recurso);
void destruir_recursos(void);

#endif // RECURSOS_H
