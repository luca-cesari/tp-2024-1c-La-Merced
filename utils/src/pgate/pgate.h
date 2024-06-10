#ifndef PGATE_H
#define PGATE_H

#include <stdlib.h>
#include <semaphore.h>

typedef enum
{
   OPEN,
   CLOSED
} gate_state;

typedef struct
{
   sem_t *semaphore;
   gate_state state;
} pgate_t;

pgate_t *pgate_create(void);
void set_gate(pgate_t *pgate);
void close_gate(pgate_t *pgate);
void open_gate(pgate_t *pgate);
void pgate_destroy(pgate_t *pgate);

#endif // PGATE_H