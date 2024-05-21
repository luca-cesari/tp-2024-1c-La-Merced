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

long char_a_numero_robusto(char *parametro)
{
   char *endptr;
   errno = 0;
   long num = strtol(parametro, &endptr, 10); // el long tiene un maximo de tamanio permitido , 10 hace referencia a la base del numero, en este caso decimal

   if (errno == ERANGE)
   {
      printf("Error de desbordamiento.\n");
      return 0;
   }
   else if (*endptr != '\0')
   {
      printf("La conversión no fue completamente exitosa. Parte de la cadena no convertida: %s\n", endptr);
      return 0;
   }
   else
   {
      return num;
   }
}

int *buscar_operando(char *parametro)
{
   if (es_numero(parametro))
   {
      return char_a_numero(parametro); // ver si usar el mas robusto
   }
   else if (strcmp(parametro, "AX") == 0)
   {
      return &(registros_cpu.AX);
   }
   else if (strcmp(parametro, "BX") == 0)
   {
      return &(registros_cpu.BX);
   }
   else if (strcmp(parametro, "CX") == 0)
   {
      return &(registros_cpu.CX);
   }
   else if (strcmp(parametro, "DX") == 0)
   {
      return &(registros_cpu.DX);
   }
   else if (strcmp(parametro, "EAX") == 0)
   {
      return &(registros_cpu.EAX);
   }
   else if (strcmp(parametro, "EBX") == 0)
   {
      return &(registros_cpu.EBX);
   }
   else if (strcmp(parametro, "ECX") == 0)
   {
      return &(registros_cpu.ECX);
   }
   else if (strcmp(parametro, "EDX") == 0)
   {
      return &(registros_cpu.EDX);
   }
   else
   {
      // en caso de operando desconocido
      printf("Operando desconocido: %s\n", parametro);
   }
}

void *procesar_instruccion(int PC)
{
   char **instruccion_parametros;
   char *instruccion_completa = obtener_instruccion(PC); // ver porque esta funcion es de memoria, y no la conoce supuestamente, hay que hacer cosas con paquetes o no se
   instruccion_parametros = string_split(instruccion_completa, " ");
   return generar_instruccion(instruccion_parametros);
}