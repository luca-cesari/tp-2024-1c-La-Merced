#include "planificador.h"

u_int32_t pid_count;
u_int32_t quantum;

t_mutext_list *lista_procesos;

sem_t grado_multiprogramacion;

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
   pid_count = 0;
   quantum = get_quantum();

   sem_init(&grado_multiprogramacion, 0, get_grado_multiprogramacion());

   cola_new = crear_estado(NEW);
   cola_ready = crear_estado(READY);
   cola_ready_prioridad = crear_estado(READY);
   cola_exec = crear_estado(EXEC);
   cola_exit = crear_estado(EXIT);
   cola_blocked_interfaces = crear_estado_blocked();
   cola_blocked_recursos = crear_estado_blocked();
   lista_procesos = mlist_create();

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
   sem_destroy(&grado_multiprogramacion);

   destruir_estado(cola_new);
   destruir_estado(cola_ready);
   destruir_estado(cola_ready_prioridad);
   destruir_estado(cola_exec);
   destruir_estado(cola_exit);
   destruir_estado_blocked(cola_blocked_interfaces, &destruir_io_queue);
   destruir_estado_blocked(cola_blocked_recursos, &destruir_resource_queue);
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
   // TODO
}

void crear_proceso(char *ruta_ejecutable)
{
   t_pcb *pcb = crear_pcb(pid_count++, ruta_ejecutable);
   set_quantum_pcb(pcb, quantum);
   push_proceso(cola_new, pcb);
   log_creacion_proceso(pcb->pid);
   t_proceso *proceso = crear_estructura_proceso(pid_count++, pcb);
   mlist_add(lista_procesos, proceso);
}

void matar_proceso(u_int32_t pid)
{
   t_proceso *proceso_buscado = mlist_find(lista_procesos, pid);
   t_pcb *pcb = proceso_buscado->pcb;
   switch (pcb->estado)
   {
   case NEW:
      proceso_buscado->pcb = remove_proceso(cola_new, pid);
      break;
   case READY:
      proceso_buscado->pcb = remove_proceso(cola_ready, pid);
      break;
   case BLOCKED:
      if (proceso_buscado->pcb->motivo_desalojo == WAIT)
      {
         proceso_buscado->pcb = remove_proceso(cola_blocked_recursos, pid);
      }
      if (proceso_buscado->pcb->motivo_desalojo == IO)
      {
         proceso_buscado->pcb = remove_proceso(cola_blocked_interfaces, pid);
      }
      break;
   case EXEC:
      enviar_interrupcion();
      break;
   default:
      return;
   }
   pasar_a_exit(proceso_buscado->pcb, INTERRUPTED_BY_USER);
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
      enviar_io_request(interfaz->fd_conexion, pcb->io_request);
      int32_t response = recibir_senial(interfaz->fd_conexion);

      // se resetea el campo io_req
      // principalmente para EXECUTED,
      // así no afectaría cuando vuelva a la CPU.
      // en otros casos no debería ser relevante
      t_io_request *empty_io_req = crear_io_request(pcb->pid, "", "", "");
      set_io_request(pcb, empty_io_req);

      pcb = remove_proceso(interfaz->cola_procesos, pcb->pid);
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

      sem_wait(&grado_multiprogramacion);
      // La razón por la que se hace un remove y la validación
      // es por si el proceso fue interrumpido por usuario
      // durante la espera de la memoria.
      // véase matar_proceso()
      pcb = remove_proceso(cola_new, pcb->pid);
      if (pcb == NULL)
         continue;
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
      destruir_pcb(pcb);
   }

   return NULL;
}

static void liberar_recursos(t_pcb *proceso)
{
   // se asume que la cantidad de recursos es fija,
   // es decir, la cola de recursos siempre coincide
   // con los recursos que figuran en la configuración
   t_list *nombres_recursos = dictionary_keys(get_recursos());
   t_list_iterator *iterador = list_iterator_create(nombres_recursos);

   // es un asco el doble while,
   // pero es la solución más corta y rápida por el momento

   while (list_iterator_has_next(iterador))
   {
      char *nombre_recurso = list_iterator_next(iterador);
      while (liberar_recurso(cola_blocked_recursos, proceso->pid, nombre_recurso) == RELEASED)
      {
         t_pcb *desbloqueado = desbloquear_para_recurso(cola_blocked_recursos, nombre_recurso);
         if (proceso == NULL)
            continue;
         pasar_a_ready_segun_prioridad(desbloqueado);
      }
   }

   list_iterator_destroy(iterador);
   list_clean_and_destroy_elements(nombres_recursos, &free);
}

static void pasar_a_ready_segun_prioridad(t_pcb *proceso)
{
   q_estado *ready = proceso->priority == 0 ? cola_ready : cola_ready_prioridad;
   push_proceso(ready, proceso);

   // el valor de proceso->priority coincide con el tipo_cola_ready
   // 0 implica cola ready normal, 1 implica cola ready prioridad
   log_ingreso_a_ready(get_pids(ready), proceso->priority);
}

static void pasar_a_exit(t_pcb *pcb, motivo_finalizacion motivo)
{
   set_motivo_finalizacion(pcb, motivo);
   push_proceso(cola_exit, pcb);
   sem_post(&grado_multiprogramacion);
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
   }
}

static void manejar_wait(t_pcb *pcb)
{
   respuesta_solicitud respuesta = consumir_recurso(cola_blocked_recursos, pcb->pid, pcb->resource);

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
      pasar_a_ready_segun_prioridad(pcb);
      break;
   default: // no debería llegar aca nunca (caso RELEASED)
      break;
   }

   set_recurso_pcb(pcb, ""); // resetea el campo resource
}

static void manejar_signal(t_pcb *pcb)
{
   respuesta_solicitud respuesta = liberar_recurso(cola_blocked_recursos, pcb->pid, pcb->resource);

   switch (respuesta)
   {
   case INVALID:
      pasar_a_exit(pcb, INVALID_RESOURCE);
      break;
   case RELEASED:
      t_pcb *proceso = desbloquear_para_recurso(cola_blocked_recursos, pcb->resource);
      if (proceso != NULL)
         pasar_a_ready_segun_prioridad(proceso);
      break;
   default: // no debería llegar aca nunca (caso ASSIGNED, ALL_RETAINED)
      break;
   }

   set_recurso_pcb(pcb, ""); // resetea el campo resource
}

static void *planificar_por_fifo()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
      push_proceso(cola_exec, proceso);

      proceso = peek_proceso(cola_exec);
      enviar_pcb_cpu(proceso);
      t_pcb *pos_exec = recibir_pcb_cpu();

      // revisar función, capaz no hay que asignar todo
      // se tendría que evitar modificar el estado del proceso
      // si durante la ejecución se interrumpe por usuario, se
      // seteará el motivo de desalojo y el estado; por lo que
      // no se tenría que sobreescribir esos campos.
      actualizar_pcb(proceso, pos_exec);
      destruir_pcb(pos_exec);

      proceso = remove_proceso(cola_exec, proceso->pid);
      if (proceso != NULL)
         pasar_a_siguiente(proceso);
   }

   return NULL;
}

static void *planificar_por_rr()
{
   while (1)
   {
      t_pcb *proceso = pop_proceso(cola_ready);
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
      if (proceso != NULL)
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

      u_int32_t quantum_proceso_nuevo = transcurrido < quantum ? quantum - transcurrido : quantum;
      set_quantum_pcb(proceso, quantum_proceso_nuevo);

      int8_t prioridad = transcurrido < quantum ? 1 : 0;
      set_prioridad(proceso, prioridad);

      proceso = remove_proceso(cola_exec, proceso->pid);
      if (proceso != NULL)
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
