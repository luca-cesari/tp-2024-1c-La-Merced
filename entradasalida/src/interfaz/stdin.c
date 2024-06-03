#include "stdin.h"

t_dictionary *instrucciones;

void iniciar_rutina_interfaz_stdin()
{
    inicializar_instrucciones();

    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (!dictionary_has_key(instrucciones, peticion_io->instruction))
            enviar_respuesta(INVALID_INSTRUCTION);

        void (*funcion)(char *) = dictionary_get(instrucciones, peticion_io->instruction);
        funcion(peticion_io->arguments);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void inicializar_instrucciones()
{
    instrucciones = dictionary_create();
    dictionary_put(instrucciones, "IO_STDIN_READ", &io_stdin_read);
}

void io_stdin_read(char *) //RECIBE COMO PARÁMETRO UNA O VARIAS DIRECCIONES FISICAS
{
    //ACA VA LA LOGICA DE STDIN
}
