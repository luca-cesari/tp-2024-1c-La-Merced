#include "consola.h"

static void leer_script(char *);
static void ejecutar_comando(char *, char *);
static void imprimir_procesos();
static void destruir_proceso(void *);

static const char *comandos[] = {
    INICIAR_PLANIFICACION,
    DETENER_PLANIFICACION,
    PROCESO_ESTADO,
    EJECUTAR_SCRIPT,
    INICIAR_PROCESO,
    FINALIZAR_PROCESO,
    MULTIPROGRAMACION,
    NULL};

void inicializar_readline()
{
   rl_readline_name = "Consola";
   rl_attempted_completion_function = completar_comando;
}

void iniciar_consola()
{
   char *comando = NULL;
   char *operacion = NULL;
   char *argumento = NULL;

   inicializar_readline();

   while (1)
   {
      comando = readline("> ");
      char **vec_comando = string_split(comando, " ");

      if (comando)
      {
         add_history(comando);
      }

      if (vec_comando[0] == NULL)
      {
         free(comando);
         string_array_destroy(vec_comando);
         continue;
      }
      if (strcmp(vec_comando[0], "KILL") == 0)
      {
         free(comando);
         string_array_destroy(vec_comando);
         return;
      }

      operacion = vec_comando[0];
      argumento = vec_comando[1];

      ejecutar_comando(operacion, argumento);

      free(comando);
      string_array_destroy(vec_comando);
   }
}

static void leer_script(char *ruta_script)
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
      char *argumento = eliminar_salto_linea(vec_comando[1]);

      ejecutar_comando(operacion, argumento);

      // free(linea); // ni idea si hace falta
      free(argumento);
      string_array_destroy(vec_comando);
   }

   free(linea);
   fclose(script);
}

static void ejecutar_comando(char *operacion, char *argumento)
{
   if (strcmp(operacion, INICIAR_PLANIFICACION) == 0)
   {
      iniciar_planificacion();
      return;
   }

   if (strcmp(operacion, DETENER_PLANIFICACION) == 0)
   {
      detener_planificacion();
      return;
   }

   if (strcmp(operacion, PROCESO_ESTADO) == 0)
   {
      imprimir_procesos();
      return;
   }

   // Si argumento es NULL, no puede seguir con ninguna de las siguientes operaciones
   if (argumento == NULL)
      return;

   if (strcmp(operacion, EJECUTAR_SCRIPT) == 0)
   {
      leer_script(argumento);
      return;
   }

   if (strcmp(operacion, INICIAR_PROCESO) == 0)
   {
      crear_proceso(argumento);
      return;
   }

   if (strcmp(operacion, FINALIZAR_PROCESO) == 0)
   {
      matar_proceso(atoi(argumento));
      return;
   }

   if (strcmp(operacion, MULTIPROGRAMACION) == 0)
   {
      modificar_grado_multiprogramacion(atoi(argumento));
      return;
   }
}

static void imprimir_procesos()
{
   t_list *procesos = obtener_procesos();
   char *estados[5] = {"NEW", "READY", "EXECUTING", "BLOCKED", "EXIT"};

   for (int i = 0; i < 5; i++)
   {
      printf("PROCESOS EN %s: \n", estados[i]);

      int8_t _es_proceso(void *pcb)
      {
         t_pcb *proceso = (t_pcb *)pcb;
         return proceso->estado == i;
      };
      t_list *procesos_estado = list_filter(procesos, (void *)_es_proceso);

      t_list_iterator *iterador = list_iterator_create(procesos_estado);
      while (list_iterator_has_next(iterador))
      {
         t_pcb *proceso = list_iterator_next(iterador);
         printf("     %d \n", proceso->pid);
      }
      list_iterator_destroy(iterador);

      list_destroy_and_destroy_elements(procesos_estado, &destruir_proceso);
      printf("\n");
   }

   list_destroy_and_destroy_elements(procesos, &destruir_proceso);
}

static void destruir_proceso(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   destruir_pcb(pcb);
}

// Función para autocompletar comandos
char **completar_comando(const char *texto, int inicio, int fin)
{
   char **coincidencias;
   if (inicio == 0)
   {
      coincidencias = rl_completion_matches(texto, generador_comando);
   }
   else
   {
      coincidencias = NULL;
   }
   return coincidencias;
}

// Generador de comandos para autocompletado
char *generador_comando(const char *texto, int estado)
{
   static int indice;
   static int longitud;
   const char *comando;

   if (!estado)
   {
      indice = 0;
      longitud = strlen(texto);
   }

   while ((comando = comandos[indice++]))
   {
      if (strncmp(comando, texto, longitud) == 0)
      {
         return strdup(comando);
      }
   }
   return NULL;
}
