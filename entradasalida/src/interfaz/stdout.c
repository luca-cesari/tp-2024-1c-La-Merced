#include "stdout.h"

void inicializar_interfaz_stdout()
{

    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_STDOUT_WRITE") != 0)
            enviar_respuesta(INVALID_INSTRUCTION);

        io_stdout_write(peticion_io->arguments);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void io_stdout_write(char *parametros)
{
}
