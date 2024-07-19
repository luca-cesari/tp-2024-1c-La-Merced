#include "stdout.h"

void inicializar_interfaz_stdout()
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (!string_is_equal(peticion_io->instruction, "IO_STDOUT_WRITE"))
        {
            enviar_respuesta(INVALID_INSTRUCTION);
            destruir_io_request(peticion_io);
            continue;
        }

        int8_t io_result = io_stdout_write(peticion_io->arguments, peticion_io->pid);
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

int8_t io_stdout_write(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t tamanio_valor = atoi(string_array_pop(parametros));
    char *direcciones_fisicas = array_a_string(parametros);

    t_io_mem_req *mem_request = crear_io_mem_request(LEER_IO, pid, direcciones_fisicas, tamanio_valor, NULL);
    enviar_mem_request(mem_request);
    destruir_io_mem_request(mem_request);

    t_mem_buffer_response *respuesta = recibir_mem_buffer();
    if (respuesta->resultado == OPERATION_FAILED)
        return -1;

    printf("%s\n", (char *)respuesta->buffer);
    destruir_buffer_response(respuesta);
    string_array_destroy(parametros);
    
    return 0;
}