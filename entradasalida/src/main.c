#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/sockets.h>

int main(void)
{
    t_config *config = config_create("entradasalida.config");

    // Conectar con Kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

    int32_t fd_kernel = crear_conexion(ip_kernel, puerto_kernel);
    handshake(fd_kernel, E_S);
    enviar_mensaje(fd_kernel, 41);

    // Conectar con Memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int32_t fd_memoria = crear_conexion(ip_memoria, puerto_memoria);
    handshake(fd_memoria, E_S);
    enviar_mensaje(fd_memoria, 42);

    liberar_conexion(fd_kernel);
    liberar_conexion(res_memoria);
    return 0;
}
