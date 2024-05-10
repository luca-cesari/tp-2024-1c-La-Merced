#include "protocol.h"

t_packet *crear_paquete(void)
{
   t_packet *paquete = malloc(sizeof(t_packet));
   paquete->op_code = PACKET;
   crear_buffer(paquete);
   return paquete;
}

void crear_buffer(t_packet *paquete)
{
   paquete->buffer = malloc(sizeof(t_buffer));
   paquete->buffer->size = 0;
   paquete->buffer->stream = NULL;
}

void agregar_a_paquete(t_packet *paquete, void *contenido, int32_t tamanio)
{
   // aumenta el tamaño del buffer agregando:
   // - el tamaño del contenido
   // - el tamaño de un entero que va a guardar el tamaño del contenido
   int32_t new_size = paquete->buffer->size + tamanio + sizeof(int32_t);
   paquete->buffer->stream = realloc(paquete->buffer->stream, new_size);

   int32_t offset = 0;

   // agrega el tamaño del contenido
   offset = paquete->buffer->size;
   memcpy(paquete->buffer->stream + offset, &tamanio, sizeof(int32_t));

   // agrega el contenido
   offset = paquete->buffer->size + sizeof(int32_t);
   memcpy(paquete->buffer->stream + offset, contenido, tamanio);

   // actualiza el tamaño del buffer
   paquete->buffer->size += tamanio + sizeof(int32_t);
}

void *serializar_paquete(t_packet *paquete, int32_t tamanio_paquete)
{
   void *serializado = malloc(tamanio_paquete);
   int32_t offset = 0;

   // agrega el código de operación
   memcpy(serializado + offset, &(paquete->op_code), sizeof(int32_t));
   offset += sizeof(int32_t);

   // agrega el tamaño del buffer
   memcpy(serializado + offset, &(paquete->buffer->size), sizeof(int32_t));
   offset += sizeof(int32_t);

   // agrega el contenido del buffer
   memcpy(serializado + offset, paquete->buffer->stream, paquete->buffer->size);
   // desplazamiento += paquete->buffer->size;

   return serializado;
}

void enviar_paquete(t_packet *paquete, int32_t fd_conexion)
{
   // los dos primeros int32_t son el código de operación y el tamaño del buffer
   int32_t tamanio_paquete = paquete->buffer->size + 2 * sizeof(int32_t);
   void *serializado = serializar_paquete(paquete, tamanio_paquete);

   send(fd_conexion, serializado, tamanio_paquete, 0);

   free(serializado);
}

void eliminar_paquete(t_packet *paquete)
{
   free(paquete->buffer->stream);
   free(paquete->buffer);
   free(paquete);
}

int32_t recibir_operacion(int fd_conexion)
{
   op_code code;
   if (recv(fd_conexion, &code, sizeof(int32_t), MSG_WAITALL) > 0)
      return code;
   else
   {
      close(fd_conexion);
      return -1;
   }
}

void *recibir_buffer(int32_t *tamanio, int32_t fd_conexion)
{
   void *buffer;

   recv(fd_conexion, tamanio, sizeof(int32_t), MSG_WAITALL);
   buffer = malloc(*tamanio);
   recv(fd_conexion, buffer, *tamanio, MSG_WAITALL);

   return buffer;
}

t_list *recibir_paquete(int32_t fd_conexion)
{
   //...
   return NULL;
}

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