#include "stdout.h"
/*
IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño):
Esta instrucción solicita al Kernel que mediante la interfaz seleccionada,
se lea desde la posición de memoria indicada por la Dirección Lógica almacenada
en el Registro Dirección, un tamaño indicado por el Registro Tamaño y se imprima
por pantalla.
*/
void inicializar_interfaz_stdout()
{
    while (1)
    {
        t_io_request *peticion_io = esperar_instruccion();

        if (strcmp(peticion_io->instruction, "IO_STDOUT_WRITE") != 0)
            enviar_respuesta(INVALID_INSTRUCTION);

        io_stdout_write(peticion_io->arguments, peticion_io->pid);
        log_operacion(peticion_io->pid, peticion_io->instruction);
        enviar_respuesta(EXECUTED);
    }
}

void io_stdout_write(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    char *direcciones_fisicas = array_a_string(parametros);
    int tamanio_valor = atoi(string_array_pop(parametros));

    parametros_io parametros_leer;
    parametros_leer.param_leer.direcciones_fisicas = direcciones_fisicas;
    parametros_leer.param_leer.tamanio_buffer = tamanio_valor;

    t_io_mem_req *mem_request = crear_io_mem_request(LEER_IO, pid, parametros_leer);
    enviar_mem_request(mem_request);
    int32_t respuesta = recibir_valor();
    printf("El valor recibido es: %d\n", respuesta); //
}

char *array_a_string(char **array)
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