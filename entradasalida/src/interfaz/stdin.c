#include "stdin.h"

static char *array_a_string(char **array);

void iniciar_rutina_interfaz_stdin()
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_STDIN_READ") != 0)
            enviar_respuesta(INVALID_INSTRUCTION);

        int8_t io_result = io_stdin_read(peticion_io->arguments, peticion_io->pid);
        if (io_result == -1)
        {
            enviar_respuesta(FAILED);
            continue;
        }
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

int8_t io_stdin_read(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t tamanio_valor = atoi(string_array_pop(parametros));

    char *direcciones_fisicas = array_a_string(parametros);

    char *valor_consola = iniciar_consola(tamanio_valor);

    parametros_io parametros_escribir;
    parametros_escribir.param_escribir.direcciones_fisicas = direcciones_fisicas;
    parametros_escribir.param_escribir.buffer = valor_consola;
    parametros_escribir.param_escribir.tamanio_buffer = tamanio_valor;

    t_io_mem_req *mem_request = crear_io_mem_request(ESCRIBIR_IO, pid, parametros_escribir);

    enviar_mem_request(mem_request);

    t_mem_response response = recibir_valor();
    return response == OPERATION_SUCCEED ? 0 : -1;
}

static char *array_a_string(char **array)
{
    char *string = string_new();
    int i = 0;
    while (array[i] != NULL)
    {
        string_append(&string, array[i]);
        i++;
    }
    return string;
}