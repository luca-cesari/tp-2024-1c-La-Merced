#ifndef UTILS_MLIST_H
#define UTILS_MLIST_H

#include <stdlib.h>
#include <commons/collections/list.h>
#include <pthread.h>

typedef struct
{
   t_list *list;
   pthread_mutex_t mutex;
} t_mutext_list;

t_mutext_list *mlist_create(void);
void mlist_add(t_mutext_list *, void *);
void *mlist_peek(t_mutext_list *);
void *mlist_pop_as_queue(t_mutext_list *);
void *mlist_remove(t_mutext_list *, u_int32_t);
void mlist_destroy(t_mutext_list *);

#endif // UTILS_MLIST_H