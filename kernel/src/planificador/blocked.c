#include "blocked.h"

q_blocked *crear_estado_blocked()
{
   q_blocked *estado = malloc(sizeof(q_blocked));

   estado->io_queues = list_create();
   pthread_mutex_init(&(estado->io_queues_mutex), NULL);

   return estado;
}

io_queue *crear_io_queue(char *nombre_interfaz, int32_t fd_conexion)
{
   io_queue *io = malloc(sizeof(io_queue));

   io->nombre_interfaz = nombre_interfaz;
   io->fd_conexion = fd_conexion;
   io->cola = crear_estado(BLOCKED);
   // io->rutina_consumo = 0;

   return io;
}

void conectar_nueva_interfaz(q_blocked *estado, io_queue *io, void *(*rutina_consumo)(void *))
{
   pthread_mutex_lock(&(estado->io_queues_mutex));
   list_add(estado->io_queues, io);
   pthread_mutex_unlock(&(estado->io_queues_mutex));

   pthread_create(&(io->rutina_consumo), NULL, rutina_consumo, io);
   pthread_detach(io->rutina_consumo);
}

int32_t bloquear_proceso(q_blocked *estado, t_pcb *pcb)
{
   int _es_interfaz_buscada(void *elemento)
   {
      io_queue *io = (io_queue *)elemento;
      return strcmp(io->nombre_interfaz, pcb->io_request->inteface_name) == 0;
   };

   pthread_mutex_lock(&(estado->io_queues_mutex));
   io_queue *io_encontrado = list_find(estado->io_queues, (void *)_es_interfaz_buscada);
   pthread_mutex_unlock(&(estado->io_queues_mutex));

   // hace falta verificar de nuevo
   // porque supuestamente siempre itera hasta el ultimo
   // nunca es NULL
   if (io_encontrado == NULL)
      return -1;

   push_proceso(io_encontrado->cola, pcb);

   return 0;
}

q_estado *desconectar_interfaz(q_blocked *estado, int32_t fd_conexion)
{
   int32_t a_remover = 0;

   pthread_mutex_lock(&(estado->io_queues_mutex));
   io_queue *io = NULL;
   t_list_iterator *iterador = list_iterator_create(estado->io_queues);
   while (list_iterator_has_next(iterador))
   {
      io = list_iterator_next(iterador);
      if (io->fd_conexion == fd_conexion)
      {
         a_remover = list_iterator_index(iterador);
         break;
      }
   }
   list_iterator_destroy(iterador);
   pthread_mutex_unlock(&(estado->io_queues_mutex));

   pthread_mutex_lock(&(estado->io_queues_mutex));
   io_queue *cola_io = list_remove(estado->io_queues, a_remover);
   pthread_mutex_unlock(&(estado->io_queues_mutex));

   q_estado *cola = cola_io->cola;
   free(cola_io); // esta bien esto?? no quiero usar destruir_io_queue porque no quiero borrar la cola

   return cola;
}

void destruir_estado_blocked(q_blocked *estado)
{
   pthread_mutex_lock(&(estado->io_queues_mutex));
   list_destroy_and_destroy_elements(estado->io_queues, &destruir_io_queue);
   pthread_mutex_unlock(&(estado->io_queues_mutex));

   pthread_mutex_destroy(&(estado->io_queues_mutex));
   free(estado);
}

void destruir_io_queue(void *elemento)
{
   io_queue *io = (io_queue *)elemento;

   free(io->nombre_interfaz);
   destruir_estado(io->cola);

   free(io);
}