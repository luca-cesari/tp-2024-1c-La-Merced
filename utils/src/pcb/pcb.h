#ifndef UTILS_PCB_H
#define UTILS_PCB_H

#include <stdlib.h>
#include <string.h>
#include <commons/bitarray.h>

#include <protocol/protocol.h>
#include <registers/registers.h>
#include <io_request/io_request.h>

typedef enum
{
   NEW,
   READY,
   BLOCKED,
   EXEC,
   EXIT
} state;

typedef enum
{
   TERMINATED,
   QUANTUM,
   IO
} motivo_desalojo;

typedef enum
{
   SUCCESS,
   INVALID_RESOURCE,
   INVALID_INTERFACE,
   INTERRUPTED_BY_USER,
   OUT_OF_MEMORY
} motivo_finalizacion;

typedef enum
{
   INTERFAZ,
   RECURSO
} motivo_bloqueo;

typedef struct
{
   u_int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   t_registers cpu_registers;
   t_bitarray *psw;
   t_io_request *io_request;
   char *executable_path;
   motivo_desalojo motivo_desalojo;
   motivo_finalizacion motivo_finalizacion;
   state estado;
} t_pcb;

t_pcb *crear_pcb(u_int32_t pid, char *ejecutable);
t_packet *serializar_pcb(t_pcb *pcb);
void enviar_pcb(int32_t fd_conexion, t_pcb *pcb);
t_pcb *recibir_pcb(int32_t fd_conexion);
void destruir_pcb(t_pcb *pcb);

#endif // UTILS_PCB_H
