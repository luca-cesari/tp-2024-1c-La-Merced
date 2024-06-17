#include "ciclo.h"

t_dictionary *instrucciones;
t_pcb *pcb;

static char *fetch(void);
static void (*decode(char *char_instruccion))(char **);
static void execute(void (*instruccion)(char **), char *char_instruccion);
static int check_interrupt();
static int check_desalojo();

void aumentar_program_counter();

static char *fetch()
{
   parametros parametro;
   parametro.program_counter = pcb->program_counter;
   t_cpu_mem_req *mem_request = crear_cpu_mem_request(FETCH_INSTRUCCION, pcb->pid, parametro);
   enviar_mem_request(mem_request);

   char *instruccion = recibir_instruccion();
   log_fetch_instruccion(pcb->pid, pcb->program_counter);

   destruir_cpu_mem_request(mem_request);
   return instruccion;
}

static void (*decode(char *char_instruccion))(char **)
{
   char **instruc_parametros = string_split(char_instruccion, " ");
   void (*instruccion)(char **) = dictionary_get(instrucciones, instruc_parametros[0]);
   string_array_destroy(instruc_parametros);
   return instruccion;
}

static void execute(void (*instruccion)(char **), char *char_instruccion)
{
   char **instruc_parametros = string_split(char_instruccion, " ");
   char **parametros = eliminar_primer_elemento(instruc_parametros);
   aumentar_program_counter();

   instruccion(parametros);
   log_instruccion_ejecutada(pcb->pid, instruc_parametros[0], array_a_string(parametros));
   // array_a_string es un nuevo string y no se esta liberando en ningun lado

   string_array_destroy(instruc_parametros);
   string_array_destroy(parametros);
}

static int check_interrupt()
{
   switch (get_interrupcion())
   {
   case 0:
      set_motivo_desalojo(pcb, KILL);
      return 1;
   case 1:
      set_motivo_desalojo(pcb, QUANTUM);
      return 1;
   }
   return 0;
}

static int check_desalojo()
{
   // el unico caso false es NONE (0)
   return pcb->motivo_desalojo;
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