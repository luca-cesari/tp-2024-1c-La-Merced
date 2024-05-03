#include "config/config.h"
#include "logger/logger.h"
#include "conexion/cpu.h"
#include "conexion/memoria.h"
#include "consola/consola.h"

void *atender_interfaz_es(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);
    free(fd_ptr); // Hace falta?

    // atender handsake (para saber quienes el cliente)
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    if (modulo_cliente != E_S)
    {
        printf("Error de Cliente \n");
        return NULL;
    }

    printf("Interfaz E/S conectada \n");
    recibir_mensaje(fd_conexion);

    return NULL;
}

int main(void)
{
    iniciar_config();
    iniciar_logger();

    int32_t fd_escucha;

    // Capaz es un poco confuso la expresion del condicional
    // pero básicamente falla en caso de -1 (o sea, true)
    if (conectar_por_dispatch()) // Conexion con CPU
        return EXIT_FAILURE;

    if (conectar_por_interrupt()) // Conexion con CPU
        return EXIT_FAILURE;

    if (conectar_con_memoria()) // Conexion con Memoria
        return EXIT_FAILURE;

    // Escuchar Interfaces (Tiene que ser rutina en un hilo aparte, no puede bloquear el hilo principal)
    char *puerto_escucha = get_puerto_escucha();
    fd_escucha = iniciar_servidor(puerto_escucha);
    while (1)
    {
        esperar_cliente(fd_escucha, &atender_interfaz_es);
    }

    iniciar_consola(); // bloqueante (corre sobre este hilo)

    liberar_conexion_cpu();
    liberar_conexion_memoria();

    destruir_config();
    destruir_logger();

    return EXIT_SUCCESS;
}
