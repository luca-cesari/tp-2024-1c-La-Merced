#include "instrucciones.h"

t_dictionary *instrucciones;
extern t_pcb *pcb;

void set(char **parametros) // clean
{
   int32_t valor = atoi(parametros[1]);
   set_valor_registro(parametros[0], valor);
}

void mov_in(char **parametros_recibidos) // clean
{
   u_int32_t direccion_logica = get_valor_registro(parametros_recibidos[1]);
   u_int32_t tamanio_registro_dato = get_tamanio_registro(parametros_recibidos[0]);
   char *direcciones_fisicas = obtener_direcciones_fisicas(direccion_logica, tamanio_registro_dato);

   parametros parametros_leer = crear_parametros_leer(direcciones_fisicas, tamanio_registro_dato);

   t_cpu_mem_req *mem_request = crear_cpu_mem_request(LEER, pcb->pid, parametros_leer);
   enviar_mem_request(mem_request);

   t_mem_buffer_response *response = recibir_buffer_response_de_memoria();
   if (response->resultado == OPERATION_SUCCEED)
   {
      char *direccion_fisica = (string_split(direcciones_fisicas, " "))[0];
      u_int32_t valor_a_guardar = 0;
      if (response->tamanio_buffer == 1)
         valor_a_guardar = *(u_int8_t *)response->buffer;
      else
         valor_a_guardar = *(u_int32_t *)(response->buffer);

      set_valor_registro(parametros_recibidos[0], valor_a_guardar);
      log_escritura_lectura_memoria(pcb->pid, READ, atoi(direccion_fisica), string_itoa(valor_a_guardar));
   }
   else
      perror("Error al leer en memoria\n");

   destruir_buffer_response(response);
   // destruir_cpu_mem_request(mem_request);
}

void mov_out(char **parametros_recibidos) // clean
{
   u_int32_t direccion_logica = get_valor_registro(parametros_recibidos[0]);
   void *dato = get_puntero_registro(parametros_recibidos[1]);
   u_int32_t tamanio_registro_dato = get_tamanio_registro(parametros_recibidos[1]);
   char *direcciones_fisicas = obtener_direcciones_fisicas(direccion_logica, tamanio_registro_dato);

   parametros parametros_escribir = crear_parametros_escribir(direcciones_fisicas, dato, tamanio_registro_dato);
   t_cpu_mem_req *mem_request = crear_cpu_mem_request(ESCRIBIR, pcb->pid, parametros_escribir);

   enviar_mem_request(mem_request);
   if (recibir_response_de_memoria() == OPERATION_SUCCEED)
   {
      char *direccion_fisica = (string_split(direcciones_fisicas, " "))[0];
      int32_t valor_escrito = get_valor_registro(parametros_recibidos[1]);
      log_escritura_lectura_memoria(pcb->pid, WRITE, atoi(direccion_fisica), string_itoa(valor_escrito));
   }
   else
      perror("Error al escribir en memoria\n");

   // destruir_cpu_mem_request(mem_request);
}

void sum(char **parametros) // clean
{
   int32_t valor = get_valor_registro(parametros[0]) + get_valor_registro(parametros[1]);
   set_valor_registro(parametros[0], valor);
}

void sub(char **parametros) // clean
{
   int32_t valor = get_valor_registro(parametros[0]) - get_valor_registro(parametros[1]);
   set_valor_registro(parametros[0], valor);
}

void jnz(char **parametros) // clean
{
   if (get_valor_registro(parametros[0]) != 0)
      pcb->program_counter = atoi(parametros[1]);
}

void resize(char **parametros_char) // clean
{
   parametros parametro;
   parametro.tamanio_nuevo = atoi(parametros_char[0]);
   t_cpu_mem_req *mem_request = crear_cpu_mem_request(RESIZE, pcb->pid, parametro);
   enviar_mem_request(mem_request);

   if (recibir_response_de_memoria() == OPERATION_FAILED)
   {
      set_motivo_desalojo(pcb, ERROR);
      set_motivo_finalizacion(pcb, OUT_OF_MEMORY);
   }

   // destruir_cpu_mem_request(mem_request);
}

void copy_string(char **param)
{
   int tamanio_valor = atoi(param[0]);

   char *direcciones_fisicas_SI = obtener_direcciones_fisicas(pcb->cpu_registers.SI, tamanio_valor);
   char *direccion_fisica_SI_inicial = string_split(direcciones_fisicas_SI, " ")[0];

   parametros parametros_leer = crear_parametros_leer(direcciones_fisicas_SI, tamanio_valor);
   t_cpu_mem_req *mem_request_leer = crear_cpu_mem_request(LEER, pcb->pid, parametros_leer);

   enviar_mem_request(mem_request_leer);

   char *string_escribir;
   t_mem_buffer_response *response = recibir_buffer_response_de_memoria();

   if (response->tamanio_buffer == tamanio_valor)
      string_escribir = response->buffer;
   else
      perror("Error al leer en memoria\n");

   log_escritura_lectura_memoria(pcb->pid, READ, atoi(direccion_fisica_SI_inicial), string_escribir);

   char *direcciones_fisicas_DI = obtener_direcciones_fisicas(pcb->cpu_registers.DI, tamanio_valor);
   char *direccion_fisica_DI_inicial = string_split(direcciones_fisicas_DI, " ")[0];

   parametros parametros_escribir = crear_parametros_escribir(direcciones_fisicas_DI, string_escribir, tamanio_valor);

   t_cpu_mem_req *mem_request_escribir = crear_cpu_mem_request(ESCRIBIR, pcb->pid, parametros_escribir);

   enviar_mem_request(mem_request_escribir);

   if (recibir_response_de_memoria() == OPERATION_SUCCEED)
      log_escritura_lectura_memoria(pcb->pid, WRITE, atoi(direccion_fisica_DI_inicial), string_escribir);
   else
      perror("Error al escribir en memoria\n");

   destruir_buffer_response(response);
}

void io_gen_sleep(char **parametros) // clean
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_GEN_SLEEP", parametros[1]);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}

void io_stdin_read(char **parametros) // clean
{
   char *direcciones_tamanio = get_direccion_tamanio(eliminar_primer_elemento(parametros));

   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDIN_READ", direcciones_tamanio);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}

void io_stdout_write(char **parametros) // clean
{
   char *direcciones_tamanio = get_direccion_tamanio(eliminar_primer_elemento(parametros));

   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDOUT_WRITE", direcciones_tamanio);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}
void io_fs_create(char **parametros) // clean
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_FS_CREATE", parametros[1]);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}
void io_fs_delete(char **parametros) // clean
{

   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_FS_DELETE", parametros[1]);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}

void io_fs_truncate(char **parametros) // clean
{
   char *parametros_nuevos = array_a_string(eliminar_primer_elemento(parametros));
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_FS_TRUNCATE", parametros_nuevos);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);

   // destruir_io_request(io_request);
}

void exit_instruction(char **parametros)
{
   set_motivo_desalojo(pcb, TERMINATED);
}

void inicializar_instrucciones(void)
{
   instrucciones = dictionary_create();
   inicializar_registros();

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
   dictionary_put(instrucciones, "IO_FS_CREATE", &io_fs_create);
   dictionary_put(instrucciones, "IO_FS_DELETE", &io_fs_delete);
   dictionary_put(instrucciones, "IO_FS_TRUNCATE", &io_fs_truncate);

   dictionary_put(instrucciones, "EXIT", &exit_instruction);
}

void (*get_instruccion(char *instruccion))(char **)
{
   return dictionary_get(instrucciones, instruccion);
}

// TODO refactorizar
char *obtener_direcciones_fisicas(u_int32_t direccion_logica, u_int32_t tamanio_dato)
{
   int32_t tamanio_pagina = get_tamanio_pagina();
   int32_t pagina_inicial = direccion_logica / tamanio_pagina;
   int32_t tamanio_dato_aux = tamanio_dato;
   int32_t direccion_logica_aux = (int32_t)direccion_logica;

   int32_t pagina_final = (direccion_logica_aux + tamanio_dato_aux - 1) / tamanio_pagina;

   pagina_final = pagina_final < 0 ? 0 : pagina_final;

   char *direcciones_fisicas = string_new();
   char *direccion_fisica_actual_str = string_itoa(get_direccion_fisica(pcb->pid, direccion_logica));
   string_append(&direcciones_fisicas, direccion_fisica_actual_str);

   for (u_int32_t pagina = pagina_inicial; pagina < pagina_final; pagina++) // Recorre las páginas necesarias para leer el registro
   {
      u_int32_t pagina_siguiente = pagina + 1;
      direccion_fisica_actual_str = string_itoa(get_direccion_fisica(pcb->pid, pagina_siguiente * tamanio_pagina));
      string_append(&direcciones_fisicas, " ");
      string_append(&direcciones_fisicas, direccion_fisica_actual_str);
   }

   return direcciones_fisicas;
}

char *get_direccion_tamanio(char **parametros)
{
   int32_t valor_registro_dato = get_valor_registro(parametros[1]);
   char *direcciones_fisicas = obtener_direcciones_fisicas(get_valor_registro(parametros[0]), valor_registro_dato);
   char *direcciones_tamanio = string_new();

   string_append(&direcciones_tamanio, direcciones_fisicas);
   string_append(&direcciones_tamanio, " ");
   string_append(&direcciones_tamanio, string_itoa(valor_registro_dato));

   printf("DIRECCIONES Y TAMANIO: %s\n", direcciones_tamanio);

   return direcciones_tamanio;
}

parametros crear_parametros_leer(char *direccion_fisica, u_int32_t tamanio_valor)
{
   parametros parametros_leer;
   parametros_leer.param_leer.direcciones_fisicas = direccion_fisica;
   parametros_leer.param_leer.tamanio_buffer = tamanio_valor;
   return parametros_leer;
}

parametros crear_parametros_escribir(char *direccion_fisica, void *buffer, u_int32_t tamanio_valor)
{
   parametros parametros_escribir;
   parametros_escribir.param_escribir.direcciones_fisicas = direccion_fisica;
   parametros_escribir.param_escribir.tamanio_buffer = tamanio_valor;
   parametros_escribir.param_escribir.buffer = buffer;
   return parametros_escribir;
}
