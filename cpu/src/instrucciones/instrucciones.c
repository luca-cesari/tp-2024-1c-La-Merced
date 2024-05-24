#include "instrucciones.h"


void set(Parametros parametros)
{
   if (parametros.parametro1.tipo_dato == INT32)
   {
      *(parametros.parametro1.dato.registro_u32) = parametros.parametro2.dato.valor;
   }
   else
   {
      *(parametros.parametro1.dato.registro_u8) = parametros.parametro2.dato.valor;
   }
}

void sum(Parametros parametros)
{
   if (parametros.parametro1.tipo_dato == INT32)
   {
      *(parametros.parametro1.dato.registro_u32) += *(parametros.parametro2.dato.registro_u32);
   }
   else
   {
      *(parametros.parametro1.dato.registro_u8) += *(parametros.parametro2.dato.registro_u8);
   }
}

void sub(Parametros parametros)
{
   if (parametros.parametro1.tipo_dato == INT32)
   {
      *(parametros.parametro1.dato.registro_u32) -= *(parametros.parametro2.dato.registro_u32);
   }
   else
   {
      *(parametros.parametro1.dato.registro_u8) -= *(parametros.parametro2.dato.registro_u8);
   }
}

void jnz(Parametros parametros)
{

   if (parametros.parametro1.tipo_dato == INT32)
   {
      if (*(parametros.parametro1.dato.registro_u32) != 0)
      {
         //Esto descomentarlo cuando tengamos registros_cpu en este archivo
         //registros_cpu.PC = parametros.parametro2.dato.valor;
      }
   }
   else
   {
      if (*(parametros.parametro1.dato.registro_u8) != 0)
      {
         //Esto descomentarlo cuando tengamos registros_cpu en este archivo
         //registros_cpu.PC = parametros.parametro2.dato.valor;
      }
   }
}

void io_gen_sleep(Parametros parametros)
{

   //Esto hay que descomentarlo una vez que se le pase el PCB por parámetro
   //t_io_request *io_request = crear_io_request(pcb.pid, parametros.parametro1.dato.interfaz, "IO_GEN_SLEEP", string_itoa(parametros.parametro2.dato.valor)); // ver implementacion gen_sleep en entrada/salida
}
t_pcb actualizar_pcb_io_request(t_pcb *pcb, t_io_request *io_request) // VER SI VA ACA
{
   //Esto descomentarlo cuando tengamos registros_cpu en este archivo
   //pcb->program_counter = registros_cpu.PC;
   //pcb->cpu_registers = registros_cpu.registers_generales;
   //pcb->io_request = io_request;
}
