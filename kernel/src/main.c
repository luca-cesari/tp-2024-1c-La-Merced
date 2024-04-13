#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <sockets/servidor.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>
#include <readline/readline.h>

#include <pthread.h>


struct PCB
{
};

int main(void)
{

    int server_fd = iniciar_servidor("8023");

    printf("entra a esperar \n");

    int cliente_fd = esperar_cliente(server_fd);

    recibir_mensaje(cliente_fd);

    return EXIT_SUCCESS;
}
