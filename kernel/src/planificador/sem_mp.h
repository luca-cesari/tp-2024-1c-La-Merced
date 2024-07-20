#ifndef PLANIFICADOR_SEM_MP_H
#define PLANIFICADOR_SEM_MP_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
   sem_t sem;
   u_int32_t grado_actual;
} sem_mp_t;

sem_mp_t *sem_mp_create(u_int32_t grado_inicial);
void sem_mp_wait(sem_mp_t *sem);
void sem_mp_post(sem_mp_t *sem);
void sem_mp_set(sem_mp_t *sem, u_int32_t nuevo_grado);
void sem_mp_destroy(sem_mp_t *sem);

#endif // PLANIFICADOR_SEM_MP_H