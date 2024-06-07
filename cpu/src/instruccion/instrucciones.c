#include "instrucciones.h"

t_dictionary *registros;
extern t_pcb *pcb;



void set(char **parametros)
{
   int valor = atoi(parametros[1]);
   if (string_starts_with(parametros[0], "E"))
   {
      u_int32_t *registro = dictionary_get(registros, parametros[0]);
      *registro = valor;
   }
   else
   {
      u_int8_t *registro = dictionary_get(registros, parametros[0]);
      *registro = valor;
   }
}

void sum(char **parametros)
{
   if (string_starts_with(parametros[0], "E"))
   {
      u_int32_t *registro1 = dictionary_get(registros, parametros[0]);
      u_int32_t *registro2 = dictionary_get(registros, parametros[1]);
      *registro1 += *registro2;
   }
   else
   {
      u_int8_t *registro = dictionary_get(registros, parametros[0]);
      u_int8_t *registro2 = dictionary_get(registros, parametros[1]);
      *registro += *registro2;
   }
}

void sub(char **parametros)
{
   if (string_starts_with(parametros[0], "E"))
   {
      u_int32_t *registro = dictionary_get(registros, parametros[0]);
      u_int32_t *registro2 = dictionary_get(registros, parametros[1]);
      *registro -= *registro2;
   }
   else
   {
      u_int8_t *registro = dictionary_get(registros, parametros[0]);
      u_int8_t *registro2 = dictionary_get(registros, parametros[1]);
      *registro -= *registro2;
   }
}

void jnz(char **parametros)
{
   if (dictionary_get(registros, parametros[0]) != 0)
   {
      int valor = atoi(parametros[1]);
      pcb->program_counter = valor;
   }
}

void io_gen_sleep(char **parametros)
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_GEN_SLEEP", parametros[1]);
   pcb->io_request = io_request;
}

void io_stdin_read(char **parametros)
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDIN_READ", array_a_string(eliminar_primer_elemento(parametros)));
   pcb->io_request = io_request;
}

void io_stdout_write(char **parametros)
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDOUT_WRITE", array_a_string(eliminar_primer_elemento(parametros)));
   pcb->io_request = io_request;
}

void exit_instruction(char **parametros)
{
   pcb->motivo_desalojo = TERMINATED;
}


//////////////// AUXILIARES ////////////////////
void inicializar_diccionario_registros()
{
   registros = dictionary_create();

   dictionary_put(registros, "AX", &(pcb->cpu_registers.AX));
   dictionary_put(registros, "BX", &(pcb->cpu_registers.BX));
   dictionary_put(registros, "CX", &(pcb->cpu_registers.CX));
   dictionary_put(registros, "DX", &(pcb->cpu_registers.DX));
   dictionary_put(registros, "EAX", &(pcb->cpu_registers.EAX));
   dictionary_put(registros, "EBX", &(pcb->cpu_registers.EBX));
   dictionary_put(registros, "ECX", &(pcb->cpu_registers.ECX));
   dictionary_put(registros, "EDX", &(pcb->cpu_registers.EDX));
}

char *array_a_string(char **array)
{
   char *string = string_new();
   int i = 0;
   while (array[i] != NULL)
   {
      string_append(&string, array[i]);
      i++;
   }
   return string;
}

char **eliminar_primer_elemento(char **array)
{
   int tamano = string_array_size(array);

   // Verificar que el array no esté vacío
   if (tamano > 0)
   {
      // Desplazar todos los punteros una posición hacia la izquierda
      for (int i = 0; i < tamano - 1; i++)
      {
         array[i] = array[i + 1];
      }
      // Establecer el último puntero a NULL para marcar el final del array
      array[tamano - 1] = NULL;
   }
   return array;
}