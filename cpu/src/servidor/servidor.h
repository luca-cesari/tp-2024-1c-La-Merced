#ifndef CPU_SERVIDOR_H
#define CPU_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>

#include "config/config.h"

void iniciar_servidor();
void *atender_kernel(void *fd_ptr);
void *escuchar_conexiones(void *fd_escucha);

#endif // CPU_SERVIDOR_H