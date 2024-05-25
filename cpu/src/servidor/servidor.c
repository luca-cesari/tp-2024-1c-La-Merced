#include "servidor.h"

// Defino variable compartida entre ambos hilos para saber si se debe interrumpir la ejecucion
int hay_interrupcion = 0;
pthread_mutex_t mutexInterrupcion;

void iniciar_servidor()
{
  pthread_mutex_init(&mutexInterrupcion, NULL);

  char *puerto_escucha_dispatch = get_puerto_escucha_dispatch();
  char *puerto_escucha_interrupt = get_puerto_escucha_interrupt();

  int32_t fd_dispatch = crear_servidor(puerto_escucha_dispatch);
  int32_t fd_interrupt = crear_servidor(puerto_escucha_interrupt);

  esperar_cliente(fd_dispatch, &atender_kernel_dispatch);
  esperar_cliente(fd_interrupt, &atender_kernel_interrupt);
}

void *atender_kernel_dispatch(void *fd_ptr)
{

  int32_t fd_dispatch = *((int32_t *)fd_ptr);
  uint32_t modulo_cliente = recibir_cliente(fd_dispatch);
  if (modulo_cliente != KERNEL)
  {
    liberar_conexion(fd_dispatch);
    return NULL;
  }

  printf("Kernel conectado por dispatch \n");

  while (1)
  {
    t_pcb *pcb = recibir_pcb(fd_dispatch);
    ciclo_instruccion(pcb);
    enviar_pcb(fd_dispatch, pcb);
  }
  return NULL;
}

void *atender_kernel_interrupt(void *fd_ptr)
{

  int32_t fd_interrupt = *((int32_t *)fd_ptr);
  uint32_t modulo_cliente = recibir_cliente(fd_interrupt);
  if (modulo_cliente != KERNEL)
  {
    liberar_conexion(fd_interrupt);
    return NULL;
  }

  printf("Kernel conectado por interrupt \n");

  recibir_interrupcion_del_kernel(fd_interrupt);

  return NULL;
}

void recibir_interrupcion_del_kernel(int32_t fd_interrupt)
{
  if (recibir_senial(fd_interrupt) == 1) // Se recibe que hubo una interrupcion desde el kernel
  {
    interrumpir();
  }
}

void interrumpir()
{
  pthread_mutex_lock(&mutexInterrupcion);
  hay_interrupcion = 1;
  pthread_mutex_unlock(&mutexInterrupcion);
}
