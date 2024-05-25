#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <protocol/protocol.h>
#include <sockets/sockets.h>
#include "servidor/servidor.h"
#include "logger/logger.h"
#include "conexion/memoria.h"

int main(void)
{
    iniciar_config();
    iniciar_logger();

    // Capaz es un poco confuso la expresion del condicional
    // pero básicamente falla en caso de -1 (o sea, true)
    if (conectar_con_memoria()) // Conexion con Memoria
        return EXIT_FAILURE;

    iniciar_servidor();


    return EXIT_SUCCESS;
}
