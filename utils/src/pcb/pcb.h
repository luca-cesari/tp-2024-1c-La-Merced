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
   NONE,
   TERMINATED,
   QUANTUM,
   KILL,
   IO,
   WAIT,
   SIGNAL,
   ERROR
} motivo_desalojo;

typedef enum
{
   SUCCESS,
   INVALID_RESOURCE,
   INVALID_INTERFACE,
   INTERRUPTED_BY_USER,
   OUT_OF_MEMORY
} motivo_finalizacion;

typedef struct
{
   u_int32_t pid;
   int32_t program_counter;
   u_int32_t quantum;
   int8_t priority;
   t_registers cpu_registers;
   char *executable;
   t_io_request *io_request;
   char *resource;
   motivo_desalojo motivo_desalojo;
   motivo_finalizacion motivo_finalizacion;
   state estado;
} t_pcb;

t_pcb *crear_pcb(u_int32_t pid, char *ejecutable);
void destruir_pcb(t_pcb *pcb);

t_packet *serializar_pcb(t_pcb *pcb);
void enviar_pcb(int32_t fd_conexion, t_pcb *pcb);
t_pcb *recibir_pcb(int32_t fd_conexion);

void actualizar_pcb(t_pcb *pcb, t_pcb *nuevo_pcb);
void set_quantum_pcb(t_pcb *pcb, u_int32_t quantum);
void set_recurso_pcb(t_pcb *pcb, char *recurso);
void set_estado_pcb(t_pcb *pcb, state estado);
void set_motivo_desalojo(t_pcb *pcb, motivo_desalojo motivo);
void set_motivo_finalizacion(t_pcb *pcb, motivo_finalizacion motivo);
void set_io_request(t_pcb *pcb, t_io_request *io_request);
void set_prioridad(t_pcb *pcb, int8_t prioridad);

void print_pcb(t_pcb *pcb);

#endif // UTILS_PCB_H
