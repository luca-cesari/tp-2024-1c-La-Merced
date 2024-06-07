#include "config/config.h"
#include "logger/logger.h"
#include "servidor/servidor.h"
#include "instrucciones/instrucciones.h"
#include "mem_usuario/mem_usuario.h"

int main(void)
{
    iniciar_config();
    iniciar_logger();

    inicializar_memoria_instrucciones();
    inicializar_memoria_usuario();
    inicializar_lista_tabla_paginas();
    iniciar_servidor();

    destruir_config();
    destruir_logger();

    return EXIT_SUCCESS;
}
