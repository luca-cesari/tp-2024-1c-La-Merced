#ifndef UTILS_MQUEUE_H
#define UTILS_MQUEUE_H

#include <stdlib.h>
#include <commons/collections/queue.h>
#include <pthread.h>

typedef struct
{
    t_queue *cola;
    pthread_mutex_t mutex;
} t_mutex_queue;

t_mutex_queue *create_mutex_queue(void);
void *pop_mutex_queue(t_mutex_queue *);
void *peek_mutex_queue(t_mutex_queue *);
void push_mutex_queue(t_mutex_queue *, void *);
void destruir_mutex_queue(t_mutex_queue *);

#endif // UTILS_MQUEUE_H