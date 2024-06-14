#include "stdout.h"

void inicializar_interfaz_stdout()
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_STDOUT_WRITE") != 0)
            enviar_respuesta(INVALID_INSTRUCTION);

        int8_t io_result = io_stdout_write(peticion_io->arguments, peticion_io->pid);
        if (io_result == -1)
        {
            enviar_respuesta(FAILED);
            continue;
        }
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

int8_t io_stdout_write(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t tamanio_valor = atoi(string_array_pop(parametros));
    char *direcciones_fisicas = array_a_string(parametros);

    parametros_io parametros_leer;
    parametros_leer.param_leer.direcciones_fisicas = direcciones_fisicas;
    parametros_leer.param_leer.tamanio_buffer = tamanio_valor;

    t_io_mem_req *mem_request = crear_io_mem_request(LEER_IO, pid, parametros_leer);
    enviar_mem_request(mem_request);
    destruir_io_mem_request(mem_request);

    char *respuesta = (char *)recibir_mem_buffer();
    if (respuesta == NULL)
        return -1;

    printf("%s\n", respuesta);
    return 0;
}