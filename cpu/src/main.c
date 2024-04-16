#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/servidor.h>
#include <sockets/cliente.h>

void hablar_con_memoria(int32_t fd_memoria)
{
    printf("Memoria conectada \n");
    liberar_conexion(fd_memoria);
}

void escuchar_dispatch(void *fd_ptr)
{
    int32_t fd_dispatch = *((int32_t *)fd_ptr);

    if (recibir_cliente(fd_dispatch) != KERNEL)
    {
        printf("Error de Cliente \n");
        return;
    }

    printf("Kernel conectado por Dispatch \n");
    recibir_mensaje(fd_dispatch);
}

void escuchar_interrupt(void *fd_ptr)
{
    int32_t fd_interrupt = *((int32_t *)fd_ptr);
    if (recibir_cliente(fd_interrupt) != KERNEL)
    {
        printf("Error de Cliente \n");
        return;
    }

    printf("Kernel conectado por Interrupt \n");
    recibir_mensaje(fd_interrupt);
}

int main(void)
{
    char *puerto_dispatch;
    char *puerto_interrupt;
    char *ip_memoria;
    char *puerto_memoria;

    t_config *config = config_create("cpu.config");

    // Conectar con memoria
    ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int32_t fd_memoria = crear_conexion(ip_memoria, puerto_memoria);
    int32_t respuesta = handshake(fd_memoria, CPU);
    if (respuesta == -1)
    {
        printf("Error de conexion a memoria \n");
        liberar_conexion(fd_memoria);
        exit(1);
    }
    else
    {
        hablar_con_memoria(fd_memoria);
    }

    // Escuchar al Kernel
    puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

    int32_t fd_escucha_dispatch = iniciar_servidor(puerto_dispatch);
    esperar_cliente(fd_escucha_dispatch, &escuchar_dispatch);

    int32_t fd_escucha_interrupt = iniciar_servidor(puerto_interrupt);
    esperar_cliente(fd_escucha_interrupt, &escuchar_interrupt);

    liberar_conexion(fd_memoria);
    return 0;
}
