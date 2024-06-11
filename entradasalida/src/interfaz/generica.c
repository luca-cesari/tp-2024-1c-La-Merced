#include "generica.h"

t_dictionary *instruccion_funcion;

void iniciar_rutina_interfaz_generica(void)
{
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
    dictionary_put(instruccion_funcion, "IO_GEN_SLEEP", &gen_sleep);
}

void gen_sleep(char *cant_unidad_trabajo)
{
    u_int32_t tiempo_en_ms = get_tiempo_unidad_trabajo();

    u_int32_t tiempo_a_bloquear = atoi(cant_unidad_trabajo) * tiempo_en_ms;
    sleep(tiempo_a_bloquear / 1000);
}
