#ifndef UTILS_PROTOCOL_PCB_H
#define UTILS_PROTOCOL_PCB_H

#include <stdlib.h>
#include <string.h>
#include <commons/bitarray.h>

#include "protocol.h"
#include "registers.h"

typedef struct
{
   u_int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   t_registers cpu_registers;
   t_bitarray *psw;
   char *executable_path;
} t_pcb;

t_pcb *crear_pcb(char *ejecutable);
t_packet *serializar_pcb(t_pcb *pcb);
void enviar_pcb(int32_t fd_conexion, t_pcb *pcb);
t_pcb *recibir_pcb(int32_t fd_conexion);

// me lo recomendo copilot, ni idea si hace falta, creo q si
void destruir_pcb(t_pcb *pcb);

#endif // UTILS_PROTOCOL_PCB_H
