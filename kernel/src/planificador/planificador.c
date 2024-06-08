#include "planificador.h"

u_int32_t pid_count;
u_int32_t quantum;

sem_t grado_multiprogramacion;

q_estado *cola_new;
q_estado *cola_ready;
q_estado *cola_ready_prioridad;
q_estado *cola_exit;

q_blocked *cola_blocked_interfaces;
q_blocked *cola_blocked_recursos;

static void finalizar_proceso_por_desconexion(void *proceso);
static void *consumir_io(void *);

static void *crear_proceso();
static void *finalizar_proceso();

static void pasar_a_exit(t_pcb *, motivo_finalizacion);
static void pasar_a_siguiente(t_pcb *);

static void manejar_wait(t_pcb *);
static void manejar_signal(t_pcb *);

static void *planificar_por_fifo();
static void *planificar_por_rr();
static void *planificar_por_vrr();

static void *cronometrar_quantum(void *);

static q_estado *cola_ready_segun_prioridad(t_pcb *);

void inicializar_planificador()
{
   pid_count = 0;
   quantum = get_quantum();

   sem_init(&grado_multiprogramacion, 0, get_grado_multiprogramacion());

   cola_new = crear_estado(NEW);
   cola_ready = crear_estado(READY);
   cola_ready_prioridad = crear_estado(READY);
   cola_exit = crear_estado(EXIT);
   cola_blocked_interfaces = crear_estado_blocked();
   cola_blocked_recursos = crear_estado_blocked();

   inicializar_recursos(cola_blocked_recursos);

   // ...................................................................

   void *(*planificador_corto_plazo)(void *) = NULL;
   algoritmo_planificacion algoritmo = get_algoritmo_planificacion();

   switch (algoritmo)
   {
   case FIFO:
      planificador_corto_plazo = &planificar_por_fifo;
      break;
   case RR:
      planificador_corto_plazo = &planificar_por_rr;
      break;
   case VRR:
      planificador_corto_plazo = &planificar_por_vrr;
      break;
   default:
      return;
   }

   pthread_t rutina_crear_proceso;
   pthread_create(&rutina_crear_proceso, NULL, &crear_proceso, NULL);
   pthread_detach(rutina_crear_proceso);

   pthread_t rutina_planificacion_corto_plazo;
   pthread_create(&rutina_planificacion_corto_plazo, NULL, planificador_corto_plazo, NULL);
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
   destruir_estado(cola_ready_prioridad);
   destruir_estado(cola_exit);
   destruir_estado_blocked(cola_blocked_interfaces, &destruir_io_queue);
   destruir_estado_blocked(cola_blocked_recursos, &destruir_resource_queue);
}

void iniciar_planificacion()
{
   // TODO
}

void detener_planificacion()
{
   // TODO
}

// TODO
void modificar_grado_multiprogramacion(u_int32_t nuevo_grado) {}

void ingresar_proceso(char *ruta_ejecutable)
{
   t_pcb *pcb = crear_pcb(pid_count++, ruta_ejecutable);
   set_quantum_pcb(pcb, quantum);
   log_creacion_proceso(pcb->pid);
   push_proceso(cola_new, pcb);
}

void conectar_entrada_salida(char *nombre_interfaz, int32_t fd_conexion)
{
   io_queue *cola_io = crear_io_queue(nombre_interfaz, fd_conexion);
   conectar_nueva_interfaz(cola_blocked_interfaces, cola_io, &consumir_io);
}

static void finalizar_proceso_por_desconexion(void *proceso)
{
   t_pcb *pcb = (t_pcb *)proceso;
   pasar_a_exit(pcb, INVALID_INTERFACE);
}

static void *consumir_io(void *cola_io)
{
   io_queue *interfaz = (io_queue *)cola_io;

   while (1)
   {
      t_pcb *pcb = pop_proceso(interfaz->cola_procesos);
      enviar_io_request(interfaz->fd_conexion, pcb->io_request);
      int32_t response = recibir_senial(interfaz->fd_conexion);

      // se resetea el campo io_req
      // principalmente para EXECUTED,
      // así no afectaría cuando vuelva a la CPU.
      // en otros casos no debería ser relevante
      reset_io_request(pcb);

      switch (response)
      {
      case INVALID_INSTRUCTION:
         pasar_a_exit(pcb, INVALID_INTERFACE);
         break;
      case EXECUTED:
         push_proceso(cola_ready, pcb);
         break;
      case -1: // cuando una interfaz se desconecta
         pasar_a_exit(pcb, INVALID_INTERFACE);
         q_estado *cola = desconectar_interfaz(cola_blocked_interfaces, interfaz->fd_conexion);
         mlist_iterate(cola->lista, &finalizar_proceso_por_desconexion);
         destruir_estado(cola);
         return NULL;
      }
   }

   return NULL;
}

// TODO
static void *crear_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso(cola_new);

      // ver si es la unica operacion que se hace antes de encolar a ready
      if (memoria_iniciar_proceso(pcb->pid, pcb->executable_path))
      {
         // no deberia ser "out of memory",
         // pero no hay otro tipo de error
         pasar_a_exit(pcb, OUT_OF_MEMORY);
         continue;
      }

      // ver si puede planificar o no,
      // o sea, si esta en pausa o no la planificacion
      sem_wait(&grado_multiprogramacion);
      push_proceso(cola_ready, pcb);
   }

   return NULL;
}

static void pasar_a_exit(t_pcb *pcb, motivo_finalizacion motivo)
{
   set_motivo_finalizacion(pcb, motivo);
   push_proceso(cola_exit, pcb);
}

// TODO
static void *finalizar_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso(cola_exit);
      if (memoria_finalizar_proceso(pcb->pid))
      {
         log_finalizacion_proceso(pcb->pid, pcb->motivo_finalizacion);
         destruir_pcb(pcb);
      }
   }
   return NULL;
}

static void pasar_a_siguiente(t_pcb *pcb)
{
   q_estado *ready = cola_ready_segun_prioridad(pcb);

   switch (pcb->motivo_desalojo)
   {
   case QUANTUM:
      log_fin_de_quantum(pcb->pid);
      push_proceso(ready, pcb);
      break;
   case IO:
      if (bloquear_para_io(cola_blocked_interfaces, pcb))
         pasar_a_exit(pcb, INVALID_INTERFACE);
      log_motivo_bloqueo(pcb->pid, INTERFAZ, NULL);
      break;
   case WAIT:
      manejar_wait(pcb);
      break;
   case SIGNAL:
      manejar_signal(pcb);
      break;
   case TERMINATED:
      pasar_a_exit(pcb, SUCCESS);
      break;
   }
}

static void manejar_wait(t_pcb *pcb)
{
   respuesta_solicitud respuesta = consumir_recurso(cola_blocked_recursos, pcb->resource);
   q_estado *ready = cola_ready_segun_prioridad(pcb);

   switch (respuesta)
   {
   case INVALID:
      pasar_a_exit(pcb, INVALID_RESOURCE);
      break;
   case ALL_RETAINED:
      bloquear_para_recurso(cola_blocked_recursos, pcb);
      log_motivo_bloqueo(pcb->pid, RECURSO, pcb->resource);
      break;
   case ASSIGNED:
      push_proceso(ready, pcb);
      break;
   default: // no debería llegar aca nunca (caso RELEASED)
      break;
   }

   // set_recurso_pcb(pcb, ""); // resetea el campo resource
}

static void manejar_signal(t_pcb *pcb)
{
   respuesta_solicitud respuesta = liberar_recurso(cola_blocked_recursos, pcb->resource);
   q_estado *ready = cola_ready_segun_prioridad(pcb);

   switch (respuesta)
   {
   case INVALID:
      pasar_a_exit(pcb, INVALID_RESOURCE);
      break;
   case RELEASED:
      t_pcb *proceso = desbloquear_para_recurso(cola_blocked_recursos, pcb->resource);
      push_proceso(ready, proceso);
      break;
   default: // no debería llegar aca nunca (caso ASSIGNED, ALL_RETAINED)
      break;
   }

   // set_recurso_pcb(pcb, ""); // resetea el campo resource
}

static void *planificar_por_fifo()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
      set_estado_pcb(proceso, EXEC);
      log_cambio_de_estado(proceso->pid, READY, EXEC);

      enviar_pcb_cpu(proceso);
      t_pcb *pos_exec = recibir_pcb_cpu();

      actualizar_pcb(&proceso, pos_exec);
      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *planificar_por_rr()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
      enviar_pcb_cpu(proceso);

      pthread_t rutina_cronometro;
      pthread_create(&rutina_cronometro, NULL, &cronometrar_quantum, &(quantum));
      pthread_detach(rutina_cronometro);

      t_pcb *pos_exec = recibir_pcb_cpu();
      pthread_cancel(rutina_cronometro);

      actualizar_pcb(&proceso, pos_exec);
      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *planificar_por_vrr()
{
   t_temporal *temporal = NULL;

   while (1)
   {
      q_estado *ready = hay_proceso(cola_ready_prioridad) ? cola_ready_prioridad : cola_ready;
      t_pcb *proceso = pop_proceso(ready);
      enviar_pcb_cpu(proceso);

      temporal = temporal_create();

      pthread_t rutina_cronometro;
      pthread_create(&rutina_cronometro, NULL, &cronometrar_quantum, &(proceso->quantum));
      pthread_detach(rutina_cronometro);

      t_pcb *pos_exec = recibir_pcb_cpu();
      pthread_cancel(rutina_cronometro);

      temporal_stop(temporal);
      int64_t transcurrido = temporal_gettime(temporal);
      // no se si afecta de algo al rendimiento
      // pero tengo que hacer destroy porque
      // se crea uno nuevo por cada ciclo del while
      temporal_destroy(temporal);

      actualizar_pcb(&proceso, pos_exec);

      u_int32_t quantum_proceso_nuevo = transcurrido < quantum ? quantum - transcurrido : quantum;
      set_quantum_pcb(proceso, quantum_proceso_nuevo);

      int8_t prioridad = transcurrido < quantum ? 1 : 0;
      set_prioridad(proceso, prioridad);

      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *cronometrar_quantum(void *milisegundos)
{
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

   u_int64_t microsegundos = (*(u_int32_t *)milisegundos) * 1000;
   for (u_int64_t i = 0; i < microsegundos; ++i)
   {
      usleep(1000);
      pthread_testcancel();
   }

   enviar_interrupcion();
   return NULL;
}

static q_estado *cola_ready_segun_prioridad(t_pcb *proceso)
{
   return proceso->priority == 0 ? cola_ready : cola_ready_prioridad;
}
