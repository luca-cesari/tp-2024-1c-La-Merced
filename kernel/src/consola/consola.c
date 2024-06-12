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
   size_t len = 0;
   ssize_t read;

   script = fopen(ruta_script, "r");
   if (script == NULL)
   {
      perror("Error al abrir el script");
      return;
   }

   while ((read = getline(&linea, &len, script)) != -1)
   {
      char **vec_comando = string_split(linea, " ");
      char *operacion = vec_comando[0];
      char *argumento = vec_comando[1];

      ejecutar_comando(operacion, argumento);

      // free(linea); // ni idea si hace falta
      string_array_destroy(vec_comando);
   }

   free(linea);
   fclose(script);
}

void ejecutar_comando(char *operacion, char *argumento)
{
   if (strcmp(operacion, INICIAR_PLANIFICACION) == 0)
   {
      printf("Iniciar Planificacion \n");
      iniciar_planificacion();
      return;
   }

   if (strcmp(operacion, DETENER_PLANIFICACION) == 0)
   {
      printf("Detener Planificacion \n");
      detener_planificacion();
      return;
   }

   if (strcmp(operacion, PROCESO_ESTADO) == 0)
   {
      printf("Estado de Proceso \n");
      return;
   }

   // Si argumento es NULL, no puede seguir con ninguna de las siguientes operaciones
   if (argumento == NULL)
      return;

   if (strcmp(operacion, EJECUTAR_SCRIPT) == 0)
   {
      printf("Ejecutar Script \n");
      printf("Path del Script: %s \n", argumento);
      leer_script(argumento);
      return;
   }

   if (strcmp(operacion, INICIAR_PROCESO) == 0)
   {
      printf("Iniciar Proceso \n");
      printf("Path del Ejecutable: %s \n", argumento);
      crear_proceso(argumento);
      return;
   }

   if (strcmp(operacion, FINALIZAR_PROCESO) == 0)
   {
      printf("Finalizar Proceso \n");
      printf("PID del Proceso: %s \n", argumento);
      matar_proceso(atoi(argumento));
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