#include "generica.h"

void iniciar_rutina_interfaz_generica(void)
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_GEN_SLEEP") != 0)
            enviar_respuesta(INVALID_INSTRUCTION);

        gen_sleep(peticion_io->arguments);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void gen_sleep(char *cant_unidad_trabajo)
{
    u_int32_t tiempo_unidad_trabajo = get_tiempo_unidad_trabajo();

    u_int32_t tiempo_a_bloquear = atoi(cant_unidad_trabajo) * tiempo_unidad_trabajo;
    usleep(tiempo_a_bloquear * 1000);
}
