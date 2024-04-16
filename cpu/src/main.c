#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/servidor.h>

int escuchar_al_kernel(int socket)
{
    int res = esperar_cliente(socket);
    if (res != 0)
    {
        
    }

    return
}

int main(void)
{
    char *puerto_dispatch, puerto_interrupt;
    t_config *config = config_create("cpu.config");

    puerto_dispatch = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH");
    puerto_interrupt = config_get_string_value(config, "PUERTO_ESCUCHA_INTERRUPT");

    int fd_dispatch = iniciar_servidor(puerto_dispatch);
    int fd_interrupt = iniciar_servidor(puerto_interrupt);

    int fd_kernel_dispatch = escuchar_al_kernel(fd_dispatch);
    int fd_kernel_interrupt = escuchar_al_kernel(fd_interrupt);

    return 0;
}
