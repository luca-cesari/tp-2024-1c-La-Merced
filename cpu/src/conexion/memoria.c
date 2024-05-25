#include "memoria.h"

int32_t fd_memoria;

int8_t conectar_con_memoria()
{
   mem_config mem = get_memoria_config();

   fd_memoria = crear_conexion(mem.ip, mem.puerto);
   int32_t response = handshake(fd_memoria, CPU);
   if (response == -1)
   {
      liberar_conexion(fd_memoria);
      return -1;
   }

   return 0;
}
void enviar_pcb_memoria(t_pcb *pcb)
{
   enviar_pcb(fd_memoria, pcb);
}
//
char *recibir_instruccion(int32_t fd_memoria)
{
   char *instruccion = recibir_mensaje(fd_memoria);

   return instruccion;
}

void liberar_conexion_memoria()
{
   liberar_conexion(fd_memoria);
}