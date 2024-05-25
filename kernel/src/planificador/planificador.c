#include "planificador.h"

// ver si va aca, puede q si, puede q no
sem_t grado_multiprogramacion;

q_estado *cola_new;
q_estado *cola_ready;
q_estado *cola_exit;

q_blocked *cola_blocked;

static void *consumir_io(void *cola_io);

void inicializar_planificador()
{
   sem_init(&grado_multiprogramacion, 0, get_grado_multiprogramacion());

   cola_new = crear_estado();
   cola_ready = crear_estado();
   cola_exit = crear_estado();
   cola_blocked = crear_estado_blocked();

   // .........................

   pthread_t rutina_crear_proceso;
   pthread_create(&rutina_crear_proceso, NULL, &crear_proceso, NULL);
   pthread_detach(rutina_crear_proceso);

   pthread_t rutina_planificacion_corto_plazo;
   pthread_create(&rutina_planificacion_corto_plazo, NULL, &planificar_por_fifo, NULL);
   pthread_detach(rutina_planificacion_corto_plazo);

   pthread_t rutina_finalizar_proceso;
   pthread_create(&rutina_finalizar_proceso, NULL, &finalizar_proceso, NULL);
   pthread_detach(rutina_finalizar_proceso);

   iniciar_planificacion();
}

void destruir_planificador()
{
   sem_destroy(&grado_multiprogramacion);

   destruir_estado(cola_new);
   destruir_estado(cola_ready);
   destruir_estado(cola_exit);
   destruir_estado_blocked(cola_blocked);
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
   t_pcb *pcb = crear_pcb(ruta_ejecutable);
   log_creacion_proceso(pcb->pid);
   push_proceso(cola_new, pcb);
}

void conectar_entrada_salida(char *nombre_interfaz, int32_t fd_conexion)
{
   io_queue *cola_io = crear_io_queue(nombre_interfaz, fd_conexion);
   conectar_nueva_interfaz(cola_blocked, cola_io, &consumir_io);
}

static void *consumir_io(void *cola_io)
{
   io_queue *io = (io_queue *)cola_io;

   while (1)
   {
      t_pcb *pcb = pop_proceso(io->cola);
      enviar_io_request(io->fd_conexion, pcb->io_request);
      int32_t response = recibir_senial(io->fd_conexion);

      switch (response)
      {
      case INVALID_INSTRUCTION:
         /* code */
         break;
      case EXECUTED:
         /* code */
         break;
      }
   }

   return NULL;
}

void *crear_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso(cola_new);

      // ver si es la unica operacion que se hace antes de encolar a ready
      if (memoria_iniciar_proceso(pcb->pid, pcb->executable_path))
      {
         pasar_a_exit(pcb, NEW);
         continue;
      }

      // ver si puede planificar o no,
      // o sea, si esta en pausa o no la planificacion
      sem_wait(&grado_multiprogramacion);
      push_proceso(cola_ready, pcb);
   }

   return NULL;
}

void pasar_a_exit(t_pcb *pcb, char *q_flag)
{
   if (strcmp(q_flag, NEW))
      sem_post(&grado_multiprogramacion);

   push_proceso(cola_exit, pcb);
}

void *finalizar_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso(cola_exit);
      if (memoria_finalizar_proceso(pcb->pid))
      {
         log_finalizacion_proceso(pcb->pid, SUCCESS); // no es correcto esto, habria q guardar un exit code en el pcb
         destruir_pcb(pcb);
      }
   }
   return NULL;
}

void *planificar_por_fifo()
{
   while (1)
   {
      t_pcb *pre_exec = pop_proceso(cola_ready);
      enviar_pcb_cpu(pre_exec);
      t_pcb *pos_exec = recibir_pcb_cpu();

      if (pos_exec == NULL)
      {
         pasar_a_exit(pre_exec, EXEC);
         continue;
      }

      destruir_pcb(pre_exec);
      // capaz analizar que onda, si necesita I/O o si termino
   }

   return NULL;
}

void *planificar_por_rr()
{
   u_int32_t quantum = get_quantum();

   while (1)
   {
      t_pcb *pre_exec = pop_proceso(cola_ready);
      enviar_pcb_cpu(pre_exec);

      pthread_t rutina_cronometro;
      pthread_create(&rutina_cronometro, NULL, &cronometrar_quantum, &(quantum));
      pthread_detach(rutina_cronometro);

      t_pcb *pos_exec = recibir_pcb_cpu();
      pthread_cancel(rutina_cronometro);

      // lo mismo que en fifo
      // chequear si necesita I/O o si termino
   }

   return NULL;
}

void *planificar_por_vrr()
{
   // usar un temporal aca
   return NULL;
}

void *cronometrar_quantum(void *milisegundos)
{
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

   u_int32_t segundos = (*(u_int32_t *)milisegundos) / 1000;
   for (u_int32_t i = 0; i < segundos; ++i)
   {
      sleep(1);
      pthread_testcancel();
   }

   enviar_interrupcion();
   return NULL;
}