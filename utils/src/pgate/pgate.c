#include "pgate.h"

pgate_t *pgate_create(void)
{
   pgate_t *pgate = malloc(sizeof(pgate_t));
   pgate->semaphore = malloc(sizeof(sem_t));
   sem_init(pgate->semaphore, 0, 1);
   pgate->state = OPEN;

   return pgate;
}

void set_gate(pgate_t *pgate)
{
   sem_wait(pgate->semaphore);
   sem_post(pgate->semaphore);
}

void close_gate(pgate_t *pgate)
{
   if (pgate->state == CLOSED)
      return;

   sem_wait(pgate->semaphore);
   pgate->state = CLOSED;
}

void open_gate(pgate_t *pgate)
{
   if (pgate->state == OPEN)
      return;

   sem_post(pgate->semaphore);
   pgate->state = OPEN;
}

void pgate_destroy(pgate_t *pgate)
{
   sem_destroy(pgate->semaphore);
   free(pgate->semaphore);
   free(pgate);
}