#include "instrucciones.h"

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

void *procesar_instruccion(int PC)
{
   char **instruccion_parametros;
   char *instruccion_completa = obtener_instruccion(PC); // ver porque esta funcion es de memoria, y no la conoce supuestamente, hay que hacer cosas con paquetes o no se
   instruccion_parametros = string_split(instruccion_completa, " ");
   return generar_instruccion(instruccion_parametros);
}