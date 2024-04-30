#include "logger.h"

t_log *kernel_logger;

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
   case INVALID_WRITE:
      motivo_str = "INVALID_WRITE";
      break;
   default:
      motivo_str = "UNKNOWN";
      break;
   }

   log_info(kernel_logger, "Finaliza el proceso %d - Motivo: %s", pid, motivo_str);
}

void log_cambio_de_estado(u_int32_t pid, char *estado_anterior, char *estado_actual)
{
   log_info(kernel_logger, "PID: %d - Estado Anterior: %s - Estado Actual: %s", pid, estado_anterior, estado_actual);
}

void log_motivo_bloqueo(u_int32_t pid, motivo_bloqueo motivo, char *recurso)
{
   switch (motivo)
   {
   case INTERFAZ:
      log_info(kernel_logger, "PID: %d - Bloqueado por: INTERFAZ", pid);
      return;
   case RECURSO:
      log_info(kernel_logger, "PID: %d - Bloqueado por: %s", pid, recurso);
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