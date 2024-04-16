#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/servidor.h>

void escuchar_kernel(int32_t fd_kernel)
{
    printf("Kernel conectado");
}

void escuchar_cpu(int32_t fd_cpu)
{
    printf("CPU conectado");
}

void escuchar_interfaz_es(int32_t fd_e_s)
{
    printf("Interfaz E/S conectada");
}

void atender_cliente(int32_t fd_conexion)
{
    // handsake (para saber quienes el cliente)
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);

    switch (modulo_cliente)
    {
    case KERNEL:
        escuchar_kernel(fd_conexion);
        break;
    case CPU:
        escuchar_cpu(fd_conexion);
        break;
    case E_S:
        escuchar_interfaz_es(fd_conexion);
        break;
    default:
        printf("Error de Cliente \n");
        return;
    }
}

int main(void)
{
    t_config *config = config_create("memoria.config");
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    int32_t fd_escucha = iniciar_servidor(puerto_escucha);

    esperar_cliente(fd_escucha, &atender_cliente);

    return 0;
}
