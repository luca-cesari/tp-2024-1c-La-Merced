#ifndef MEMORIA_SERVIDOR_H
#define MEMORIA_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include "config/config.h"
#include "protocol/protocol.h"





/**
 * @brief Inicia el servidor Memoria. La función no es bloqueante,
 *        lanza un hilo para escuchar conexiones.
 */
void iniciar_servidor();
void *escuchar_conexiones(void *fd_escucha);
void escuchar_kernel(int32_t fd_kernel);
void escuchar_cpu(int32_t fd_cpu);
void escuchar_interfaz_es(int32_t fd_es);
void *atender_cliente(void *fd_ptr);


#endif // MEMORIA_SERVIDOR_H