#ifndef MEMORIA_SERVIDOR_H
#define MEMORIA_SERVIDOR_H

#include <stdlib.h>
#include <pthread.h>
#include <sockets/sockets.h>
#include <commons/string.h>
#include <mem_request/kernel.h>
#include <mem_request/cpu.h>
#include <mem_request/entradasalida.h>

#include "config/config.h"
#include "protocol/protocol.h"
#include "instrucciones/instrucciones.h"
#include "mem_usuario/mem_usuario.h"

/**
 * @brief Inicia el servidor Memoria. La función no es bloqueante,
 *        lanza un hilo para escuchar conexiones.
 */
void iniciar_servidor();
void *escuchar_conexiones(void *fd_escucha);
void *escuchar_interfaces(void *);
void *atender_cpu(void *fd_ptr);
void *atender_kernel(void *fd_ptr);
void *atender_interfaz(void *fd_ptr);
void finalizar_servidor();

#endif // MEMORIA_SERVIDOR_H