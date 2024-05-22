#include "instrucciones.h"
#include "registros/registros.c"

t_dictionary *instrucciones;
t_dictionary *registros;

void ciclo_instruccion()
{
   char *char_instruccion = fetch();
   void (*instruccion)(struct Parametros);
   decode(char_instruccion, &instruccion);
   execute(instruccion); // no hace mucho, revisar
   check_interrupt();
}

// void decode(char *char_instruccion, void (**instruccion)(struct Parametros))
// {
//    char **instruccion_parametros;
//    instruccion_parametros = string_split(instruccion, " ");
//    return generar_instruccion(instruccion_parametros);
// }

void decode(char *char_instruccion_completa, void (**instruccion)(struct Parametros))
{
   char **instruccion_parametros;
   instruccion_parametros = string_split(char_instruccion_completa, " ");
   instrucciones = dictionary_create();
   set_diccionario_instrucciones(instrucciones); // ver porque cada vez que busque operando tiene que llenar el diciconario
   if (dictionary_has_key(instrucciones, instrucion_parametros[0]))
   {
      void (*set_instruccion)(struct Parametros);
      set_instruccion = dictionary_get(instrucciones, instrucion_parametros[0]);
      instruccion = set_instruccion(obtener_parametros(instruc_parametros));
   }
   else
   {
      // En caso de instrucción desconocida
      printf("Instrucción desconocida: %s\n", instrucion_parametros[0]);
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

union Parametro *buscar_operando(char *parametro)
{
   registros = dictionary_create();
   set_diccionario_registros(registros); // ver porque cada vez que busque operando tiene que llenar el diciconario
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

void set_diccionario_instrucciones(t_dictionary *instrucciones)
{
   dictionary_put(instrucciones, "SET", set);
   dictionary_put(instrucciones, "SUM", sum);
   dictionary_put(instrucciones, "SUB", sub);
   dictionary_put(instrucciones, "JNZ", jnz);
   dictionary_put(instrucciones, "IO_GEN_SLEEP", io_gen_sleep);
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