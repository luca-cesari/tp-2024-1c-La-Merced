#include "config/config.h"
#include "logger/logger.h"
#include "servidor/servidor.h"
#include "instrucciones/instrucciones.h"

int main(void)
{
    iniciar_config();
    iniciar_logger();

    inicializar_memoria_instrucciones();
    iniciar_servidor();

    return 0;
}
