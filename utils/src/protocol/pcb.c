#include "pcb.h"

// revisar los valores iniciales
t_pcb *crear_pcb(char *ejecutable)
{
   t_pcb *pcb = malloc(sizeof(t_pcb));
   pcb->pid = 0; // hay q ver que onda con esto, tiene q ser autoincremental en todo el sistema
   pcb->program_counter = 0;
   pcb->quantum = 0;
   pcb->cpu_registers = crear_registros();
   pcb->psw = bitarray_create_with_mode(NULL, 1, LSB_FIRST);
   pcb->executable_path = strdup(ejecutable);

   return pcb;
}

t_packet *serializar_pcb(t_pcb *pcb)
{
   t_packet *paquete = crear_paquete();

   agregar_a_paquete(paquete, &(pcb->pid), sizeof(u_int32_t));
   agregar_a_paquete(paquete, &(pcb->program_counter), sizeof(int32_t));
   agregar_a_paquete(paquete, &(pcb->quantum), sizeof(u_int32_t));

   // capaz es mejor usar un serializar_registers para este
   agregar_a_paquete(paquete, &(pcb->cpu_registers), sizeof(t_registers));

   agregar_a_paquete(paquete, pcb->psw->bitarray, pcb->psw->size);
   agregar_a_paquete(paquete, &(pcb->psw->size), sizeof(size_t));
   agregar_a_paquete(paquete, &(pcb->psw->mode), sizeof(bit_numbering_t));
   agregar_a_paquete(paquete, pcb->executable_path, strlen(pcb->executable_path) + 1);

   return paquete;
}

void enviar_pcb(int32_t fd_conexion, t_pcb *pcb)
{
   t_packet *paquete = serializar_pcb(pcb);
   enviar_paquete(paquete, fd_conexion);
   eliminar_paquete(paquete);
}

t_pcb *recibir_pcb(int32_t fd_conexion)
{
   t_list *paquete = recibir_paquete(fd_conexion);
   t_pcb *pcb = malloc(sizeof(t_pcb));

   pcb->pid = *(u_int32_t *)list_get(paquete, 0);
   pcb->program_counter = *(int32_t *)list_get(paquete, 1);
   pcb->quantum = *(u_int32_t *)list_get(paquete, 2);

   pcb->cpu_registers = *(t_registers *)list_get(paquete, 3);
   pcb->psw = bitarray_create_with_mode(list_get(paquete, 4), *(size_t *)list_get(paquete, 5), *(bit_numbering_t *)list_get(paquete, 6));
   pcb->executable_path = strdup(list_get(paquete, 7));

   return pcb;
}

void destruir_pcb(t_pcb *pcb)
{
}