#include "logger.h"

t_log *kernel_logger;

static char *state_to_string(state estado);

void iniciar_logger(void)
{
   kernel_logger = log_create(LOG_FILE, "Kernel", true, LOG_LEVEL_INFO);
}

void destruir_logger(void)
{
   log_destroy(kernel_logger);
}

void log_creacion_proceso(u_int32_t pid)
{
   log_info(kernel_logger, "Se crea el proceso %d en NEW", pid);
}

void log_finalizacion_proceso(u_int32_t pid, motivo_finalizacion motivo)
{
   char *motivo_str;
   switch (motivo)
   {
   case SUCCESS:
      motivo_str = "SUCCESS";
      break;
   case INVALID_RESOURCE:
      motivo_str = "INVALID_RESOURCE";
      break;
   case INVALID_INTERFACE:
      motivo_str = "INVALID_INTERFACE";
      break;
   case INTERRUPTED_BY_USER:
      motivo_str = "INTERRUPTED_BY_USER";
      break;
   case OUT_OF_MEMORY:
      motivo_str = "OUT_OF_MEMORY";
      break;
   default:
      motivo_str = "UNKNOWN";
      break;
   }

   log_info(kernel_logger, "Finaliza el proceso %d - Motivo: %s", pid, motivo_str);
}

void log_cambio_de_estado(u_int32_t pid, state estado_anterior, state estado_actual)
{
   char *estado_anterior_str = state_to_string(estado_anterior);
   char *estado_actual_str = state_to_string(estado_actual);
   log_info(kernel_logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid, estado_anterior_str, estado_actual_str);
}

static char *state_to_string(state estado)
{
   switch (estado)
   {
   case NEW:
      return "NEW";
   case READY:
      return "READY";
   case EXEC:
      return "EXEC";
   case BLOCKED:
      return "BLOCKED";
   case EXIT:
      return "EXIT";
   default:
      return "UNKNOWN";
   }
}

void log_motivo_bloqueo(u_int32_t pid, motivo_bloqueo motivo, char *nombre)
{
   switch (motivo)
   {
   case INTERFAZ:
      log_info(kernel_logger, "PID: %d - Bloqueado por Interfaz: %s", pid, nombre);
      return;
   case RECURSO:
      log_info(kernel_logger, "PID: %d - Bloqueado por Recurso: %s", pid, nombre);
      return;
   default:
      return;
   }
}

void log_fin_de_quantum(u_int32_t pid)
{
   log_info(kernel_logger, "PID: %d - Desalojado por fin de Quantum", pid);
}

void log_ingreso_a_ready(t_list *lista_pid, tipo_cola_ready tipo)
{
   char *pids_str = string_new();

   string_append(&pids_str, "[ ");
   void _agregar_pid_a_string(void *ptr_pid)
   {
      u_int32_t pid = *(u_int32_t *)ptr_pid;
      string_append(&pids_str, string_itoa(pid));
      string_append(&pids_str, " ");
   };
   list_iterate(lista_pid, &_agregar_pid_a_string);
   string_append(&pids_str, "]");

   char *tipo_cola_str = tipo == PRIORIDAD ? "Ready Prioridad" : "Ready";

   log_info(kernel_logger, "Cola %s: %s", tipo_cola_str, pids_str);

   list_destroy_and_destroy_elements(lista_pid, &free);
   free(pids_str);
}
