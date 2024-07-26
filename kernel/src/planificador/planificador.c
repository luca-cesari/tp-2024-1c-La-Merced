#include "planificador.h"

u_int32_t pid_count;
u_int32_t quantum;

sem_mp_t *grado_multiprogramacion;

q_estado *cola_new;
q_estado *cola_ready;
q_estado *cola_ready_prioridad;
q_estado *cola_exec;
q_estado *cola_exit;

q_blocked *cola_blocked_interfaces;
q_blocked *cola_blocked_recursos;

static void finalizar_proceso_por_desconexion(void *);
static void *consumir_io(void *);

static void *admitir_proceso();
static void *finalizar_proceso();
static void liberar_recursos(t_pcb *);

static void pasar_a_exit(t_pcb *, motivo_finalizacion);
static void pasar_a_siguiente(t_pcb *);
static void pasar_a_ready_segun_prioridad(t_pcb *);

static void manejar_wait(t_pcb *);
static void manejar_signal(t_pcb *);

static void *planificar_por_fifo();
static void *planificar_por_rr();
static void *planificar_por_vrr();

static void *cronometrar_quantum(void *);

void inicializar_planificador()
{
   pid_count = 1;
   quantum = get_quantum();

   grado_multiprogramacion = sem_mp_create(get_grado_multiprogramacion());

   cola_new = crear_estado(NEW);
   cola_ready = crear_estado(READY);
   cola_ready_prioridad = crear_estado(READY);
   cola_exec = crear_estado(EXEC);
   cola_exit = crear_estado(EXIT);
   cola_blocked_interfaces = crear_estado_blocked();
   cola_blocked_recursos = crear_estado_blocked();

   inicializar_lista_procesos();

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

   pthread_t rutina_admitir_proceso;
   pthread_create(&rutina_admitir_proceso, NULL, &admitir_proceso, NULL);
   pthread_detach(rutina_admitir_proceso);

   pthread_t rutina_planificacion_corto_plazo;
   pthread_create(&rutina_planificacion_corto_plazo, NULL, planificador_corto_plazo, NULL);
   pthread_detach(rutina_planificacion_corto_plazo);

   pthread_t rutina_finalizar_proceso;
   pthread_create(&rutina_finalizar_proceso, NULL, &finalizar_proceso, NULL);
   pthread_detach(rutina_finalizar_proceso);
}

void destruir_planificador()
{
   sem_mp_destroy(grado_multiprogramacion);

   destruir_estado(cola_new);
   destruir_estado(cola_ready);
   destruir_estado(cola_ready_prioridad);
   destruir_estado(cola_exec);
   destruir_estado(cola_exit);
   destruir_estado_blocked(cola_blocked_interfaces, &destruir_io_queue);
   destruir_estado_blocked(cola_blocked_recursos, &destruir_resource_queue);

   destruir_lista_procesos();
}

void iniciar_planificacion()
{
   bloquear_estado(cola_exec);
   bloquear_estado(cola_ready_prioridad);
   bloquear_estado(cola_ready);
   bloquear_colas_io(cola_blocked_interfaces);
   bloquear_colas_de_recursos(cola_blocked_recursos);
}

void detener_planificacion()
{
   desbloquear_colas_de_recursos(cola_blocked_recursos);
   desbloquear_colas_io(cola_blocked_interfaces);
   desbloquear_estado(cola_ready);
   desbloquear_estado(cola_ready_prioridad);
   desbloquear_estado(cola_exec);
}

void modificar_grado_multiprogramacion(u_int32_t nuevo_grado)
{
   sem_mp_set(grado_multiprogramacion, nuevo_grado);
}

void crear_proceso(char *ruta_ejecutable)
{
   t_pcb *pcb = crear_pcb(pid_count++, ruta_ejecutable);
   set_quantum_pcb(pcb, quantum);

   registrar_proceso(pcb);
   push_proceso(cola_new, pcb);

   log_creacion_proceso(pcb->pid);
}

void matar_proceso(u_int32_t pid)
{
   t_pcb *proceso = obtener_proceso_por_pid(pid);
   if (proceso == NULL)
      return;

   switch (proceso->estado)
   {
   case NEW:
      proceso->motivo_finalizacion = INTERRUPTED_BY_USER;
      return;
   case READY:
      proceso = remove_proceso(cola_ready, pid);
      pasar_a_exit(proceso, INTERRUPTED_BY_USER);
      return;
   case BLOCKED:
      if (proceso->motivo_desalojo == IO)
         proceso = remove_proceso_cola_io(cola_blocked_interfaces, proceso->io_request->interface_name, pid);

      if (proceso->motivo_desalojo == WAIT)
         proceso = remove_proceso_cola_recurso(cola_blocked_recursos, proceso->resource, pid);

      pasar_a_exit(proceso, INTERRUPTED_BY_USER);
      return;
   case EXEC:
      enviar_interrupcion(USER_INT);
      return;
   case EXIT:
      return;
   }
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
      t_pcb *pcb = peek_proceso(interfaz->cola_procesos);
      u_int32_t pid = pcb->pid;
      enviar_io_request(interfaz->fd_conexion, pcb->io_request);
      int32_t response = recibir_senial(interfaz->fd_conexion);

      pcb = remove_proceso(interfaz->cola_procesos, pid);
      if (pcb == NULL)
         continue;

      switch (response)
      {
      case INVALID_INSTRUCTION:
         pasar_a_exit(pcb, INVALID_INTERFACE);
         break;
      case EXECUTED:
         pasar_a_ready_segun_prioridad(pcb);
         break;
      case FAILED:
         pasar_a_exit(pcb, ERROR);
         break;
      case -1: // cuando una interfaz se desconecta
         pasar_a_exit(pcb, INVALID_INTERFACE);

         // unicamente desencola la interfaz de la cola blocked (no destruye)
         q_estado *cola = desconectar_interfaz(cola_blocked_interfaces, interfaz->fd_conexion);
         mlist_iterate(cola->lista, &finalizar_proceso_por_desconexion);
         log_evento(string_from_format("Se desconecta la interfaz %s", interfaz->nombre_interfaz));
         destruir_io_queue(interfaz);
         return NULL;
      }
   }

   return NULL;
}

static void *admitir_proceso()
{
   while (1)
   {
      t_pcb *pcb = peek_proceso(cola_new);

      // ver si es la unica operacion que se hace antes de encolar a ready
      if (memoria_iniciar_proceso(pcb->pid, pcb->executable))
      {
         pasar_a_exit(pcb, -1); // no hay motivo de error por no poder iniciar un proceso
         continue;
      }

      if (pcb->motivo_finalizacion == INTERRUPTED_BY_USER)
      {
         pcb = remove_proceso(cola_new, pcb->pid);
         pasar_a_exit(pcb, INTERRUPTED_BY_USER);
         continue;
      }

      sem_mp_wait(grado_multiprogramacion);
      pcb = remove_proceso(cola_new, pcb->pid);
      push_proceso(cola_ready, pcb);
      log_ingreso_a_ready(get_pids(cola_ready), NORMAL);
   }

   return NULL;
}

static void *finalizar_proceso()
{
   while (1)
   {
      t_pcb *pcb = pop_proceso(cola_exit);

      memoria_finalizar_proceso(pcb->pid);
      liberar_recursos(pcb);
      log_finalizacion_proceso(pcb->pid, pcb->motivo_finalizacion);

      quitar_proceso_por_pid(pcb->pid); // a chequear
      destruir_pcb(pcb);
   }

   return NULL;
}

static void liberar_recursos(t_pcb *proceso)
{
   // se asume que la cantidad de recursos es fija,
   // es decir, la cola de recursos siempre coincide
   // con los recursos que figuran en la configuración
   t_dictionary *recursos = get_recursos();
   t_list *nombres_recursos = dictionary_keys(recursos);
   // dictionary_destroy_and_destroy_elements(recursos, &free);

   t_list_iterator *iterador = list_iterator_create(nombres_recursos);

   // es un asco el doble while,
   // pero es la solución más corta y rápida por el momento

   while (list_iterator_has_next(iterador))
   {
      char *nombre_recurso = list_iterator_next(iterador);
      while (liberar_recurso(cola_blocked_recursos, nombre_recurso, proceso->pid) == RELEASED)
      {
         t_pcb *desbloqueado = desbloquear_encolado(cola_blocked_recursos, nombre_recurso);
         if (desbloqueado != NULL)
            pasar_a_ready_segun_prioridad(desbloqueado);
      }
   }

   list_iterator_destroy(iterador);
   list_destroy_and_destroy_elements(nombres_recursos, &free);
}

static void pasar_a_ready_segun_prioridad(t_pcb *proceso)
{
   q_estado *ready = proceso->priority == 0
                         ? cola_ready
                         : cola_ready_prioridad;
   push_proceso(ready, proceso);

   // el valor de proceso->priority coincide con el tipo_cola_ready
   // 0 implica cola ready normal, 1 implica cola ready prioridad
   log_ingreso_a_ready(get_pids(ready), proceso->priority);
}

static void pasar_a_exit(t_pcb *pcb, motivo_finalizacion motivo)
{
   set_motivo_finalizacion(pcb, motivo);
   push_proceso(cola_exit, pcb);
   sem_mp_post(grado_multiprogramacion);
}

static void pasar_a_siguiente(t_pcb *pcb)
{
   switch (pcb->motivo_desalojo)
   {
   case QUANTUM:
      log_fin_de_quantum(pcb->pid);
      pasar_a_ready_segun_prioridad(pcb);
      break;
   case IO:
      if (bloquear_para_io(cola_blocked_interfaces, pcb))
         pasar_a_exit(pcb, INVALID_INTERFACE);
      log_motivo_bloqueo(pcb->pid, INTERFAZ, pcb->io_request->interface_name);
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
   case ERROR:
      pasar_a_exit(pcb, pcb->motivo_finalizacion);
      break;
   case KILL:
      pasar_a_exit(pcb, INTERRUPTED_BY_USER);
      break;
   default: // no debería llegar aca nunca (caso NONE)
      break;
   }
}

static void manejar_wait(t_pcb *pcb)
{
   respuesta_solicitud respuesta = consumir_recurso(cola_blocked_recursos, pcb->resource, pcb);

   switch (respuesta)
   {
   case INVALID:
      pasar_a_exit(pcb, INVALID_RESOURCE);
      break;
   case QUEUED:
      log_motivo_bloqueo(pcb->pid, RECURSO, pcb->resource);
      break;
   case ASSIGNED:
      pasar_a_ready_segun_prioridad(pcb);
      break;
   default: // no debería llegar aca nunca (caso RELEASED)
      break;
   }
}

static void manejar_signal(t_pcb *pcb)
{
   respuesta_solicitud respuesta = liberar_recurso(cola_blocked_recursos, pcb->resource, pcb->pid);

   switch (respuesta)
   {
   case INVALID:
      pasar_a_exit(pcb, INVALID_RESOURCE);
      break;
   case RELEASED:
      t_pcb *proceso = desbloquear_encolado(cola_blocked_recursos, pcb->resource);
      if (proceso != NULL)
         pasar_a_ready_segun_prioridad(proceso);
      break;
   default: // no debería llegar aca nunca (caso ASSIGNED, QUEUED)
      break;
   }
}

static void *planificar_por_fifo()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
      set_motivo_desalojo(proceso, NONE);
      push_proceso(cola_exec, proceso);

      proceso = peek_proceso(cola_exec);
      enviar_pcb_cpu(proceso);
      t_pcb *pos_exec = recibir_pcb_cpu();

      actualizar_pcb(proceso, pos_exec);
      destruir_pcb(pos_exec);

      proceso = remove_proceso(cola_exec, proceso->pid);
      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *planificar_por_rr()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
      set_motivo_desalojo(proceso, NONE);
      push_proceso(cola_exec, proceso);

      proceso = peek_proceso(cola_exec);
      enviar_pcb_cpu(proceso);

      pthread_t rutina_cronometro;
      pthread_create(&rutina_cronometro, NULL, &cronometrar_quantum, &(quantum));
      pthread_detach(rutina_cronometro);

      t_pcb *pos_exec = recibir_pcb_cpu();
      pthread_cancel(rutina_cronometro);

      actualizar_pcb(proceso, pos_exec);
      destruir_pcb(pos_exec);

      proceso = remove_proceso(cola_exec, proceso->pid);
      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *planificar_por_vrr()
{
   t_temporal *temporal = NULL;

   while (1)
   {
      q_estado *ready = hay_proceso(cola_ready_prioridad)
                            ? cola_ready_prioridad
                            : cola_ready;

      t_pcb *proceso = pop_proceso(ready);
      set_motivo_desalojo(proceso, NONE);
      push_proceso(cola_exec, proceso);

      proceso = peek_proceso(cola_exec);
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

      actualizar_pcb(proceso, pos_exec);
      destruir_pcb(pos_exec);

      motivo_desalojo motivo = proceso->motivo_desalojo;

      u_int32_t quantum_nuevo = motivo == QUANTUM
                                    ? quantum
                                    : quantum - transcurrido;
      set_quantum_pcb(proceso, quantum_nuevo);

      int8_t prioridad = motivo == QUANTUM ? 0 : 1;
      set_prioridad(proceso, prioridad);

      proceso = remove_proceso(cola_exec, proceso->pid);
      pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *cronometrar_quantum(void *milisegundos)
{
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
   pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

   u_int64_t _milisegundos = *(u_int32_t *)milisegundos;
   for (u_int64_t i = 0; i < _milisegundos; i++)
   {
      usleep(1000);
      pthread_testcancel();
   }

   enviar_interrupcion(QUANTUM_INT);
   log_envio_de_interrupcion(QUANTUM_INT);
   return NULL;
}
