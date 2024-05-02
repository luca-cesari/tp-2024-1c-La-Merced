#include "consola.h"

void iniciar_consola()
{
   char *comando = NULL;
   char *operacion = NULL;
   char *argumento = NULL;

   while (1)
   {
      comando = readline("> ");
      char **vec_comando = string_split(comando, " ");

      if (vec_comando[0] == NULL)
      {
         free(comando); // por las dudas
         string_array_destroy(vec_comando);
         continue;
      }

      operacion = vec_comando[0];
      argumento = vec_comando[1];

      ejecutar_comando(operacion, argumento);

      free(comando); // lo mismo, por las dudas
      string_array_destroy(vec_comando);
   }
}

void leer_script(char *ruta_script)
{
   FILE *script;
   char *linea = NULL;

   script = fopen(ruta_script, "r");
   if (script == NULL)
   {
      printf("No se pudo abrir el script \n");
      return;
   }

   while (getline(&linea, NULL, script) != -1)
   {
      char **vec_comando = string_split(linea, " ");
      char *operacion = vec_comando[0];
      char *argumento = vec_comando[1];

      ejecutar_comando(operacion, argumento);

      free(linea); // ni idea si hace falta
      string_array_destroy(vec_comando);
   }

   free(linea);
   fclose(script);
}

void ejecutar_comando(char *operacion, char *argumento)
{
   if (strcmp(operacion, INICIAR_PLANIFICACION) == 0)
   {
      printf("Iniciar planificacion \n");
      iniciar_planificacion();
      return;
   }

   if (strcmp(operacion, DETENER_PLANIFICACION) == 0)
   {
      printf("Detener planificacion \n");
      detener_planificacion();
      return;
   }

   if (strcmp(operacion, PROCESO_ESTADO) == 0)
   {
      printf("Estado de proceso \n");
      return;
   }

   // Si argumento es NULL, no puede seguir con ninguna de las siguientes operaciones
   if (argumento == NULL)
      return;

   if (strcmp(operacion, EJECUTAR_SCRIPT) == 0)
   {
      printf("Ejecutar script \n");
      printf("Path del script: %s \n", argumento);
      leer_script(argumento);
      return;
   }

   if (strcmp(operacion, INICIAR_PROCESO) == 0)
   {
      printf("Iniciar proceso \n");
      printf("Path del ejecutable: %s \n", argumento);
      crear_proceso(argumento);
      return;
   }

   if (strcmp(operacion, FINALIZAR_PROCESO) == 0)
   {
      printf("Finalizar proceso \n");
      printf("PID del proceso: %s \n", argumento);
      finalizar_proceso(atoi(argumento));
      return;
   }

   if (strcmp(operacion, MULTIPROGRAMACION) == 0)
   {
      printf("Cambio de Multiprogramacion \n");
      printf("Grado de multiprogramacion: %s \n", argumento);
      modificar_grado_multiprogramacion(atoi(argumento));
      return;
   }
}