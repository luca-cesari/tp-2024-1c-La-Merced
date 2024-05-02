#ifndef UTILS_PROTOCOLS_PCB_H
#define UTILS_PROTOCOLS_PCB_H

#include <stdlib.h>
#include <string.h>
#include <commons/bitarray.h>
#include "registers.h"

typedef struct
{
   u_int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   t_registers cpu_registers;
   t_bitarray *psw;
} t_pcb;

// Los argumentos de las funciones me los autocompleto copilot
// habria q revisar

t_pcb *pcb_create();

void pcb_serialize(t_pcb *pcb, void **buffer, size_t *buffer_size);
void pcb_unserialize(t_pcb *pcb, void *buffer, size_t buffer_size);

void pcb_send(int32_t fd, t_pcb *pcb);
t_pcb *pcb_recv(int32_t fd);

// me lo recomendo copilot, ni idea si hace falta, creo q si
void pcb_destroy(t_pcb *pcb);

#endif // UTILS_PROTOCOLS_PCB_H
