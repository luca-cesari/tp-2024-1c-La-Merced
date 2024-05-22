#include "instrucciones.h"

void ciclo_instruccion()
{
   char *char_instruccion = fetch();
   void *instruccion = decode(char_instruccion);
   execute(instruccion); // no hace mucho, revisar
   check_interrupt();
}

void *decode(char *instruccion)
{
   char **instruccion_parametros;
   instruccion_parametros = string_split(instruccion, " ");
   return generar_instruccion(instruccion_parametros);
}

void *generar_instruccion(char **instruc_parametros)
{
   set_diccionario_parametros(instrucciones); // ver porque cada vez que busque operando tiene que llenar el diciconario
   if (dictionary_has_key(instrucciones, parametro))
   {
      void *instruccion = dictionary_get(instrucciones, instruc_parametros[0]);
      return instruccion(obtener_parametros(instruc_parametros));
   }
   else
   {
      // En caso de instrucción desconocida
      printf("Instrucción desconocida: %s\n", instruc_parametros[0]);
   }
}

 struct Parametros obtener_parametros(char **parametros)
{
   // RECIBE UN ARRAY CON LA INTRUCCION EN LA PRIMERA POSICION, POR LO QUE HAY QUE COMENZAR DESDE 1
   struct Parametros struct_parametros;
   struct_parametros.parametro1 = buscar_operando(parametros[1]);
   struct_parametros.parametro2 = buscar_operando(parametros[2]);
   // struct_parametros.parametro3 = buscar_operando(parametros[3]) // siguientes checkpoints
   // struct_parametros.parametro4 = buscar_operando(parametros[4])
   return struct_parametros;
}

void *buscar_operando(char *parametro)
{
   set_diccionario_parametros(registros); // ver porque cada vez que busque operando tiene que llenar el diciconario
   if (es_numero(parametro))
   {
      return char_a_numero(parametro); // ver si usar el mas robusto
   }
   else if (dictionary_has_key(registros, parametro))
   {
      return dictionary_get(registros, parametro);
   }
   else // falta interfaz
   {
      // en caso de operando desconocido
      printf("Operando desconocido: %s\n", parametro);
   }
}

///////// ESTRUCTURAS AUXILIARES ///////////
t_dictionary *instrucciones = *dictionary_create();
t_dictionary *registros = *dictionary_create();

void set_diccionario_instrucciones(t_dictionary *instrucciones)
{
   dictionary_put(instrucciones, "SET", set);
   dictionary_put(instrucciones, "SUM", sum);
   dictionary_put(instrucciones, "SUB", sub);
   dictionary_put(instrucciones, "JNZ", jnz);
   dictionary_put(instrucciones, "IO_GEN_SLEEP", io_gen_sleep);
}

void set_diccionario_parametros(t_dictionary *registros)
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