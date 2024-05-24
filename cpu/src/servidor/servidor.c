#include "servidor.h"

//Defino variable compartida entre ambos hilos para saber si se debe interrumpir la ejecucion
int hay_interrupcion = 0;
pthread_mutex_t mutexInterrupcion;


void iniciar_servidor()
{
  pthread_mutex_init(&mutexInterrupcion, NULL);

  char *puerto_escucha_dispatch = get_puerto_escucha_dispatch();
  char *puerto_escucha_interrupt = get_puerto_escucha_interrupt();

  int32_t *fd_dispatch = malloc(sizeof(int32_t));
  int32_t *fd_interrupt = malloc(sizeof(int32_t));

  *fd_dispatch = crear_servidor(puerto_escucha_dispatch);
  *fd_interrupt = crear_servidor(puerto_escucha_interrupt);


  pthread_t hilo_dispatch, hilo_interrupt;
  pthread_create(&hilo_dispatch, NULL, &escuchar_dispatch, fd_dispatch);
  pthread_create(&hilo_interrupt, NULL, &escuchar_interrupt, fd_interrupt);
  pthread_detach(hilo_dispatch);
  pthread_detach(hilo_interrupt);
}

void *atender_kernel_dispatch(void *fd_ptr)
{

  int32_t fd_dispatch = *((int32_t *)fd_ptr);
  uint32_t modulo_cliente = recibir_cliente(fd_dispatch);
  if (modulo_cliente != KERNEL)
  {
    return NULL;
  }
  //...
  return NULL;
}
void *atender_kernel_interrupt(void *fd_ptr)
{

  int32_t fd_interrupt = *((int32_t *)fd_ptr);
  uint32_t modulo_cliente = recibir_cliente(fd_interrupt);
  if (modulo_cliente != KERNEL)
  {
    return NULL;
  }
  recibir_interrupcion_del_kernel(fd_interrupt);

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

void recibir_interrupcion_del_kernel(int32_t fd_interrupt)
{
  if (recibir_senial(fd_interrupt) == 1) //Se recibe que hubo una interrupcion desde el kernel
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
