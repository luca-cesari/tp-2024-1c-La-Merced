#include "mlist.h"

t_mutext_list *mlist_create(void)
{
   t_mutext_list *lista_mutex = malloc(sizeof(t_mutext_list));
   lista_mutex->list = list_create();
   pthread_mutex_init(&(lista_mutex->mutex), NULL);
   return lista_mutex;
}

void mlist_add(t_mutext_list *lista_mutex, void *elemento)
{
   pthread_mutex_lock(&(lista_mutex->mutex));
   list_add(lista_mutex->list, elemento);
   pthread_mutex_unlock(&(lista_mutex->mutex));
}

void *mlist_peek(t_mutext_list *lista_mutex)
{
   pthread_mutex_lock(&(lista_mutex->mutex));
   void *elemento = list_get(lista_mutex->list, 0);
   pthread_mutex_unlock(&(lista_mutex->mutex));
   return elemento;
}

void *mlist_pop_as_queue(t_mutext_list *lista_mutex)
{
   return mlist_remove(lista_mutex, 0);
}

void *mlist_remove(t_mutext_list *lista_mutex, u_int32_t index)
{
   pthread_mutex_lock(&(lista_mutex->mutex));
   void *elemento = list_remove(lista_mutex->list, index);
   pthread_mutex_unlock(&(lista_mutex->mutex));
   return elemento;
}

void mlist_destroy(t_mutext_list *lista_mutex)
{
   list_destroy(lista_mutex->list);
   pthread_mutex_destroy(&(lista_mutex->mutex));
   free(lista_mutex);
}