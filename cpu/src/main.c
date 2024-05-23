#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/sockets.h>


int main(void)
{
    char *puerto_dispatch;
    char *puerto_interrupt;
    char *ip_memoria;
    char *puerto_memoria;

    t_config *config = config_create("cpu.config");

    // Escuchar al Kernel
    puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

    int32_t fd_escucha_dispatch = iniciar_servidor(puerto_dispatch);
    esperar_cliente(fd_escucha_dispatch, &escuchar_dispatch);

    int32_t fd_escucha_interrupt = iniciar_servidor(puerto_interrupt);
    esperar_cliente(fd_escucha_interrupt, &escuchar_interrupt);

    // Conectar con memoria
    ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int32_t fd_memoria = crear_conexion(ip_memoria, puerto_memoria);
    int32_t respuesta = handshake(fd_memoria, CPU);
    if (respuesta == -1)
    {
        printf("Error de conexion a memoria \n");
        liberar_conexion(fd_memoria);
        return 1;
    }
    hablar_con_memoria(fd_memoria);

    liberar_conexion(fd_memoria);
    return 0;
}
