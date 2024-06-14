#include "interrupcion.h"

int8_t _hay_interrupcion;
int32_t interrupcion;
pthread_mutex_t interrupcion_mutex;

void inicializar_interrupcion()
{
   _hay_interrupcion = 0;
   interrupcion = -1;
   pthread_mutex_init(&interrupcion_mutex, NULL);
}

void set_interrupcion(int32_t tipo)
{
   pthread_mutex_lock(&interrupcion_mutex);
   _hay_interrupcion = 1;
   interrupcion = tipo;
   pthread_mutex_unlock(&interrupcion_mutex);
   
}

void reset_interrupcion()
{
   pthread_mutex_lock(&interrupcion_mutex);
   _hay_interrupcion = 0;
   pthread_mutex_unlock(&interrupcion_mutex);
}

int8_t hay_interrupcion()
{
   int8_t hay_interrupt;
   pthread_mutex_lock(&interrupcion_mutex);
   hay_interrupt = _hay_interrupcion;
   pthread_mutex_unlock(&interrupcion_mutex);
   return hay_interrupt;
}

int32_t tipo_interrupcion()
{
   pthread_mutex_lock(&interrupcion_mutex);
   int32_t interruption = interrupcion;
   pthread_mutex_unlock(&interrupcion_mutex);
   return interruption;
}
