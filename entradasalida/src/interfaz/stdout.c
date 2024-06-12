#include "stdout.h"

void inicializar_interfaz_stdout()
{
    t_dictionary *instruccion_funcion;

    crear_instrucciones();

    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (!dictionary_has_key(instruccion_funcion, peticion_io->instruction))
            enviar_respuesta(INVALID_INSTRUCTION);

        void (*funcion)(char *) = dictionary_get(instruccion_funcion, peticion_io->instruction);
        funcion(peticion_io->arguments);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void crear_instrucciones()
{
    instruccion_funcion = dictionary_create();
    dictionary_put(instruccion_funcion, "IO_STDOUT_WRITE", &io_stdout_write);
}

void io_stdout_write(char **parametros)
{
}