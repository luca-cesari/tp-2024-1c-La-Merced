#include "ciclo.h"

t_dictionary *instrucciones;
t_pcb *pcb;

char *fetch()
{
   parametros parametro;
   parametro.program_counter = pcb->program_counter;
   t_cpu_mem_req *mem_request = crear_cpu_mem_request(FETCH_INSTRUCCION, pcb->pid, parametro);
   enviar_mem_request(mem_request);

   char *instruccion = recibir_instruccion();
   log_fetch_instruccion(pcb->pid, pcb->program_counter);

   return instruccion;
}

void (*decode(char *char_instruccion))(char **)
{
   char **instruc_parametros = string_split(char_instruccion, " ");
   return dictionary_get(instrucciones, instruc_parametros[0]);
}

void execute(void (*instruccion)(char **), char *char_instruccion)
{
   char **instruc_parametros = string_split(char_instruccion, " ");
   char **parametros = eliminar_primer_elemento(instruc_parametros);
   aumentar_program_counter();

   log_instruccion_ejecutada(pcb->pid, instruc_parametros[0], array_a_string(parametros));
   instruccion(parametros);
}

int check_interrupt()
{
   switch (get_interrupcion())
   {
   case 0:
      pcb->motivo_desalojo = KILL;
      return 1;
   case 1:
      pcb->motivo_desalojo = QUANTUM;
      return 1;
   }
   return 0;
}

int check_desalojo()
{
   return pcb->motivo_desalojo; // el unico caso false es NONE (0)
}

void *ciclo_instruccion(t_pcb *pcb_kernel)
{
   pcb = pcb_kernel;
   inicializar_diccionario_registros();

   while (1)
   {
      char *char_instruccion = fetch();

      void (*instruccion)(char **) = decode(char_instruccion);

      execute(instruccion, char_instruccion);

      // si ocurren simultaneamente pesa mas I/O
      if (check_desalojo())
         return NULL;

      if (check_interrupt())
         return NULL;
   }
}

//////////FUNCIONES AUXILIARES////////////

void inicializar_diccionario_instrucciones()
{
   instrucciones = dictionary_create();

   dictionary_put(instrucciones, "SET", &set);
   dictionary_put(instrucciones, "SUM", &sum);
   dictionary_put(instrucciones, "SUB", &sub);
   dictionary_put(instrucciones, "JNZ", &jnz);
   dictionary_put(instrucciones, "RESIZE", &resize);
   dictionary_put(instrucciones, "MOV_IN", &mov_in);
   dictionary_put(instrucciones, "MOV_OUT", &mov_out);
   dictionary_put(instrucciones, "COPY_STRING", &copy_string);
   dictionary_put(instrucciones, "IO_GEN_SLEEP", &io_gen_sleep);
   dictionary_put(instrucciones, "IO_STDIN_READ", &io_stdin_read);
   dictionary_put(instrucciones, "IO_STDOUT_WRITE", &io_stdout_write);
   dictionary_put(instrucciones, "EXIT", &exit_instruction);
}

void aumentar_program_counter() /// VER SI VA  ACA
{
   pcb->program_counter += 1;
}