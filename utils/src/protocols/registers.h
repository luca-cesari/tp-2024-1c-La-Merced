#ifndef UTILS_PROTOCOLS_REGISTERS_H
#define UTILS_PROTOCOLS_REGISTERS_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h> // para el size_t, si despues no lo usamos lo sacamos

typedef struct
{
   u_int8_t AX;
   u_int8_t BX;
   u_int8_t CX;
   u_int8_t DX;
   u_int32_t EAX;
   u_int32_t EBX;
   u_int32_t ECX;
   u_int32_t EDX;
} t_registers;

// Los argumentos de las funciones me los autocompleto copilot
// habria q revisar

// como registros a secas o como puntero?
t_registers registers_create();

void registers_serialize(t_registers *registers, void **buffer, size_t *buffer_size);
void registers_unserialize(t_registers *registers, void *buffer, size_t buffer_size);

#endif // UTILS_PROTOCOLS_REGISTROS_H