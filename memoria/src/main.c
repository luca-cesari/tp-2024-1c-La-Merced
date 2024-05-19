#include <stdlib.h>
#include <stdio.h>
#include <commons/config.h>
#include <sockets/sockets.h>
#include "config/config.h"
#include "logger/logger.h"
#include "servidor/servidor.h"


int main(void)
{

    iniciar_config();
    iniciar_logger();
    iniciar_servidor();
    //mem_config config = get_memoria_config();
    //char *puerto_escucha = config.puerto_escucha;

    /*int32_t fd_escucha = iniciar_servidor(puerto_escucha);

    while (1)
    {
        esperar_cliente(fd_escucha, &atender_cliente);
    }
*/
    return 0;
}
