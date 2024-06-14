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

void enviar_pcb_cpu(t_pcb *pcb)
{
   enviar_pcb(fd_dispatch, pcb);
}

t_pcb *recibir_pcb_cpu()
{
   t_pcb *pcb = recibir_pcb(fd_dispatch);
   return pcb;
}

void enviar_interrupcion(interrupt_code code)
{
   enviar_senial(code, fd_interrupt);
}

void liberar_conexion_cpu()
{
   liberar_conexion(fd_dispatch);
   liberar_conexion(fd_interrupt);
}