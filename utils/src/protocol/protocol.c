#include "protocol.h"

// ==============================================================================================================

t_packet *packet_create(void)
{
   t_packet *packet = malloc(sizeof(t_packet));
   packet->op_code = PACKET;
   buffer_create(packet);
   return packet;
}

void buffer_create(t_packet *packet)
{
   packet->buffer = malloc(sizeof(t_buffer));
   packet->buffer->size = 0;
   packet->buffer->stream = NULL;
}

void packet_add(t_packet *packet, void *content, int32_t content_size)
{
   // aumenta el tamaño del buffer agregando:
   // - el tamaño del contenido
   // - el tamaño de un entero que va a guardar el tamaño del contenido
   int32_t new_size = packet->buffer->size + content_size + sizeof(int32_t);
   packet->buffer->stream = realloc(packet->buffer->stream, new_size);

   int32_t offset = 0;

   // agrega el tamaño del contenido
   offset = packet->buffer->size;
   memcpy(packet->buffer->stream + offset, &content_size, sizeof(int32_t));

   // agrega el contenido
   offset = packet->buffer->size + sizeof(int32_t);
   memcpy(packet->buffer->stream + offset, content, content_size);

   // actualiza el tamaño del buffer
   packet->buffer->size += content_size + sizeof(int32_t);
}

void *packet_serialize(t_packet *packet, int32_t packet_size)
{
   void *serialized = malloc(packet_size);
   int32_t offset = 0;

   // agrega el código de operación
   memcpy(serialized + offset, &(packet->op_code), sizeof(int32_t));
   offset += sizeof(int32_t);

   // agrega el tamaño del buffer
   memcpy(serialized + offset, &(packet->buffer->size), sizeof(int32_t));
   offset += sizeof(int32_t);

   // agrega el contenido del buffer
   memcpy(serialized + offset, packet->buffer->stream, packet->buffer->size);
   // desplazamiento += packet->buffer->size;

   return serialized;
}

void packet_send(t_packet *packet, int32_t fd_conexion)
{
   // los dos primeros int32_t son el código de operación y el tamaño del buffer
   int32_t packet_size = packet->buffer->size + 2 * sizeof(int32_t);
   void *serialized = packet_serialize(packet, packet_size);

   send(fd_conexion, serialized, packet_size, 0);

   free(serialized);
}

void packet_remove(t_packet *packet)
{
   free(packet->buffer->stream);
   free(packet->buffer);
   free(packet);
}

// ==============================================================================================================

// int recibir_operacion(int socket_cliente)
// {
//    int cod_op;
//    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
//       return cod_op;
//    else
//    {
//       close(socket_cliente);
//       return -1;
//    }
// }

// void *recibir_buffer(int *size, int socket_cliente)
// {
//    void *buffer;

//    recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
//    buffer = malloc(*size);
//    recv(socket_cliente, buffer, *size, MSG_WAITALL);

//    return buffer;
// }

// t_list *recibir_paquete(int socket_cliente)
// {
//    int size;
//    int desplazamiento = 0;
//    void *buffer;
//    t_list *valores = list_create();
//    int tamanio;

//    buffer = recibir_buffer(&size, socket_cliente);
//    while (desplazamiento < size)
//    {
//       memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
//       desplazamiento += sizeof(int);
//       char *valor = malloc(tamanio);
//       memcpy(valor, buffer + desplazamiento, tamanio);
//       desplazamiento += tamanio;
//       list_add(valores, valor);
//    }
//    free(buffer);
//    return valores;
// }