#ifndef PROCESO_H
#define PROCESO_H

#include <stdlib.h>
#include <commons/process.h>
#include <commons/bitarray.h>

struct t_registro_cpu
{
   u_int8_t AX;
   u_int8_t BX;
   u_int8_t CX;
   u_int8_t DX;
   u_int32_t EAX;
   u_int32_t EBX;
   u_int32_t ECX;
   u_int32_t EDX;
};

// algo que se va a mandar por dispatch
// y hay q serializar
typedef struct
{
   int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   struct t_registro_cpu registros_cpu;
   t_bitarray *psw;
} t_pcb;

t_pcb new_pcb();

#endif // PROCESO_H
