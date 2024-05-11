#include "kernel.h"


int32_t fd_kernel;

int8_t conectar_con_kernel()
{
    kernel_config ker = get_kernel_config();

    int32_t fd_kernel = crear_conexion(ker.ip , ker.puerto);
    handshake(fd_kernel, E_S);
    enviar_mensaje(fd_kernel, 41);

    return 0;
}


void liberar_conexion_kernel()
{
   liberar_conexion(fd_kernel);
}