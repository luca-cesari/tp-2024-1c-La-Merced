#include "logger.h"

t_log *cpu_logger;

void iniciar_logger(void)
{
   cpu_logger = log_create(LOG_FILE, "cpu", true, LOG_LEVEL_INFO);
}

void destruir_logger(void)
{
   log_destroy(cpu_logger);
}

void log_evento(char *evento)
{
   log_info(cpu_logger, "%s", evento);
}

void log_fetch_instruccion(u_int32_t pid, u_int32_t pc)
{
   log_info(cpu_logger, "PID: %d - FETCH - Program Counter: %d", pid, pc);
}

void log_instruccion_ejecutada(u_int32_t pid, char *instruccion, char *parametros)
{
   log_info(cpu_logger, "PID: %d - Ejecutando: %s - %s", pid, instruccion, parametros);
}

void log_tlb_hit(u_int32_t pid, int numero_pagina)
{
   log_info(cpu_logger, "PID: %d - TLB HIT - Pagina: %d", pid, numero_pagina);
}

void log_tlb_miss(u_int32_t pid, int numero_pagina) // ver si se abstrae con el hit
{
   log_info(cpu_logger, "PID: %d - TLB MISS - Pagina: %d", pid, numero_pagina);
}

void log_reemplazo_de_entrada()
{
   log_info(cpu_logger, "Reemplazo de entrada en TLB");
}

void log_obtener_marco(u_int32_t pid, int numero_pagina, int numero_marco)
{
   log_info(cpu_logger, "PID: %d - OBTENER MARCO - Página: %d - Marco: %d", pid, numero_pagina, numero_marco);
}

void log_escritura_lectura_memoria(u_int32_t pid, accion accion, u_int32_t direccion_fisica, char *valor)
{
   switch (accion)
   {
   case READ:
      log_info(cpu_logger, "PID: %d - Acción: LEER - Dirección Física: %d - Valor: %s", pid, direccion_fisica, valor);
      return;
   case WRITE:
      log_info(cpu_logger, "PID: %d - Acción: ESCRIBIR - Dirección Física: %d - Valor: %s", pid, direccion_fisica, valor);
      return;
   default:
      return;
   }
}