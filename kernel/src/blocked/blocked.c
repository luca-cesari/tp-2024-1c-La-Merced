#include "blocked.h"

q_blocked *crear_estado_blocked()
{
   q_blocked *estado = malloc(sizeof(q_blocked));

   estado->io_interfaces = mlist_create();
   estado->resources = mlist_create();

   return estado;
}

void destruir_estado_blocked(q_blocked *estado)
{
   mlist_destroy(estado->io_interfaces);
   mlist_destroy(estado->resources);
}

void conectar_nueva_interfaz(q_blocked *estado, io_queue *io, void *(*rutina_consumo)(void *))
{
   mlist_add(estado->io_interfaces, io);

   pthread_create(&(io->rutina_consumo), NULL, rutina_consumo, io);
   pthread_detach(io->rutina_consumo);
}

int32_t bloquear_para_io(q_blocked *estado, t_pcb *pcb)
{
   int _es_interfaz_buscada(void *elemento)
   {
      io_queue *io = (io_queue *)elemento;
      return strcmp(io->nombre_interfaz, pcb->io_request->inteface_name) == 0;
   };

   io_queue *io_encontrado = mlist_find(estado->io_interfaces, (void *)_es_interfaz_buscada);

   if (io_encontrado == NULL)
      return -1;

   push_proceso(io_encontrado->cola, pcb);
   return 0;
}

q_estado *desconectar_interfaz(q_blocked *estado, int32_t fd_conexion)
{
   int _es_interfaz_buscada(void *elemento)
   {
      io_queue *io = (io_queue *)elemento;
      return io->fd_conexion == fd_conexion;
   };

   io_queue *io_encontrado = mlist_remove_by_condition(estado->io_interfaces, (void *)_es_interfaz_buscada);

   q_estado *cola = io_encontrado->cola;
   free(io_encontrado->nombre_interfaz);
   free(io_encontrado);

   return cola;
}

// TODO
int32_t bloquear_para_recurso(q_blocked *estado, t_pcb *pcb)
{
   return 0;
}