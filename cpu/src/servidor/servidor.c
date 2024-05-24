#include "servidor.h"

void iniciar_servidor()
{

  char *puerto_escucha_dispatch = get_puerto_escucha_dispatch();
  char *puerto_escucha_interrupt = get_puerto_escucha_interrupt();

  int32_t *fd_dispatch = malloc(sizeof(int32_t));
  int32_t *fd_interrupt = malloc(sizeof(int32_t));

  *fd_dispatch = crear_servidor(puerto_escucha_dispatch);
  *fd_dispatch = crear_servidor(puerto_escucha_interrupt);

  pthread_t hilo_dispatch, hilo_interrupt;
  pthread_create(&hilo_dispatch, NULL, &escuchar_conexiones, fd_dispatch);
  pthread_create(&hilo_interrupt, NULL, &escuchar_conexiones, fd_interrupt);
  pthread_detach(hilo_dispatch);
  pthread_detach(hilo_interrupt);
}

void *atender_kernel(void *fd_ptr)
{
  int32_t fd_conexion = *((int32_t *)fd_ptr);

  if (recibir_cliente(fd_conexion) != KERNEL)
  {
    printf("Error de Cliente \n");
    return NULL;
  }

  if (fd_conexion == *fd_dispatch)
  {
    printf("Kernel conectado por Dispatch \n");
    while (1)
    {
      recibir_mensaje(fd_dispatch);
    }
  }
  else
  {
    printf("Kernel conectado por Interrupt \n");
    while (1)
    {
      recibir_mensaje(fd_interrupt);
    }
  }
  return NULL;
}

void *escuchar_dispatch(void *fd_dispatch)
{
  while (1)
  {
    esperar_cliente(*((int32_t *)fd_dispatch), &escuchar_dispatch);
  }
  return NULL;
}
void *escuchar_interrupt(void *fd_interrupt)
{

  while (1)
  {
    esperar_cliente(*((int32_t *)fd_interrupt), &escuchar_interrupt);
  }

  return NULL;
}
