#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/socket.h>

int main(void)
{
    t_config *config = config_create("entradasalida.config");

    // Conectar con Kernel
    char *ip_kernel = config_get_string_value(config, "IP_KERNEL");
    char *puerto_kernel = config_get_string_value(config, "PUERTO_KERNEL");

    int32_t fd_kernel = crear_conexion(ip_kernel, puerto_kernel);
    int32_t res_kernel = handshake(fd_kernel, E_S);
    if (res_kernel == -1)
    {
        liberar_conexion(fd_kernel);
    }

    // Conectar con Memoria
    char *ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char *puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    int32_t fd_memoria = crear_conexion(ip_memoria, puerto_memoria);
    int32_t res_memoria = handshake(fd_memoria, E_S);
    if (res_memoria == -1)
    {
        liberar_conexion(res_memoria);
    }

    liberar_conexion(fd_kernel);
    liberar_conexion(res_memoria);
    return 0;
}
