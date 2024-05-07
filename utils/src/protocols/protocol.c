#include "protocol.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <commons/collections/list.h>

// ARCHIVO "TEMPORAL"
// TA TODO COPIADO DEL TP 0

/*
 *
 * [  "HEADER" |                "PAYLOAD"                 ]
 * [[ OP_CODE ] [[ SIZE | STREAM ] [ SIZE | STREAM ] ... ]]
 *
 */

typedef enum
{
   SIGNAL,
   PACKET
} op_code;

typedef struct
{
   int size;
   void *stream;
} t_buffer;

typedef struct
{
   op_code operation;
   t_buffer *buffer;
} t_packet;

// ==============================================================================================================

void *serializar_paquete(t_packet *paquete, int bytes)
{
   void *magic = malloc(bytes);
   int desplazamiento = 0;

   memcpy(magic + desplazamiento, &(paquete->operation), sizeof(int));
   desplazamiento += sizeof(int);
   memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
   desplazamiento += sizeof(int);
   memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
   desplazamiento += paquete->buffer->size;

   return magic;
}

void crear_buffer(t_packet *paquete)
{
   paquete->buffer = malloc(sizeof(t_buffer));
   paquete->buffer->size = 0;
   paquete->buffer->stream = NULL;
}

t_packet *crear_paquete(void)
{
   t_packet *paquete = malloc(sizeof(t_packet));
   paquete->operation = PACKET;
   crear_buffer(paquete);
   return paquete;
}

void agregar_a_paquete(t_packet *paquete, void *valor, int tamanio)
{
   paquete->buffer->stream = realloc(paquete->buffer->stream, paquete->buffer->size + tamanio + sizeof(int));

   memcpy(paquete->buffer->stream + paquete->buffer->size, &tamanio, sizeof(int));
   memcpy(paquete->buffer->stream + paquete->buffer->size + sizeof(int), valor, tamanio);

   paquete->buffer->size += tamanio + sizeof(int);
}

void enviar_paquete(t_packet *paquete, int socket_cliente)
{
   int bytes = paquete->buffer->size + 2 * sizeof(int);
   void *a_enviar = serializar_paquete(paquete, bytes);

   send(socket_cliente, a_enviar, bytes, 0);

   free(a_enviar);
}

void eliminar_paquete(t_packet *paquete)
{
   free(paquete->buffer->stream);
   free(paquete->buffer);
   free(paquete);
}

// ==============================================================================================================

int recibir_operacion(int socket_cliente)
{
   int cod_op;
   if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
      return cod_op;
   else
   {
      close(socket_cliente);
      return -1;
   }
}

void *recibir_buffer(int *size, int socket_cliente)
{
   void *buffer;

   recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
   buffer = malloc(*size);
   recv(socket_cliente, buffer, *size, MSG_WAITALL);

   return buffer;
}

t_list *recibir_paquete(int socket_cliente)
{
   int size;
   int desplazamiento = 0;
   void *buffer;
   t_list *valores = list_create();
   int tamanio;

   buffer = recibir_buffer(&size, socket_cliente);
   while (desplazamiento < size)
   {
      memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
      desplazamiento += sizeof(int);
      char *valor = malloc(tamanio);
      memcpy(valor, buffer + desplazamiento, tamanio);
      desplazamiento += tamanio;
      list_add(valores, valor);
   }
   free(buffer);
   return valores;
}