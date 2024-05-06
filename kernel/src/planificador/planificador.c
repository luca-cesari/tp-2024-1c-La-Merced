#include "planificador.h"

// ver si va aca, puede q si, puede q no
sem_t grado_multiprogramacion;

q_new *cola_new;
q_ready *cola_ready;

void inicializar_planificador()
{
   sem_init(&grado_multiprogramacion, 0, 5); // valor inicial depende del config

   cola_new = crear_estado_new();
   cola_ready = crear_estado_ready();

   // .........................

   pthread_t rutina_crear_proceso;
   pthread_create(&rutina_crear_proceso, NULL, &crear_proceso, NULL);
   pthread_detach(rutina_crear_proceso);

   pthread_t rutina_finalizar_proceso;
   pthread_create(&rutina_finalizar_proceso, NULL, &finalizar_proceso, NULL);
   pthread_detach(rutina_finalizar_proceso);

   iniciar_planificacion();
}

void destruir_planificador()
{
   sem_destroy(&grado_multiprogramacion);
   destruir_estado_new(cola_new);
   destruir_estado_ready(cola_ready);
}

void iniciar_planificacion()
{
}

void detener_planificacion()
{
}

void modificar_grado_multiprogramacion(u_int8_t nuevo_grado) {}

void ingresar_proceso(char *ruta_ejecutable)
{
   t_pcb *pcb = pcb_create(ruta_ejecutable);
   log_creacion_proceso(pcb->pid);
   push_proceso_nuevo(cola_new, pcb);
}

void *crear_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso_nuevo(cola_new);

      // tiene q pedir cosas a la memoria y esperar a que responda
      // !! contemplar si falla (debería pasar directo a EXIT?)
      // break;

      // ver si puede planificar o no,
      // o sea, si esta en pausa o no la planificacion
      sem_wait(&grado_multiprogramacion);
      push_proceso_ready(cola_ready, pcb);
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

   // sem_post(&hay_proceso_en_exit);
   //...
}

void *finalizar_proceso()
{
   while (1)
   {
      // sem_wait(&hay_proceso_en_exit);

      // t_pcb *pcb = pop_mutex_queue(cola_exit);
      // libera memoria y esas cosas
      // ...
   }

   return NULL;
}