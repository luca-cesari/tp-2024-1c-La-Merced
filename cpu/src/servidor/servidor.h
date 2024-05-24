#ifndef CPU_SERVIDOR_H
#define CPU_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include "protocol/protocol.h"
#include "config/config.h"

void iniciar_servidor();
void *atender_kernel_dispatch(void *fd_ptr);
void *atender_kernel_interrupt(void *fd_ptr);
void *escuchar_dispatch(void *fd_dispatch);
void *escuchar_interrupt(void *fd_interrupt);
void recibir_interrupcion_del_kernel(int32_t fd_interrupt);
void interrumpir();

#endif // CPU_SERVIDOR_H