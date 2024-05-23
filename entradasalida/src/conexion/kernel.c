#include "kernel.h"

int32_t fd_kernel;

int8_t conectar_con_kernel(char *nombre_interfaz)
{
    kernel_config kernel = get_kernel_config();

    int32_t fd_kernel = crear_conexion(kernel.ip, kernel.puerto);
    int32_t response = handshake(fd_kernel, E_S);
    if (response == -1)
    {
        liberar_conexion(fd_kernel);
        return -1;
    }

    enviar_mensaje(nombre_interfaz, fd_kernel);

    return 0;
}

t_io_request *esperar_instruccion()
{
    return recibir_io_request(fd_kernel);
}

void enviar_respuesta(t_io_response respuesta)
{
    enviar_senial(respuesta, fd_kernel);
}

void liberar_conexion_kernel()
{
    liberar_conexion(fd_kernel);
}