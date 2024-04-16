#include <stdlib.h>
#include <stdio.h>

#include <commons/config.h>

#include <sockets/servidor.h>

void atender_cliente(int32_t fd_conexion)
{
    // handsake (para saber quienes el cliente)
    uint32_t modulo_cliente = recibir_cliente(fd_conexion);
    printf("Modulo conectado: %d", modulo_cliente);
    // ver como hacer para registrar los clientes
    // conectados, (algun diccionario global? para asociar, modulo con fd)

    // listen
    // while (1)
    // {
    // }
}

int main(void)
{
    t_config *config = config_create("memoria.config");
    char *puerto_escucha = config_get_string_value(config, "PUERTO_ESCUCHA");

    int32_t fd_escucha = iniciar_servidor(puerto_escucha);

    esperar_cliente(fd_escucha, atender_cliente);

    return 0;
}
