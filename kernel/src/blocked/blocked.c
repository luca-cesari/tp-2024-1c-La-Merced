#include "blocked.h"

q_blocked *crear_estado_blocked(void)
{
   q_blocked *estado = malloc(sizeof(q_blocked));
   estado->lista_colas = mlist_create();
   return estado;
}

void destruir_estado_blocked(q_blocked *estado, void (*destructor)(void *))
{
   mlist_clean(estado->lista_colas, destructor);
   mlist_destroy(estado->lista_colas);
   free(estado);
}