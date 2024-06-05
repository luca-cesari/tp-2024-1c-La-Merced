#include "io_queue.h"

io_queue *crear_io_queue(char *nombre_interfaz, int32_t fd_conexion)
{
   io_queue *io = malloc(sizeof(io_queue));

   io->nombre_interfaz = nombre_interfaz;
   io->fd_conexion = fd_conexion;
   io->cola = crear_estado(BLOCKED);

   return io;
}

void destruir_io_queue(io_queue *cola_io)
{
   free(cola_io->nombre_interfaz);
   destruir_estado(cola_io->cola);
   free(cola_io);
}