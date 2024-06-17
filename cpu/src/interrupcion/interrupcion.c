#include "interrupcion.h"

int32_t interrupcion;
pthread_mutex_t interrupcion_mutex;

void inicializar_interrupcion()
{
   interrupcion = -1;
   pthread_mutex_init(&interrupcion_mutex, NULL);
}

void set_interrupcion(int32_t tipo)
{
   pthread_mutex_lock(&interrupcion_mutex);
   interrupcion = tipo;
   pthread_mutex_unlock(&interrupcion_mutex);
}

void reset_interrupcion()
{
   pthread_mutex_lock(&interrupcion_mutex);
   interrupcion = -1;
   pthread_mutex_unlock(&interrupcion_mutex);
}

int8_t get_interrupcion()
{
   int8_t _interrupt;
   pthread_mutex_lock(&interrupcion_mutex);
   _interrupt = interrupcion;
   pthread_mutex_unlock(&interrupcion_mutex);
   return _interrupt;
}
