#include "config/config.h"
#include "logger/logger.h"
#include "conexion/memoria.h"
#include "mmu/mmu.h"
#include "servidor/servidor.h"

int main(void)
{
    iniciar_config();
    iniciar_logger();

    inicializar_interrupcion();
    inicializar_mmu();
    inicializar_instrucciones();

    // Capaz es un poco confuso la expresion del condicional
    // pero básicamente falla en caso de -1 (o sea, true)
    if (conectar_con_memoria()) // Conexion con Memoria
        return EXIT_FAILURE;

    iniciar_servidor();

    liberar_conexion_memoria();
    destruir_mmu();
    finalizar_servidor();
    destruir_config();
    destruir_logger();

    return EXIT_SUCCESS;
}
