#include "ciclo.h"

t_dictionary *instrucciones;
t_dictionary *registros;
t_pcb *pcb;

char *fetch()
{
   enviar_pcb_memoria(pcb);
   char *instruccion = recibir_instruccion();
   log_fetch_instruccion(pcb->pid, pcb->program_counter);

   return instruccion;
}

void (*decode(char *char_instruccion))(Parametros)
{
   char **instruc_parametros = instruccion_parametros(char_instruccion);

   if (dictionary_has_key(instrucciones, instruc_parametros[0]))
   {
      return dictionary_get(instrucciones, instruc_parametros[0]);
   }
   else
   {
      // En caso de instrucción desconocida
      printf("Instrucción desconocida: %s\n", instruc_parametros[0]);
      return NULL;
   }
}

Parametros obtener_parametros(char **parametros)
{
   // RECIBE UN ARRAY CON LA INTRUCCION EN LA PRIMERA POSICION, POR LO QUE HAY QUE COMENZAR DESDE 1
   Parametros struct_parametros;
   struct_parametros.parametro1 = buscar_operando(parametros[1]);
   if (parametros[1] != NULL) // Esto hay que arreglarlo, sirve solamente para el TERMINATED del segundo checkpoint
   {
      struct_parametros.parametro2 = buscar_operando(parametros[2]);
   }
   // struct_parametros.parametro3 = buscar_operando(parametros[3]) // siguientes checkpoints
   // struct_parametros.parametro4 = buscar_operando(parametros[4])
   return struct_parametros;
}

Parametro buscar_operando(char *parametro)
{
   Parametro operando;
   if (parametro == NULL)
   {
      operando.tipo_dato = NINGUNO;
      return operando;
   }
   else if (es_numero(parametro))
   {
      operando.tipo_dato = VALOR;
      operando.dato.valor = char_a_numero(parametro);
      return operando;
   }
   else if (dictionary_has_key(registros, parametro))
   {
      if (string_starts_with(parametro, "E"))
      {
         operando.tipo_dato = INT32;
         operando.dato.registro_u32 = dictionary_get(registros, parametro);
         return operando;
      }
      else
      {
         operando.tipo_dato = INT8;
         operando.dato.registro_u8 = dictionary_get(registros, parametro);
         return operando;
      }
   }
   else
   {
      operando.tipo_dato = INTERF;
      operando.dato.interfaz = parametro;
      return operando;
   }
}

char **instruccion_parametros(char *char_instruccion)
{
   return string_split(char_instruccion, " ");
}

void execute(void (*instruccion)(Parametros), char *char_instruccion)
{
   char **instruc_parametros = instruccion_parametros(char_instruccion);
   instruccion(obtener_parametros(instruc_parametros));

   log_instruccion_ejecutada(pcb->pid, instruc_parametros[0], instruc_parametros[1]); // Esto traería problemas con la forma actual de obtener parámetros
   aumentar_program_counter();
}

void aumentar_program_counter() /// VER SI VA  ACA
{
   pcb->program_counter += 1;
}

int check_interrupt()
{
   if (hay_interrupcion())
   {
      pcb->motivo_desalojo = QUANTUM;
      return 1;
   }
   return 0;
}

int check_desalojo()
{
   if (pcb->io_request != NULL)
   {
      pcb->motivo_desalojo = IO;
      return 1;
   }
   if (pcb->motivo_desalojo == TERMINATED)
   {
      return 1;
   }

   return 0;
}

void *ciclo_instruccion(t_pcb *pcb_kernel)
{
   pcb = pcb_kernel;

   while (1)
   {
      char *char_instruccion = fetch();

      void (*instruccion)(Parametros) = decode(char_instruccion);

      execute(instruccion, char_instruccion);

      if (check_desalojo())
      {
         return NULL;
      } // si ocurren simultaneamente pesa mas I/O

      if (check_interrupt())
      {
         return NULL;
      }
   }
}

///////// ESTRUCTURAS AUXILIARES ///////////

void set_diccionario_instrucciones(t_dictionary *instrucciones)
{
   dictionary_put(instrucciones, "SET", &set);
   dictionary_put(instrucciones, "SUM", &sum);
   dictionary_put(instrucciones, "SUB", &sub);
   dictionary_put(instrucciones, "JNZ", &jnz);
   dictionary_put(instrucciones, "IO_GEN_SLEEP", &io_gen_sleep);
   dictionary_put(instrucciones, "EXIT", &exit_instruction);
}

void set_diccionario_registros(t_dictionary *registros)
{
   dictionary_put(registros, "AX", &(pcb->cpu_registers.AX));
   dictionary_put(registros, "BX", &(pcb->cpu_registers.BX));
   dictionary_put(registros, "CX", &(pcb->cpu_registers.CX));
   dictionary_put(registros, "DX", &(pcb->cpu_registers.DX));
   dictionary_put(registros, "EAX", &(pcb->cpu_registers.EAX));
   dictionary_put(registros, "EBX", &(pcb->cpu_registers.EBX));
   dictionary_put(registros, "ECX", &(pcb->cpu_registers.ECX));
   dictionary_put(registros, "EDX", &(pcb->cpu_registers.EDX));
}

//////////FUNCIONES AUXILIARES////////////

int es_numero(char *parametro)
{
   while (*parametro)
   {
      if (*parametro < '0' || *parametro > '9')
      {
         return 0; // tiene un caracter no numerico
      }
      parametro++;
   }
   return 1; // es un numero
}

int char_a_numero(char *parametro)
{
   return atoi(parametro);
}

void inicializar_diccionarios_inst_reg()
{
   instrucciones = dictionary_create();
   registros = dictionary_create();

   set_diccionario_instrucciones(instrucciones);
   set_diccionario_registros(registros);
}