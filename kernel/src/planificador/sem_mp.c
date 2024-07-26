#include "sem_mp.h"

static void incrementar_grado(sem_mp_t *sem_mp, u_int32_t incremento);
static void decrementar_grado(sem_mp_t *sem_mp, u_int32_t decremento);

sem_mp_t *sem_mp_create(u_int32_t grado_inicial)
{
   sem_mp_t *sem_mp = malloc(sizeof(sem_mp_t));
   sem_mp->sem = malloc(sizeof(sem_t));
   sem_init(sem_mp->sem, 0, grado_inicial);
   sem_mp->grado_actual = grado_inicial;
   return sem_mp;
}

void sem_mp_wait(sem_mp_t *sem_mp)
{
   sem_wait(sem_mp->sem);
}

void sem_mp_post(sem_mp_t *sem_mp)
{
   sem_post(sem_mp->sem);
}

void sem_mp_set(sem_mp_t *sem_mp, u_int32_t nuevo_grado)
{
   if (nuevo_grado > sem_mp->grado_actual)
      incrementar_grado(sem_mp, nuevo_grado - sem_mp->grado_actual);

   else if (nuevo_grado < sem_mp->grado_actual)
      decrementar_grado(sem_mp, sem_mp->grado_actual - nuevo_grado);

   sem_mp->grado_actual = nuevo_grado;
}

void sem_mp_destroy(sem_mp_t *sem_mp)
{
   sem_destroy(sem_mp->sem);
   free(sem_mp->sem);
   free(sem_mp);
}

static void incrementar_grado(sem_mp_t *sem_mp, u_int32_t incremento)
{
   for (u_int32_t i = 0; i < incremento; i++)
      sem_post(sem_mp->sem);
}

static void decrementar_grado(sem_mp_t *sem_mp, u_int32_t decremento)
{
   for (u_int32_t i = 0; i < decremento; i++)
      sem_wait(sem_mp->sem);
}
