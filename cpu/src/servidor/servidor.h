#ifndef CPU_SERVIDOR_H
#define CPU_SERVIDOR_H

#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <sockets/sockets.h>

#include "protocol/protocol.h"
#include "protocol/pcb.h"
#include "config/config.h"
#include "instrucciones/ciclo.h"

void iniciar_servidor();
void *atender_kernel_dispatch(void *fd_ptr);
void *atender_kernel_interrupt(void *fd_ptr);
void recibir_interrupcion_del_kernel(int32_t fd_interrupt);
void interrumpir();

#endif // CPU_SERVIDOR_H