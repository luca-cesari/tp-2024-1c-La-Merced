#include "planificador.h"

// ver si va aca, puede q si, puede q no
sem_t grado_multiprogramacion;

q_new *cola_new;
q_ready *cola_ready;
q_exec *cola_exec;
q_exit *cola_exit;

void inicializar_planificador()
{
   sem_init(&grado_multiprogramacion, 0, get_grado_multiprogramacion());

   cola_new = crear_estado_new();
   cola_ready = crear_estado_ready();
   cola_exec = crear_estado_exec();
   cola_exit = crear_estado_exit();

   // .........................

   pthread_t rutina_crear_proceso;
   pthread_create(&rutina_crear_proceso, NULL, &crear_proceso, NULL);
   pthread_detach(rutina_crear_proceso);

   pthread_t rutina_planificacion_corto_plazo;
   pthread_create(&rutina_planificacion_corto_plazo, NULL, &planificar_a_corto_plazo, &elegir_por_fifo);
   pthread_detach(rutina_planificacion_corto_plazo);

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
   destruir_estado_exec(cola_exec);
   destruir_estado_exit(cola_exit);
}

void iniciar_planificacion()
{
}

void detener_planificacion()
{
}

void modificar_grado_multiprogramacion(u_int32_t nuevo_grado) {}

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

      // ver si es la unica operacion que se hace antes de encolar a ready
      if (reservar_paginas(pcb))
      {
         pasar_a_exit(pcb, NEW);
         continue;
      }

      // ver si puede planificar o no,
      // o sea, si esta en pausa o no la planificacion
      sem_wait(&grado_multiprogramacion);
      push_proceso_ready(cola_ready, pcb);
   }

   return NULL;
}

void pasar_a_exit(t_pcb *pcb, char *q_flag)
{
   if (strcmp(q_flag, NEW) == 0)
      sem_post(&grado_multiprogramacion);

   push_proceso_exit(cola_exit, pcb);
}

void *finalizar_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso_exit(cola_exit);
      liberar_memoria(pcb);
   }
   return NULL;
}

void *planificar_a_corto_plazo(void *ptr_planificador)
{
   t_pcb *(*planificador)() = ptr_planificador;

   while (1)
   {
      t_pcb *pcb = (*planificador)();

      // no es solo esto, hay q hablar con cpu
      push_proceso_exec(cola_exec, pcb);
   }
   return NULL;
}

t_pcb *elegir_por_fifo()
{
   return pop_proceso_ready(cola_ready);
}

t_pcb *elegir_por_rr()
{
   return NULL;
}

t_pcb *elegir_por_vrr()
{
   return NULL;
}