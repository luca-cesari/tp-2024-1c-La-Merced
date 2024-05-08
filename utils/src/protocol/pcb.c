#include "pcb.h"

t_pcb *pcb_create(char *executable_path)
{
   t_pcb *pcb = malloc(sizeof(t_pcb));
   return pcb;
}

void pcb_send(int32_t fd_conexion, t_pcb *pcb)
{
   return;
}

t_pcb *pcb_recv(int32_t fd_conexion)
{
   t_pcb *pcb = malloc(sizeof(t_pcb));
   return pcb;
}

void pcb_destroy(t_pcb *pcb)
{
}