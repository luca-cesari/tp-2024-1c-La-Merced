#ifndef UTILS_PROTOCOL_H
#define UTILS_PROTOCOL_H

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <commons/collections/list.h>

typedef enum
{
   MESSAGE,
   PACKET
} op_code;

typedef struct
{
   int32_t size;
   void *stream;
} t_buffer;

typedef struct
{
   op_code op_code;
   t_buffer *buffer;
} t_packet;

t_packet *packet_create(void);
void buffer_create(t_packet *packet);
void packet_add(t_packet *packet, void *content, int32_t content_size);
void *packet_serialize(t_packet *packet, int32_t packet_size);
void packet_send(t_packet *packet, int32_t fd_conexion);
void packet_remove(t_packet *packet);

#endif // UTILS_PROTOCOL_H
