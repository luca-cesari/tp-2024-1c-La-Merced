
#include "instrucciones.h"

t_dictionary *instrucciones;
t_dictionary *registros;
extern t_pcb *pcb;

void inicializar_instrucciones(void)
{
   instrucciones = dictionary_create();
   registros = dictionary_create();

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

void (*get_instruccion(char *instruccion))(char **)
{
   return dictionary_get(instrucciones, instruccion);
}

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

void mov_in(char **parametros_recibidos) //  MOV_IN (Registro Datos, Registro Dirección)
{
   char *primer_parametro = parametros_recibidos[0];  // Registro Datos
   char *segundo_parametro = parametros_recibidos[1]; // Registro Dirección
   char **parametros_aux = string_array_new();
   string_array_push(&parametros_aux, segundo_parametro);
   string_array_push(&parametros_aux, primer_parametro);

   operandos _operandos = obtener_operandos(parametros_aux, REGISTER_SIZE);
   u_int32_t tamanio_registro = obtener_tamanio_registro(parametros_recibidos[0]);

   t_cpu_mem_req *mem_request;
   parametros parametros_leer;

   parametros_leer = crear_parametros_leer(_operandos.direcciones_fisicas, tamanio_registro);
   mem_request = crear_cpu_mem_request(LEER, pcb->pid, parametros_leer);
   enviar_mem_request(mem_request);

   t_mem_buffer_response *response = recibir_buffer_response_de_memoria();
   if (response->tamanio_buffer == tamanio_registro)
   {
      char *direccion_fisica = (string_split(_operandos.direcciones_fisicas, " "))[0];
      if (tamanio_registro == 1)
      {
         *(_operandos.registro_datos._8_bit) = *(u_int8_t *)response->buffer;
         log_escritura_lectura_memoria(pcb->pid, READ, atoi(direccion_fisica), string_itoa(*_operandos.registro_datos._8_bit));
      }
      else
      {
         *(_operandos.registro_datos._32_bit) = *(u_int32_t *)(response->buffer);
         log_escritura_lectura_memoria(pcb->pid, READ, atoi(direccion_fisica), string_itoa(*_operandos.registro_datos._32_bit));
      }
   }
   else
      perror("Error al leer en memoria\n");

   destruir_buffer_response(response);
}

void mov_out(char **parametros_recibidos) //  MOV_OUT (Registro Dirección, Registro Datos)
{
   operandos _operandos = obtener_operandos(parametros_recibidos, REGISTER_SIZE);
   u_int32_t tamanio_registro = obtener_tamanio_registro(parametros_recibidos[1]);

   t_cpu_mem_req *mem_request;
   parametros parametros_escribir;

   void *buffer = malloc(tamanio_registro); // ESTO NO VA, REPENSAR
   if (tamanio_registro == 1)
      memcpy(buffer, _operandos.registro_datos._8_bit, tamanio_registro);
   else
      memcpy(buffer, _operandos.registro_datos._32_bit, tamanio_registro);

   parametros_escribir = crear_parametros_escribir(_operandos.direcciones_fisicas, buffer, tamanio_registro);
   mem_request = crear_cpu_mem_request(ESCRIBIR, pcb->pid, parametros_escribir);

   enviar_mem_request(mem_request);
   if (recibir_response_de_memoria() == OPERATION_SUCCEED)
   {
      char *direccion_fisica = (string_split(_operandos.direcciones_fisicas, " "))[0];
      if (tamanio_registro == 1)
         log_escritura_lectura_memoria(pcb->pid, WRITE, atoi(direccion_fisica), string_itoa(*_operandos.registro_datos._8_bit));
      else
         log_escritura_lectura_memoria(pcb->pid, WRITE, atoi(direccion_fisica), string_itoa(*_operandos.registro_datos._32_bit));
   }
   else
      perror("Error al escribir en memoria\n");
}

void sum(char **parametros)
{
   void *registro1 = dictionary_get(registros, parametros[0]);
   void *registro2 = dictionary_get(registros, parametros[1]);

   if (string_starts_with(parametros[0], "E"))
      *(u_int32_t *)registro1 += *(u_int32_t *)registro2;
   else
      *(u_int8_t *)registro1 += *(u_int8_t *)registro2;
}

void sub(char **parametros)
{
   void *registro1 = dictionary_get(registros, parametros[0]);
   void *registro2 = dictionary_get(registros, parametros[1]);

   if (string_starts_with(parametros[0], "E"))
      *(u_int32_t *)registro1 -= *(u_int32_t *)registro2;
   else
      *(u_int8_t *)registro1 -= *(u_int8_t *)registro2;
}

void jnz(char **parametros)
{
   if (dictionary_get(registros, parametros[0]) != 0)
   {
      int valor = atoi(parametros[1]);
      pcb->program_counter = valor;
   }
}

void resize(char **parametros_char)
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

void io_gen_sleep(char **parametros)
{
   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_GEN_SLEEP", parametros[1]);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);
}

void io_stdin_read(char **parametros)
{
   char *direcciones_tamanio = get_direccion_tamanio(eliminar_primer_elemento(parametros));

   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDIN_READ", direcciones_tamanio);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);
}

void io_stdout_write(char **parametros)
{
   char *direcciones_tamanio = get_direccion_tamanio(eliminar_primer_elemento(parametros));

   t_io_request *io_request = crear_io_request(pcb->pid, parametros[0], "IO_STDOUT_WRITE", direcciones_tamanio);
   set_io_request(pcb, io_request);
   set_motivo_desalojo(pcb, IO);
}

void exit_instruction(char **parametros)
{
   set_motivo_desalojo(pcb, TERMINATED);
}

void set_registros()
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
      direccion_fisica_actual_str = string_itoa(get_direccion_fisica(pcb->pid, pagina_siguiente * tamanio_pagina)); // Esto debería devolver la dirección física de la página nueva que se necesita
      string_append(&direcciones_fisicas, " ");
      string_append(&direcciones_fisicas, direccion_fisica_actual_str);
   }

   return direcciones_fisicas;
}

operandos obtener_operandos(char **parametros, size_flag flag)
{
   operandos operandos;
   u_int32_t direccion_logica = set_direccion_logica(&operandos, parametros[0]);
   u_int32_t tamanio_dato = set_registro_datos(&operandos, parametros[1]);

   if (flag == REGISTER_SIZE)
      tamanio_dato = obtener_tamanio_registro(parametros[1]);

   operandos.direcciones_fisicas = obtener_direcciones_fisicas(direccion_logica, tamanio_dato);
   return operandos;
}

u_int32_t set_direccion_logica(operandos *operandos, char *registro_direccion)
{
   u_int32_t valor = 0;
   if (obtener_tamanio_registro(registro_direccion) == 1)
   {
      operandos->direccion_logica._8_bit = (u_int8_t *)dictionary_get(registros, registro_direccion);
      valor = *(operandos->direccion_logica._8_bit);
   }
   else
   {
      operandos->direccion_logica._32_bit = (u_int32_t *)dictionary_get(registros, registro_direccion);
      valor = *(operandos->direccion_logica._32_bit);
   }
   return valor;
}

u_int32_t set_registro_datos(operandos *operandos, char *registro_datos)
{
   u_int32_t valor = 0;
   if (obtener_tamanio_registro(registro_datos) == 1)
   {
      operandos->registro_datos._8_bit = (u_int8_t *)dictionary_get(registros, registro_datos);
      valor = *(operandos->registro_datos._8_bit);
   }
   else
   {
      operandos->registro_datos._32_bit = (u_int32_t *)dictionary_get(registros, registro_datos);
      valor = *(operandos->registro_datos._32_bit);
   }
   return valor;
}

char *get_direccion_tamanio(char **parametros)
{
   operandos operandos = obtener_operandos(parametros, REGISTER_CONTENT);
   // no es muy limpio hacer doble llama a set_registro_datos
   char *tamanio_valor = string_itoa(set_registro_datos(&operandos, parametros[1]));
   char *direcciones_tamanio = string_new();

   // concatenar tamanio con direcciones fisicas
   string_append(&direcciones_tamanio, operandos.direcciones_fisicas);
   string_append(&direcciones_tamanio, " ");
   string_append(&direcciones_tamanio, tamanio_valor);

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

u_int32_t obtener_tamanio_registro(char *parametros_recibidos)
{
   return string_starts_with(parametros_recibidos, "E") ? 4 : 1;
}
