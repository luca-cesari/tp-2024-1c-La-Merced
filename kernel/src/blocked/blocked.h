#ifndef BLOCKED_H
#define BLOCKED_H

#include <mlist/mlist.h>

typedef struct
{
   t_mutext_list *lista_colas;
} q_blocked;

q_blocked *crear_estado_blocked(void);
void destruir_estado_blocked(q_blocked *estado, void (*destructor)(void *));

#endif // BLOCKED_H
