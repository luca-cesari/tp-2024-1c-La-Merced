#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <sockets/sockets.h>
#include <pcb/pcb.h>
#include <protocol/protocol.h>
#include <unistd.h>
#include <sys/socket.h>
#include <mem_request/cpu.h>
#include "config/config.h"

int8_t conectar_con_memoria();
void liberar_conexion_memoria();
void enviar_mem_request(t_cpu_mem_req *mem_request);
char *recibir_instruccion();
char *recibir_confirmacion();
u_int32_t recibir_valor();
u_int32_t recibir_marco();
u_int32_t recibir_tamanio_pagina();

#endif // CONEXION_MEMORIA_H