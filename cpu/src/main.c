#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <protocol/protocol.h>
#include <sockets/sockets.h>

void hablar_con_memoria(int32_t fd_memoria)
{
    printf("Memoria conectada \n");
    // enviar_mensaje("21", fd_memoria); // mensaje de prueba

    // recibir_mensaje(fd_memoria); // para bloquear nd mas
}

void *escuchar_dispatch(void *fd_ptr)
{
    int32_t fd_dispatch = *((int32_t *)fd_ptr);

    if (recibir_cliente(fd_dispatch) != KERNEL)
    {
        printf("Error de Cliente \n");
        return NULL;
    }

    printf("Kernel conectado por Dispatch \n");
    while (1)
    {
        // recibir_mensaje(fd_dispatch);
    }

    return NULL;
}

void *escuchar_interrupt(void *fd_ptr)
{
    int32_t fd_interrupt = *((int32_t *)fd_ptr);
    if (recibir_cliente(fd_interrupt) != KERNEL)
    {
        printf("Error de Cliente \n");
        return NULL;
    }

    printf("Kernel conectado por Interrupt \n");
    while (1)
    {
        // recibir_mensaje(fd_interrupt);
    }

    return NULL;
}

int main(void)
{
    iniciar_config();
    iniciar_logger();

    // Capaz es un poco confuso la expresion del condicional
    // pero básicamente falla en caso de -1 (o sea, true)
    if (conectar_con_memoria()) // Conexion con Memoria
        return EXIT_FAILURE;

    iniciar_servidor();

    int32_t fd_escucha_dispatch = crear_servidor(puerto_dispatch);
    esperar_cliente(fd_escucha_dispatch, &escuchar_dispatch);

    int32_t fd_escucha_interrupt = crear_servidor(puerto_interrupt);
    esperar_cliente(fd_escucha_interrupt, &escuchar_interrupt);

    return EXIT_SUCCESS;
}
