#include "planificador.h"

// ver si va aca, puede q si, puede q no
sem_t grado_multiprogramacion;

sem_t puede_planificar;
sem_t hay_proceso_en_new;
sem_t hay_proceso_en_ready;
sem_t hay_proceso_en_exit;

t_mutex_queue *cola_new;
t_mutex_queue *cola_ready;
t_mutex_queue *cola_exec;

// quiza no es correcto una cola de bloqueados
// capaz hay q implementar de otra forma
t_mutex_queue *cola_blocked;

t_mutex_queue *cola_exit;

void inicializar_planificador()
{
   sem_init(&grado_multiprogramacion, 0, 5); // valor inicial depende del config
   sem_init(&puede_planificar, 0, 0);
   sem_init(&hay_proceso_en_new, 0, 0);
   sem_init(&hay_proceso_en_ready, 0, 0);
   sem_init(&hay_proceso_en_exit, 0, 0);

   cola_new = crear_mutex_queue();
   cola_ready = crear_mutex_queue();
   cola_exec = crear_mutex_queue();
   cola_blocked = crear_mutex_queue();
   cola_exit = crear_mutex_queue();

   pthread_t rutina_proceso_nuevo;
   pthread_create(&rutina_proceso_nuevo, NULL, &pasar_a_ready, NULL);
   pthread_detach(rutina_proceso_nuevo);

   pthread_t rutina_finalizar_proceso;
   pthread_create(&rutina_finalizar_proceso, NULL, &finalizar_proceso, NULL);
   pthread_detach(rutina_finalizar_proceso);

   iniciar_planificacion();
}

void destruir_planificador()
{
   sem_destroy(&grado_multiprogramacion);
   sem_destroy(&puede_planificar);
   sem_destroy(&hay_proceso_en_new);
   sem_destroy(&hay_proceso_en_ready);

   destruir_mutex_queue(cola_new);
   destruir_mutex_queue(cola_ready);
   destruir_mutex_queue(cola_exec);
   destruir_mutex_queue(cola_blocked);
   destruir_mutex_queue(cola_exit);
}

void iniciar_planificacion()
{
   sem_post(&puede_planificar);
}

void detener_planificacion()
{
   sem_wait(&puede_planificar);
}

void modificar_grado_multiprogramacion(u_int8_t nuevo_grado) {}

void crear_proceso(char *ruta)
{
   // crea pcb y encola
   // ... (peticiones a memoria, hace todo lo que tiene q hacer)

   sem_post(&hay_proceso_en_new);
}

void *pasar_a_ready()
{
   while (1)
   {
      sem_wait(&puede_planificar);
      sem_wait(&hay_proceso_en_new);
      sem_wait(&grado_multiprogramacion);

      t_pcb *pcb = pop_mutex_queue(cola_new);
      push_mutex_queue(cola_ready, pcb);

      sem_post(&hay_proceso_en_ready);
      // ...
   }

   return NULL;
}

void pasar_a_exit(u_int32_t pid)
{
   // ...

   // en teoria puede ser llamado en cualquier momento
   // tener en cuenta:
   // 1. Que hace con el pid rebibido? no hay función de "búsqueda" para las colas
   // 2. Suponiendo que sabe en qué cola está, habría que esperar a q llegue su turno para popearlo

   // (ya c que excede del contexto de esta funcion)
   // pero para los planificadores de corto plazo pasa lo mismo
   // elige un proceso a su criterio,
   // pero de alguna forma lo tiene q popear y encolar a exec

   sem_post(&hay_proceso_en_exit);
   //...
}

void *finalizar_proceso()
{
   while (1)
   {
      sem_wait(&hay_proceso_en_exit);

      // t_pcb *pcb = pop_mutex_queue(cola_exit);
      // libera memoria y esas cosas
      // ...
   }

   return NULL;
}