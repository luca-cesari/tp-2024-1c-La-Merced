#include "colaMutex.h"

t_queue_mutex *crear_cola_mutex()
{
  t_queue_mutex *cola_mutex = malloc(sizeof(t_queue_mutex));
  cola_mutex->cola = queue_create();
  pthread_mutex_init(&(cola_mutex->mutex), NULL);
  return cola_mutex;
}

void pop_cola_mutex(t_queue_mutex *cola_mutex)
{
  pthread_mutex_lock(&(cola_mutex->mutex));
  void *contenido = queue_pop(cola_mutex->cola);
  pthread_mutex_unlock(&(cola_mutex->mutex));
  return contenido;
}

void push_cola_mutex(t_queue_mutex *cola_mutex, void *contenido)
{
  pthread_mutex_lock(&(cola_mutex->mutex));
  queue_push(cola_mutex->cola, contenido);
  pthread_mutex_unlock(&(cola_mutex->mutex));
}

void destruir_cola_mutex(t_queue_mutex *cola_mutex)
{
  pthread_mutex_lock(&(cola_mutex->mutex));
  queue_destroy(cola_mutex->cola);
  pthread_mutex_unlock(&(cola_mutex->mutex));
  pthread_mutex_destroy(&(cola_mutex->mutex));
  free(cola_mutex);
}

// Ver si se usa
void peek_cola_mutex(t_queue_mutex *cola_mutex)
{
  pthread_mutex_lock(&(cola_mutex->mutex));
  void *contenido = queue_peek(cola_mutex);
  pthread_mutex_unlock(&(cola_mutex->mutex));
  return contenido;
}
