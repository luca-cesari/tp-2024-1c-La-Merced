#ifndef CPU_SERVIDOR_H
#define CPU_SERVIDOR_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <protocol/protocol.h>
#include <pcb/pcb.h>

#include "config/config.h"
#include "interrupcion/interrupcion.h"
#include "instrucciones/ciclo.h"

void iniciar_servidor();
void *atender_kernel_dispatch(void *fd_ptr);
void *atender_kernel_interrupt(void *fd_ptr);

#endif // CPU_SERVIDOR_H