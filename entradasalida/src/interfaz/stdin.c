#include "stdin.h"

void iniciar_rutina_interfaz_stdin()
{

    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_STDIN_READ")!=0)
            enviar_respuesta(INVALID_INSTRUCTION);

        io_stdin_read(peticion_io->arguments);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void io_stdin_read(char *) //RECIBE COMO PARÁMETRO UNA O VARIAS DIRECCIONES FISICAS
{
    //ACA VA LA LOGICA DE STDIN
}
