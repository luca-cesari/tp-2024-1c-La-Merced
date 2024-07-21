#include "servidor.h"

int32_t fd_escucha;

static void responder_lectura(void *buffer, u_int32_t tamanio_buffer, int32_t fd_conexion);
static void retardo_respuesta(void);

void iniciar_servidor()
{
    char *puerto_escucha = get_puerto_escucha();
    fd_escucha = crear_servidor(puerto_escucha);

    while (1)
    {
        int8_t error = esperar_cliente(fd_escucha, &atender_cliente);
        printf("retorno esperar cliente , %d\n", error);
        if (error)
            return;
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
        if (mem_request == NULL)
            return;

        // Dado que es indistinto el momento en que se aplica el retardo,
        // lo aplico antes de procesar la solicitud
        retardo_respuesta();
        switch (mem_request->operacion)
        {
        case INICIAR_PROCESO:
            printf("INICIAR_PROCESO \n");
            cargar_proceso_a_memoria(mem_request->pid, mem_request->path);
            crear_tabla_de_paginas_para_proceso(mem_request->pid);

            // Siempre envía resultado exitoso, podría haber error??
            enviar_senial(OPERATION_SUCCEED, fd_kernel);
            break;

        case FINALIZAR_PROCESO:
            printf("FINALIZAR_PROCESO \n");
            // Creo que falta sacar el proceso de la memoria de instrucciones
            destruir_tabla_de_paginas_para_proceso(mem_request->pid);
            break;

        default:
            printf("Error de instruccion \n");
            // ...
            break;
        }

        destruir_kernel_mem_request(mem_request);
    }
}

void escuchar_cpu(int32_t fd_cpu)
{
    printf("CPU conectado \n");
    enviar_senial(get_tamanio_pagina(), fd_cpu);

    while (1)
    {
        t_cpu_mem_req *mem_request = recibir_cpu_mem_request(fd_cpu);
        if (mem_request == NULL)
        {
            printf("deberia cerrar \n");
            close(fd_escucha);
            return;
        }

        t_list *direcciones_fisicas;
        retardo_respuesta();

        switch (mem_request->operacion)
        {
        case FETCH_INSTRUCCION:
            printf("FETCH_INSTRUCCION \n");
            char *instruccion = proxima_instruccion(mem_request->pid, mem_request->program_counter);
            enviar_mensaje(instruccion, fd_cpu);
            break;

        case OBTENER_MARCO:
            printf("OBTENER_MARCO \n");
            u_int32_t marco = obtener_marco(mem_request->pid, mem_request->nro_pag);
            enviar_senial(marco, fd_cpu); // ver si es con senial por tema del unsigned
            break;

        case LEER:
            printf("LEER \n");
            direcciones_fisicas = convertir_a_lista_de_direcciones_fisicas(mem_request->direcciones_fisicas);
            void *buffer_response = leer_memoria_usuario(mem_request->pid, direcciones_fisicas, mem_request->tamanio_buffer);
            responder_lectura(buffer_response, mem_request->tamanio_buffer, fd_cpu);
            free(buffer_response);
            list_destroy_and_destroy_elements(direcciones_fisicas, &free);
            break;

        case ESCRIBIR:
            printf("ESCRIBIR \n");
            direcciones_fisicas = convertir_a_lista_de_direcciones_fisicas(mem_request->direcciones_fisicas);
            t_mem_response response = escribir_memoria_usuario(mem_request->pid, direcciones_fisicas, mem_request->buffer, mem_request->tamanio_buffer);
            enviar_senial(response, fd_cpu);
            list_destroy_and_destroy_elements(direcciones_fisicas, &free);
            break;

        case RESIZE:
            printf("RESIZE \n");
            t_mem_response resultado = ajustar_memoria_para_proceso(mem_request->pid, mem_request->tamanio_nuevo);
            enviar_senial(resultado, fd_cpu);
            break;
        }

        destruir_cpu_mem_request(mem_request);
    }
}

void escuchar_interfaz_es(int32_t fd_es)
{
    printf("Interfaz E/S conectada \n");

    while (1)
    {
        t_io_mem_req *mem_request = recibir_io_mem_request(fd_es);
        if (mem_request == NULL)
            return;

        t_list *direcciones_fisicas;

        retardo_respuesta();
        switch (mem_request->operacion)
        {
        case LEER_IO:
            printf("LEER_IO \n");
            direcciones_fisicas = convertir_a_lista_de_direcciones_fisicas(mem_request->direcciones_fisicas);
            void *buffer_response = leer_memoria_usuario(mem_request->pid, direcciones_fisicas, mem_request->tamanio_buffer);
            responder_lectura(buffer_response, mem_request->tamanio_buffer, fd_es);
            free(buffer_response);
            break;

        case ESCRIBIR_IO:
            printf("ESCRIBIR_IO \n");
            direcciones_fisicas = convertir_a_lista_de_direcciones_fisicas(mem_request->direcciones_fisicas);
            t_mem_response response = escribir_memoria_usuario(mem_request->pid, direcciones_fisicas, mem_request->buffer, mem_request->tamanio_buffer);
            enviar_senial(response, fd_es);
            break;
        }

        destruir_io_mem_request(mem_request);
        list_destroy_and_destroy_elements(direcciones_fisicas, &free);
    }
}

static void responder_lectura(void *buffer, u_int32_t tamanio_buffer, int32_t fd_conexion)
{
    t_mem_response response = buffer == NULL ? OPERATION_FAILED : OPERATION_SUCCEED;
    t_mem_buffer_response *buffer_response = crear_buffer_response(response, buffer, tamanio_buffer);
    enviar_buffer_response(fd_conexion, buffer_response);
    destruir_buffer_response(buffer_response);
}

static void retardo_respuesta()
{
    usleep(get_retardo_respuesta() * 1000);
}
