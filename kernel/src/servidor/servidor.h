#ifndef KERNEL_SERVIDOR_H
#define KERNEL_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <commons/string.h>
#include <protocol/protocol.h>

#include "logger/logger.h"
#include "config/config.h"
#include "planificador/planificador.h"

/**
 * @brief Inicia el servidor Kernel. La función no es bloqueante,
 *        lanza un hilo para escuchar conexiones.
 */
void iniciar_servidor();
void *escuchar_conexiones(void *fd_escucha);
void *atender_interfaz_es(void *fd_ptr);
void finalizar_servidor();

#endif // KERNEL_SERVIDOR_H