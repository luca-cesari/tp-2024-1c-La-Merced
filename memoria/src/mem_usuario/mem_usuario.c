#include "mem_usuario.h"

void *memoria_usuario;

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica);

void inicializar_memoria_usuario()
{
    memoria_usuario = malloc(get_tamanio_memoria());
    inicializar_tabla_paginas();
    inicializar_bitmap_estados();
}

t_mem_response ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio_nuevo)
{
    t_proceso_tabla *tabla_paginas = get_tabla_proceso(pid);
    u_int32_t tamanio_actual = list_size(tabla_paginas->lista_frames) * get_tamanio_pagina();

    return tamanio_nuevo > tamanio_actual
               ? ampliar_memoria_para_proceso(tabla_paginas, tamanio_actual, tamanio_nuevo)
               : reducir_memoria_para_proceso(tabla_paginas, tamanio_actual, tamanio_nuevo);
}

t_mem_response ampliar_memoria_para_proceso(t_proceso_tabla *tabla_paginas, u_int32_t tamanio_actual, u_int32_t tamanio_nuevo)
{
    u_int32_t cantidad_frames_necesarios = tamanio_nuevo / get_tamanio_pagina() - tamanio_actual / get_tamanio_pagina();
    u_int32_t cantidad_frames_disponibles = get_cantidad_frames_disponibles();

    if (cantidad_frames_necesarios > cantidad_frames_disponibles)
        return OPERATION_FAILED;

    for (u_int32_t i = 0; i < cantidad_frames_necesarios; i++)
    {
        u_int32_t frame = get_frame_libre();
        set_estado_frame(frame, OCUPADO);
        u_int32_t *nro_frame = malloc(sizeof(u_int32_t));
        *nro_frame = frame;
        list_add(tabla_paginas->lista_frames, nro_frame);
    }

    return OPERATION_SUCCEED;
}

t_mem_response reducir_memoria_para_proceso(t_proceso_tabla *tabla_paginas, u_int32_t tamanio_actual, u_int32_t tamanio_nuevo)
{
    u_int32_t cantidad_frames_a_liberar = tamanio_actual / get_tamanio_pagina() - tamanio_nuevo / get_tamanio_pagina();

    for (u_int32_t i = 0; i < cantidad_frames_a_liberar; i++)
    {
        u_int32_t *nro_frame = list_get(tabla_paginas->lista_frames, list_size(tabla_paginas->lista_frames) - 1);
        set_estado_frame(*nro_frame, LIBRE);
        list_remove(tabla_paginas->lista_frames, list_size(tabla_paginas->lista_frames) - 1);
    }

    return OPERATION_SUCCEED;
}

/*
Acceso a espacio de usuario
Esta petición puede venir tanto de la CPU como de un Módulo de Interfaz de I/O, es importante tener en cuenta que las peticiones pueden ocupar más de una página.
El módulo Memoria deberá realizar lo siguiente:
Ante un pedido de lectura, devolver el valor que se encuentra a partir de la dirección física pedida.
Ante un pedido de escritura, escribir lo indicado a partir de la dirección física pedida. En caso satisfactorio se responderá un mensaje de ‘OK’.
*/

void escribir_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, void *buffer, u_int32_t tamanio_buffer, int32_t fd)
{
    /*Se tiene en cuenta que se puede pedir escribir más de una página, por lo que esta función recibe más de una dirección fisica
    ya que antes se obtuvieron los marcos correspondientes*/

    u_int32_t frame;
    u_int32_t limite_de_frame;
    u_int32_t tamanio_guardado = 0;
    u_int32_t *direccion_fisica_a_recorrer;
    int i = 0;

    while (tamanio_guardado <= tamanio_buffer)
    {

        direccion_fisica_a_recorrer = list_get(direcciones_fisicas, i);
        frame = get_numero_de_frame(*direccion_fisica_a_recorrer);
        limite_de_frame = frame * get_tamanio_pagina() + get_tamanio_pagina();

        while ((*direccion_fisica_a_recorrer < limite_de_frame) && (tamanio_guardado <= tamanio_buffer))
        {
            memcpy(memoria_usuario + (*direccion_fisica_a_recorrer), buffer, 1); // Va copiando byte por byte del buffer a la memoria
            (*direccion_fisica_a_recorrer)++;
            buffer++;
            tamanio_guardado++;
        }
        i++;
    }

    return tamanio_guardado == tamanio_buffer
               ? enviar_senial(OPERATION_SUCCEED, fd)
               : enviar_senial(OPERATION_FAILED, fd);
}

void leer_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, u_int32_t tamanio_buffer, int32_t fd)
{
    /*Se tiene en cuenta que se puede pedir escribir más de una página, por lo que esta función recibe más de una dirección fisica
    ya que antes se obtuvieron los marcos correspondientes*/
    u_int32_t *direccion_fisica_a_recorrer;
    u_int32_t frame;
    u_int32_t limite_de_frame;
    u_int32_t tamanio_leido = 0;
    void *buffer = malloc(tamanio_buffer);

    while (tamanio_leido <= tamanio_buffer)
    {

        direccion_fisica_a_recorrer = list_get(direcciones_fisicas, 0);
        frame = get_numero_de_frame(*direccion_fisica_a_recorrer);
        limite_de_frame = frame * get_tamanio_pagina() + get_tamanio_pagina();

        while ((*direccion_fisica_a_recorrer < limite_de_frame) && (tamanio_leido <= tamanio_buffer))
        {
            memcpy(buffer, memoria_usuario + (*direccion_fisica_a_recorrer), 1); // Va copiando byte por byte del buffer a la memoria
            (*direccion_fisica_a_recorrer)++;
            buffer++;
            tamanio_leido++;
        }
    }

    if (tamanio_leido == tamanio_buffer)
    {
        // DEVOLVER BUFFER A CPU (la cpu se debe encargar de castearlo ya que puede ser un int32 o int8) O STRING A INTERFAZ DE I/O
        t_packet *paquete = crear_paquete();
        crear_buffer(paquete);
        agregar_a_paquete(paquete, buffer, tamanio_buffer);
        enviar_paquete(paquete, fd);
        eliminar_paquete(paquete);
    }
    else
    {
        // ERROR
    }
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    destruir_bitmap_estados();
    destruir_tabla_paginas();
}

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica)
{
    return direccion_fisica / get_tamanio_pagina();
}
