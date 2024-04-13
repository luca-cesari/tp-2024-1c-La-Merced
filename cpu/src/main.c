#include <stdlib.h>
#include <stdio.h>
#include <sockets/cliente.h>

int main(void)
{
    int conexion = crear_conexion("127.0.0.1", "8023");
    handshake(conexion);
    liberar_conexion(conexion);
    return 0;
}
