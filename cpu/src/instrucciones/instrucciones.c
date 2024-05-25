#include "instrucciones.h"

extern t_pcb *pcb;
int hay_iorequest = 0;
pthread_mutex_t mutex_io_request;

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
         pcb->program_counter = parametros.parametro2.dato.valor;
      }
   }
   else
   {
      if (*(parametros.parametro1.dato.registro_u8) != 0)
      {
         pcb->program_counter = parametros.parametro2.dato.valor;
      }
   }
}

void io_gen_sleep(Parametros parametros)
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros.parametro1.dato.interfaz, "IO_GEN_SLEEP", string_itoa(parametros.parametro2.dato.valor)); // ver implementacion gen_sleep en entrada/salida
   pcb->io_request = io_request;
}

void exit_instruction(Parametros parametros)
{
   pcb->motivo_desalojo = TERMINATED;
}