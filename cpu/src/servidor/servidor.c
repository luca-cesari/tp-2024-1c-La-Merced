#include "servidor.h"

int32_t fd_dispatch;
int32_t fd_interrupt;

sem_t fin_de_proceso;

void iniciar_servidor()
{
   sem_init(&fin_de_proceso, 0, 0);

   char *puerto_escucha_dispatch = get_puerto_escucha_dispatch();
   char *puerto_escucha_interrupt = get_puerto_escucha_interrupt();

   fd_dispatch = crear_servidor(puerto_escucha_dispatch);
   fd_interrupt = crear_servidor(puerto_escucha_interrupt);

   esperar_cliente(fd_dispatch, &atender_kernel_dispatch);
   esperar_cliente(fd_interrupt, &atender_kernel_interrupt);

   sem_wait(&fin_de_proceso);
}

void *atender_kernel_dispatch(void *fd_ptr)
{
   int32_t fd_dispatch = *((int32_t *)fd_ptr);
   uint32_t modulo_cliente = recibir_cliente(fd_dispatch);
   if (modulo_cliente != KERNEL)
   {
      sem_post(&fin_de_proceso);
      return NULL;
   }

   log_evento("Kernel conectado por dispatch");

   while (1)
   {
      t_pcb *pcb = recibir_pcb(fd_dispatch);
      if (pcb == NULL)
         break;

      reset_interrupcion();
      ciclo_instruccion(pcb);
      enviar_pcb(fd_dispatch, pcb);
      destruir_pcb(pcb);
   }

   sem_post(&fin_de_proceso);
   return NULL;
}

void *atender_kernel_interrupt(void *fd_ptr)
{

   int32_t fd_interrupt = *((int32_t *)fd_ptr);
   uint32_t modulo_cliente = recibir_cliente(fd_interrupt);
   if (modulo_cliente != KERNEL)
   {
      liberar_conexion(fd_interrupt);
      sem_post(&fin_de_proceso);
      return NULL;
   }

   log_evento("Kernel conectado por interrupt");

   while (1)
   {
      int32_t interrupcion = recibir_senial(fd_interrupt);
      if (interrupcion == -1)
         break;
      if (interrupcion == 0 || interrupcion == 1)
         set_interrupcion(interrupcion);
   }

   sem_post(&fin_de_proceso);
   return NULL;
}

void finalizar_servidor()
{
   sem_destroy(&fin_de_proceso);
   liberar_conexion(fd_dispatch);
   liberar_conexion(fd_interrupt);
}
