#include "stdin.h"

void iniciar_rutina_interfaz_stdin()
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();
        if (!string_is_equal(peticion_io->instruction, "IO_STDIN_READ"))
        {
            enviar_respuesta(INVALID_INSTRUCTION);
            destruir_io_request(peticion_io);
            continue;
        }

        int8_t io_result = io_stdin_read(peticion_io->arguments, peticion_io->pid);
        if (io_result == -1)
        {
            enviar_respuesta(FAILED);
            destruir_io_request(peticion_io);
            continue;
        }

        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
        destruir_io_request(peticion_io);
    }
}

int8_t io_stdin_read(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t tamanio_valor = atoi(string_array_pop(parametros));
    char *direcciones_fisicas = array_a_string(parametros);

    char *valor_consola = iniciar_consola(tamanio_valor);

    t_io_mem_req *mem_request = crear_io_mem_request(ESCRIBIR_IO, pid, direcciones_fisicas, tamanio_valor, valor_consola);
    enviar_mem_request(mem_request);
    destruir_io_mem_request(mem_request);

    t_mem_response response = recibir_valor();
    return response == OPERATION_SUCCEED ? 0 : -1;
}