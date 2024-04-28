#include "consola.h"

void iniciar_consola()
{
   char *comando;
   char *operacion;
   char *argumento;

   while (1)
   {
      comando = readline("> ");
      char **vec_comando = string_split(comando, " ");

      if (vec_comando[0] == NULL)
      {
         free(comando);
         free(vec_comando);
         continue;
      }

      operacion = vec_comando[0];
      argumento = vec_comando[1];

      ejecutar_comando(operacion, argumento);

      free(comando);
      free(operacion);
   }
}

void ejecutar_comando(char *operacion, char *argumento)
{
   // Que asco los if
   // pero no deja hacer switch de strings

   if (strcmp(operacion, EJECUTAR_SCRIPT) == 0)
   {
      printf("Ejecutar script \n");
      printf("Path del script: %s \n", argumento);
   }
   else if (strcmp(operacion, INICIAR_PROCESO) == 0)
   {
      printf("Iniciar proceso \n");
      printf("Path del ejecutable: %s \n", argumento);
   }
   else if (strcmp(operacion, FINALIZAR_PROCESO) == 0)
   {
      printf("Finalizar proceso \n");
      printf("PID del proceso: %s \n", argumento);
   }
   else if (strcmp(operacion, INICIAR_PLANIFICACION) == 0)
   {
      printf("Iniciar planificacion \n");
   }
   else if (strcmp(operacion, DETENER_PLANIFICACION) == 0)
   {
      printf("Detener planificacion \n");
   }
   else if (strcmp(operacion, MULTIPROGRAMACION) == 0)
   {
      printf("Cambio de Multiprogramacion \n");
      printf("Grado de multiprogramacion: %s \n", argumento);
   }
   else if (strcmp(operacion, PROCESO_ESTADO) == 0)
   {
      printf("Estado de proceso \n");
   }
   else
   {
      // caso typo
      return;
   }
}