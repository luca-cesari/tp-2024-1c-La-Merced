#include "registers.h"

t_registers_generales *crear_registros()
{
   t_registers_generales *registros = malloc(sizeof(t_registers_generales);


   registros->AX = 0;
   registros->BX = 0;
   registros->CX = 0;
   registros->DX = 0;
   registros->EAX = 0;
   registros->EBX = 0;
   registros->ECX = 0;
   registros->EBX = 0;

   return registros;
}

void destruir_registers_generales(t_registers_generales *registros)
{
   free(registros);
}