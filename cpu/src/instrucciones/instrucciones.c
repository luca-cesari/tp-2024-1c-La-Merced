#include "instrucciones.h"

t_dictionary *instrucciones;
t_dictionary *registros;
t_registers registros_cpu;

char *fetch()
{
   return "SET AX 10";
}

void check_interrupt()
{
   printf("Check interrupt\n");
}

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

void io_gen_sleep(Parametros parametros)
{
   // falta implementar
}

void execute(void (*instruccion)(Parametros), char *char_instruccion)
{

   char **instruc_parametros = instruccion_parametros(char_instruccion);
   instruccion(obtener_parametros(instruc_parametros));
}

char **instruccion_parametros(char *char_instruccion)
{
   return string_split(char_instruccion, " ");
}

void (*decode(char *char_instruccion))(Parametros)
{
   char **instruc_parametros = instruccion_parametros(char_instruccion);
   // char **instruccion_parametros;
   // instruccion_parametros = string_split(char_instruccion_completa, " ");
   instrucciones = dictionary_create();
   set_diccionario_instrucciones(instrucciones); // ver porque cada vez que busque operando tiene que llenar el diciconario
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

Parametros obtener_parametros(char **parametros) // CHEQUEAR SI ES QUE RECIBE NONE
{
   // RECIBE UN ARRAY CON LA INTRUCCION EN LA PRIMERA POSICION, POR LO QUE HAY QUE COMENZAR DESDE 1
   Parametros struct_parametros;
   struct_parametros.parametro1 = buscar_operando(parametros[1]);
   struct_parametros.parametro2 = buscar_operando(parametros[2]);
   // struct_parametros.parametro3 = buscar_operando(parametros[3]) // siguientes checkpoints
   // struct_parametros.parametro4 = buscar_operando(parametros[4])
   return struct_parametros;
}

Parametro buscar_operando(char *parametro)
{
   registros = dictionary_create();
   set_diccionario_registros(registros); // ver porque cada vez que busque operando tiene que llenar el diciconario
   Parametro operando;
   if (es_numero(parametro))
   {
      operando.tipo_dato = VALOR;
      operando.dato.valor = char_a_numero(parametro);
      return operando; // ver si usar el mas robusto
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
      operando.tipo_dato = INTERFAZ;
      operando.dato= parametro;
      return operando;
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
}

void set_diccionario_registros(t_dictionary *registros)
{
   dictionary_put(registros, "AX", &(registros_cpu.AX));
   dictionary_put(registros, "BX", &(registros_cpu.BX));
   dictionary_put(registros, "CX", &(registros_cpu.CX));
   dictionary_put(registros, "DX", &(registros_cpu.DX));
   dictionary_put(registros, "EAX", &(registros_cpu.EAX));
   dictionary_put(registros, "EBX", &(registros_cpu.EBX));
   dictionary_put(registros, "ECX", &(registros_cpu.ECX));
   dictionary_put(registros, "EDX", &(registros_cpu.EDX));
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
// VER QUE  OPCION ES MEJOR

// int es_numero_isDigit(const char *cadena) {
//     size_t longitud = strlen(cadena);
//     for (size_t i = 0; i < longitud; i++) {
//         if (!isdigit(cadena[i])) {
//             return 0; // No es un número
//         }
//     }
//     return 1; // Es un número
// }

int char_a_numero(char *parametro)
{
   return atoi(parametro);
}

// VER QUE  OPCION ES MEJOR

// long char_a_numero_robusto(char *parametro)
// {
//    char *endptr;
//    errno = 0;
//    long num = strtol(parametro, &endptr, 10); // el long tiene un maximo de tamanio permitido , 10 hace referencia a la base del numero, en este caso decimal

//    if (errno == ERANGE)
//    {
//       printf("Error de desbordamiento.\n");
//       return 0;
//    }
//    else if (*endptr != '\0')
//    {
//       printf("La conversión no fue completamente exitosa. Parte de la cadena no convertida: %s\n", endptr);
//       return 0;
//    }
//    else
//    {
//       return num;
//    }
// }

void ciclo_instruccion()
{
   char *char_instruccion = fetch();

   void (*instruccion)(Parametros) = decode(char_instruccion);
   if (instruccion != NULL)
   {
      execute(instruccion, char_instruccion); // no hace mucho, revisar
   }
   else
   {
      printf("Error: instrucción desconocida '%s'\n", char_instruccion);
   }

   check_interrupt();
}