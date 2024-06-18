#include "registros.h"

t_dictionary *registros;

void inicializar_registros()
{
   registros = dictionary_create();
}

void set_registros(t_pcb *pcb)
{
   dictionary_put(registros, "AX", &(pcb->cpu_registers.AX));
   dictionary_put(registros, "BX", &(pcb->cpu_registers.BX));
   dictionary_put(registros, "CX", &(pcb->cpu_registers.CX));
   dictionary_put(registros, "DX", &(pcb->cpu_registers.DX));
   dictionary_put(registros, "EAX", &(pcb->cpu_registers.EAX));
   dictionary_put(registros, "EBX", &(pcb->cpu_registers.EBX));
   dictionary_put(registros, "ECX", &(pcb->cpu_registers.ECX));
   dictionary_put(registros, "EDX", &(pcb->cpu_registers.EDX));
   dictionary_put(registros, "SI", &(pcb->cpu_registers.SI));
   dictionary_put(registros, "DI", &(pcb->cpu_registers.DI));
   dictionary_put(registros, "PC", &(pcb->program_counter));
}

int32_t get_valor_registro(char *registro)
{
   u_int8_t tamanio = get_tamanio_registro(registro);
   switch (tamanio)
   {
   case 1:
      return get_valor_registro_8bit(registro);
   case 4:
      return get_valor_registro_32bit(registro);
   }
   return -1;
}

u_int8_t get_valor_registro_8bit(char *registro)
{
   u_int8_t valor = *(u_int8_t *)dictionary_get(registros, registro);
   return valor;
}

u_int32_t get_valor_registro_32bit(char *registro)
{
   u_int32_t valor = *(u_int32_t *)dictionary_get(registros, registro);
   return valor;
}

void set_valor_registro(char *registro, u_int32_t valor)
{
   u_int8_t tamanio = get_tamanio_registro(registro);
   switch (tamanio)
   {
   case 1:
      set_valor_registro_8bit(registro, valor);
      break;
   case 4:
      set_valor_registro_32bit(registro, valor);
      break;
   }
}

void set_valor_registro_8bit(char *registro, u_int8_t valor)
{
   u_int8_t *registro_8bit = (u_int8_t *)dictionary_get(registros, registro);
   *registro_8bit = valor;
}

void set_valor_registro_32bit(char *registro, u_int32_t valor)
{
   u_int32_t *registro_32bit = (u_int32_t *)dictionary_get(registros, registro);
   *registro_32bit = valor;
}

u_int8_t get_tamanio_registro(char *registro)
{
   return string_starts_with(registro, "E") ? 4 : 1;
}