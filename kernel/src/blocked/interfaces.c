#include "interfaces.h"

static void _bloquear_interfaz(void *ptr_interfaz);
static void _desbloquear_interfaz(void *ptr_interfaz);

io_queue *crear_io_queue(char *nombre_interfaz, int32_t fd_conexion)
{
   io_queue *io = malloc(sizeof(io_queue));

   io->nombre_interfaz = nombre_interfaz;
   io->fd_conexion = fd_conexion;
   io->cola_procesos = crear_estado(BLOCKED);

   return io;
}

void destruir_io_queue(void *prt_io)
{
   io_queue *cola_io = (io_queue *)prt_io;
   free(cola_io->nombre_interfaz);
   destruir_estado(cola_io->cola_procesos);
   free(cola_io);
}

void conectar_nueva_interfaz(q_blocked *estado, io_queue *io, void *(*rutina_consumo)(void *))
{
   mlist_add(estado->lista_colas, io);

   pthread_create(&(io->rutina_consumo), NULL, rutina_consumo, io);
   pthread_detach(io->rutina_consumo);
}

int32_t bloquear_para_io(q_blocked *estado, t_pcb *pcb)
{
   int _es_interfaz_buscada(void *elemento)
   {
      io_queue *io = (io_queue *)elemento;
      return strcmp(io->nombre_interfaz, pcb->io_request->interface_name) == 0;
   };

   io_queue *io_encontrado = mlist_find(estado->lista_colas, (void *)_es_interfaz_buscada);

   if (io_encontrado == NULL)
      return -1;

   push_proceso(io_encontrado->cola_procesos, pcb);
   return 0;
}

q_estado *desconectar_interfaz(q_blocked *estado, int32_t fd_conexion)
{
   int _es_interfaz_buscada(void *elemento)
   {
      io_queue *io = (io_queue *)elemento;
      return io->fd_conexion == fd_conexion;
   };

   io_queue *io_encontrado = mlist_remove_by_condition(estado->lista_colas, (void *)_es_interfaz_buscada);

   q_estado *procesos = io_encontrado->cola_procesos;
   free(io_encontrado->nombre_interfaz);
   free(io_encontrado);

   return procesos;
}

void bloquear_colas_io(q_blocked *estado)
{
   mlist_iterate(estado->lista_colas, &_bloquear_interfaz);
}

void desbloquear_colas_io(q_blocked *estado)
{
   mlist_iterate(estado->lista_colas, &_desbloquear_interfaz);
}

static void _bloquear_interfaz(void *ptr_interfaz)
{
   io_queue *interfaz = (io_queue *)ptr_interfaz;
   bloquear_estado(interfaz->cola_procesos);
}

static void _desbloquear_interfaz(void *ptr_interfaz)
{
   io_queue *interfaz = (io_queue *)ptr_interfaz;
   desbloquear_estado(interfaz->cola_procesos);
}