#include "interrupcion.h"

int8_t _hay_interrupcion;
pthread_mutex_t hay_interrupcion_mutex;

void inicializar_interrupcion()
{
   _hay_interrupcion = 0;
   pthread_mutex_init(&hay_interrupcion_mutex, NULL);
}

void set_interrupcion()
{
   pthread_mutex_lock(&hay_interrupcion_mutex);
   _hay_interrupcion = 1;
   pthread_mutex_unlock(&hay_interrupcion_mutex);
}

void reset_interrupcion()
{
   pthread_mutex_lock(&hay_interrupcion_mutex);
   _hay_interrupcion = 0;
   pthread_mutex_unlock(&hay_interrupcion_mutex);
}

int8_t hay_interrupcion()
{
   int8_t interrupcion;
   pthread_mutex_lock(&hay_interrupcion_mutex);
   interrupcion = _hay_interrupcion;
   pthread_mutex_unlock(&hay_interrupcion_mutex);
   return interrupcion;
}
