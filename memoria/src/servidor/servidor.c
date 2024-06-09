#include "servidor.h"

void iniciar_servidor()
{
    char *puerto_escucha = get_puerto_escucha();
    int32_t fd_escucha = crear_servidor(puerto_escucha);

    while (1)
    {
        esperar_cliente(fd_escucha, &atender_cliente);
    }
}

void *atender_cliente(void *fd_ptr)
{
    int32_t fd_conexion = *((int32_t *)fd_ptr);

    // atender handsake (para saber quienes el cliente)
    int32_t modulo_cliente = recibir_cliente(fd_conexion);

    switch (modulo_cliente)
    {
    case KERNEL:
        escuchar_kernel(fd_conexion);
        break;
    case CPU:
        escuchar_cpu(fd_conexion);
        break;
    case E_S:
        escuchar_interfaz_es(fd_conexion);
        break;
    default:
        printf("Error de Cliente \n");
        return NULL;
    }

    return NULL;
}

void escuchar_kernel(int32_t fd_kernel)
{
    printf("Kernel conectado \n");

    while (1)
    {
        t_kernel_mem_req *mem_request = recibir_kernel_mem_request(fd_kernel);

        switch (mem_request->operacion)
        {
        case INICIAR_PROCESO:
            printf("INICIAR_PROCESO \n");
            cargar_proceso_a_memoria(mem_request->pid, mem_request->parametros.path);
            crear_tabla_de_paginas_para_proceso(mem_request->pid);
            break;

        case FINALIZAR_PROCESO:
            printf("FINALIZAR_PROCESO \n");
            // ...
            break;

        default:
            printf("Error de instruccion \n");
            // ...
            break;
        }
    }
}

void escuchar_cpu(int32_t fd_cpu)
{
    printf("CPU conectado \n");

    while (1)
    {
        t_cpu_mem_req *mem_request = recibir_cpu_mem_request(fd_cpu);
        switch (mem_request->operacion)
        {
        case FETCH_INSTRUCCION:
            printf("FETCH_INSTRUCCION \n");
            char *instruccion = proxima_instruccion(mem_request->pid, mem_request->parametros.program_counter);
            enviar_mensaje(instruccion, fd_cpu);
            break;

        case OBTENER_MARCO:
            printf("OBTENER_MARCO \n");
            u_int32_t marco = obtener_marco(mem_request->pid, mem_request->parametros.nro_pag);
            enviar_senial(marco, fd_cpu); // ver si es con senial por tema del unsigned
            break;

        default:
            printf("Error de instruccion \n");
            // ...
            break;
        }
    }
}

void escuchar_interfaz_es(int32_t fd_es)
{
    printf("Interfaz E/S conectada \n");
    recibir_mensaje(fd_es);
}
