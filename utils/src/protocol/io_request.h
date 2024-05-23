#ifndef UTILS_PROTOCOL_IO_REQUEST_H
#define UTILS_PROTOCOL_IO_REQUEST_H

#include <stdlib.h>

#include "protocol.h"

typedef struct
{
   u_int32_t pid;
   char *inteface_name;
   char *instruction;
   char *arguments; // esta pensado como un solo string con los argumentos separados por espacios (si es que hay mas de uno)
} t_io_request;

// Ver si es necesario meter el PID para la response
typedef enum
{
   INVALID_INSTRUCTION,
   EXECUTED,
} t_io_response;

t_io_request *crear_io_request(u_int32_t pid, char *nombre_interfaz, char *instruccion, char *argumantos);
t_packet *serializar_io_request(t_io_request *io_request);
void enviar_io_request(int32_t fd_conexion, t_io_request *io_request);
t_io_request *recibir_io_request(int32_t fd_conexion);
void destruir_io_request(t_io_request *io_request);

#endif // UTILS_PROTOCOL_IO_REQUEST_H