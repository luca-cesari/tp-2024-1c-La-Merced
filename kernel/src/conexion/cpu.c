#include "cpu.h"

int32_t fd_dispatch;
int32_t fd_interrupt;

int8_t conectar_por_dispatch()
{
   cpu_config cpu = get_cpu_config();

   fd_dispatch = crear_conexion(cpu.ip, cpu.puerto_dispatch);
   int32_t response = handshake(fd_dispatch, KERNEL);
   if (response == -1)
   {
      liberar_conexion(fd_dispatch);
      return -1;
   }

   return 0;
}

int8_t conectar_por_interrupt()
{
   cpu_config cpu = get_cpu_config();

   fd_interrupt = crear_conexion(cpu.ip, cpu.puerto_interrupt);
   int32_t response = handshake(fd_interrupt, KERNEL);
   if (response == -1)
   {
      liberar_conexion(fd_interrupt);
      return -1;
   }

   return 0;
}

void enviar_pcb(t_pcb *pcb)
{
   // TODO: Implementar
}

t_pcb *recibir_pcb()
{
   // TODO: Implementar

   return NULL;
}

void liberar_conexion_cpu()
{
   liberar_conexion(fd_dispatch);
   liberar_conexion(fd_interrupt);
}