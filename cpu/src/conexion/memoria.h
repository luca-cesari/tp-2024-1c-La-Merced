#ifndef CONEXION_MEMORIA_H
#define CONEXION_MEMORIA_H

#include <stdlib.h>
#include <sockets/sockets.h>
#include <pcb/pcb.h>
#include <protocol/protocol.h>
#include <mem_request/cpu.h>

#include "config/config.h"

int8_t conectar_con_memoria();
void liberar_conexion_memoria();
void enviar_mem_request(t_cpu_mem_req *mem_request);
char *recibir_instruccion();
t_mem_response recibir_response_de_memoria();
t_mem_buffer_response *recibir_buffer_response_de_memoria();
u_int32_t recibir_marco();
u_int32_t get_tamanio_pagina();

#endif // CONEXION_MEMORIA_H