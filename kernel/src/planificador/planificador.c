#include "planificador.h"

const t_mutex_queue *cola_new;
const t_mutex_queue *cola_ready;
const t_mutex_queue *cola_exec;
const t_mutex_queue *cola_block;
const t_mutex_queue *cola_exit;

void *crear_proceso(void *ruta_archivo)
{
   char *ejecutable = (char *)ruta_archivo;
   printf("Proceso Creado %s \n", ejecutable);
   return NULL;
}
