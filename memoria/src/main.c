#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/servidor.h>

void atender_cliente(int32_t fd_conexion)
{
    // handsake (para saber quienes el cliente)
    // ver como hacer para registrar los clientes
    // conectados, (algun diccionario global?)

    while (1)
    {
        // listen
    }
}

int main(int argc, char *argv[])
{
    t_config *config = config_create("memoria.config");
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    int32_t fd_escucha = iniciar_servidor(puerto_escucha);

    esperar_cliente(fd_escucha, atender_cliente);

    return 0;
}
