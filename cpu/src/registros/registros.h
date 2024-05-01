#ifndef REGISTROS_H
#define REGISTROS_H
#include <stdlib.h>

struct t_registro_cpu
{
   u_int32_t PC;
   u_int8_t AX;
   u_int8_t BX;
   u_int8_t CX;
   u_int8_t DX;
   u_int32_t EAX;
   u_int32_t EBX;
   u_int32_t ECX;
   u_int32_t EDX;
   u_int32_t SI;
   u_int32_t DI;
};
//ver si PC y compania van aca o en otro lado


#endif // REGISTROS_H