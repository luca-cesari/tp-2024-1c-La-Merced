#ifndef UTILS_PROTOCOL_REGISTERS_H
#define UTILS_PROTOCOL_REGISTERS_H

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

// typedef struct // no lo uso por ahora
// {
//    u_int32_t PC;
//    u_int32_t SI;
//    u_int32_t DI;
//    t_registers_generales registers_generales;
// } t_registers;

// Los argumentos de las funciones me los autocompleto copilot
// habria q revisar

// como registros a secas o como puntero?
t_registers crear_registros();

#endif // UTILS_PROTOCOL_REGISTROS_H