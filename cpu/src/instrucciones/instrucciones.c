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
         registros_cpu.PC = parametros.parametro2.dato.valor;
      }
   }
   else
   {
      if (*(parametros.parametro1.dato.registro_u8) != 0)
      {
         registros_cpu.PC = parametros.parametro2.dato.valor;
      }
   }
}

void io_gen_sleep(Parametros parametros) // falta dato pcb que lo consigue de la conexion con kernel
{
   t_io_request *io_request = crear_io_request(pcb.pid, parametros.parametro1.dato.interfaz, "IO_GEN_SLEEP", parametros.parametro2.dato.valor); // ver implementacion gen_sleep en entrada/salida
   actualizar_pcb_io_request(pcb, io_request);
   enviar_pcb(fd_dispatch, pcb);
}

t_pcb actualizar_pcb_io_request(t_pcb *pcb, t_io_request *io_request) // VER SI VA ACA
{
   pcb->program_counter = registros_cpu.PC;
   pcb->cpu_registers = registros_cpu.registers_generales;
   pcb->io_request = io_request;
}
