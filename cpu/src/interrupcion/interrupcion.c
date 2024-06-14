#include "interrupcion.h"

int8_t _hay_interrupcion;
pthread_mutex_t hay_interrupcion_mutex;
int32_t interrupcion;

void inicializar_interrupcion()
{
   _hay_interrupcion = 0;
   pthread_mutex_init(&hay_interrupcion_mutex, NULL);
}

void set_interrupcion(int32_t tipo)
{
   pthread_mutex_lock(&hay_interrupcion_mutex);
   _hay_interrupcion = 1;
   interrupcion = tipo;
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

int32_t tipo_interrupcion()
{
   return interrupcion;
}
