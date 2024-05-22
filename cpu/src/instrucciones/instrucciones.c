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
   instruccion_parametros = string_split(instruccion_completa, " ");
   return generar_instruccion(instruccion_parametros);
}

void *set(*registro, int *valor)
{
}

void *generar_instruccion(char **instruc_parametros)
{
   char *instruccion = instruc_parametros[0];
   if (strcmp(instruccion, SET) == 0)
   {
      return set(instruc_parametros[1], instruc_parametros[2]);
   }
   else if (strcmp(instruccion, SUM) == 0)
   {
      return sum(instruc_parametros[1], instruc_parametros[2]);
   }
   else if (strcmp(instruccion, SUB) == 0)
   {
      return sub(instruc_parametros[1], instruc_parametros[2]);
   }
   else if (strcmp(instruccion, JNZ) == 0)
   {
      return jnz(instruc_parametros[1], instruc_parametros[2]);
   }
   else if (strcmp(instruccion, IO_GEN_SLEEP) == 0)
   {
      return io_gen_sleep(instruc_parametros[1], instruc_parametros[2]);
   }
   else
   {
      // En caso de instrucción desconocida
      printf("Instrucción desconocida: %s\n", instruccion);
   }
}

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
void *obtener_parametros(char **parametros){
   
}

void *buscar_operandos(char **parametros)
{
   set_diccionario_parametros(t_dictionary * registros); // ver porque cada vez que busque operandos de una instruccion tiene que llenar el diciconario
   int i=0;
   while (parametros[i] != NULL)
   {
      if (es_numero(parametros[i]))
      {
         return char_a_numero(parametros[i]); // ver si usar el mas robusto
      }
      else if (dictionary_has_key(registros, parametros[i]))
      {
      }
      else
      {
         // en caso de operando desconocido
         printf("Operando desconocido: %s\n", parametros[i]);
      }
      i++;
   }
}

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

t_dictionary *instrucciones = *dictionary_create();
t_dictionary *registros = *dictionary_create();
