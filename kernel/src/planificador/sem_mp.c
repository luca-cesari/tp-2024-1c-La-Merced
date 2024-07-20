#include "sem_mp.h"

static void incrementar_grado(sem_mp_t *sem, u_int32_t incremento);
static void decrementar_grado(sem_mp_t *sem, u_int32_t decremento);

sem_mp_t *sem_mp_create(u_int32_t grado_inicial)
{
}

void sem_mp_wait(sem_mp_t *sem)
{
}

void sem_mp_post(sem_mp_t *sem)
{
}

void sem_mp_set(sem_mp_t *sem, u_int32_t nuevo_grado)
{
   if (nuevo_grado > sem->grado_actual)
      incrementar_grado(sem, nuevo_grado - sem->grado_actual);

   else if (nuevo_grado < sem->grado_actual)
      decrementar_grado(sem, sem->grado_actual - nuevo_grado);
}

void sem_mp_destroy(sem_mp_t *sem)
{
}

static void incrementar_grado(sem_mp_t *sem, u_int32_t incremento)
{
}

static void decrementar_grado(sem_mp_t *sem, u_int32_t decremento)
{
}