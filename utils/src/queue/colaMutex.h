#include <stdlib.h>
#include <commons/collections/queue.h>
#include <pthread.h>

typedef struct
{
    t_queue *cola;
    pthread_mutex_t *mutex;
} t_queue_mutex;

t_queue_mutex *crear_cola_mutex(void);
void destruir_queue_mutex(t_queue_mutex *);
void pop_queue_mutex(t_queue_mutex *);
void push_queue_mutex(t_queue_mutex *);
void peek_queue_mutex(t_queue_mutex *);