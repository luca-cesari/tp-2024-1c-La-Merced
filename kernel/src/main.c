#include "config/config.h"
#include "logger/logger.h"
#include "conexion/cpu.h"
#include "conexion/memoria.h"
#include "servidor/servidor.h"
#include "planificador/planificador.h"
#include "consola/consola.h"

int main(void)
{
    iniciar_config();
    iniciar_logger();

    // Capaz es un poco confuso la expresion del condicional
    // pero básicamente falla en caso de -1 (o sea, true)
    if (conectar_por_dispatch()) // Conexion con CPU
        return EXIT_FAILURE;

    if (conectar_por_interrupt()) // Conexion con CPU
        return EXIT_FAILURE;

    if (conectar_con_memoria()) // Conexion con Memoria
        return EXIT_FAILURE;

    inicializar_planificador();

    iniciar_servidor();

    iniciar_consola(); // bloqueante (corre sobre este hilo)

    liberar_conexion_cpu();
    liberar_conexion_memoria();

    destruir_planificador();
    destruir_config();
    destruir_logger();

    return EXIT_SUCCESS;
}
