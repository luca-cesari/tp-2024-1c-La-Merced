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

void log_ingreso_a_ready(void)
{
   // ...
   log_info(kernel_logger, "Cola Ready <COLA>: [<LISTA DE PIDS>]");
}