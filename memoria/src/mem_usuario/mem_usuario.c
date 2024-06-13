#include "mem_usuario.h"

void *memoria_usuario;
pthread_mutex_t memoria_usuario_mutex;

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica);

void inicializar_memoria_usuario()
{
    memoria_usuario = malloc(get_tamanio_memoria());
    inicializar_tabla_paginas();
    inicializar_bitmap_estados();
    pthread_mutex_init(&memoria_usuario_mutex, NULL);
}

t_mem_response ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio_nuevo)
{
    t_proceso_tabla *tabla_paginas = get_tabla_proceso(pid);
    u_int32_t tamanio_actual = get_cantidad_paginas(tabla_paginas) * get_tamanio_pagina();

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
        asignar_frame_a_tabla(tabla_paginas);

    log_ampliacion_proceso(tabla_paginas->pid, tamanio_actual, tamanio_nuevo);

    return OPERATION_SUCCEED;
}

t_mem_response reducir_memoria_para_proceso(t_proceso_tabla *tabla_paginas, u_int32_t tamanio_actual, u_int32_t tamanio_nuevo)
{
    u_int32_t cantidad_frames_a_liberar = tamanio_actual / get_tamanio_pagina() - tamanio_nuevo / get_tamanio_pagina();

    for (u_int32_t i = 0; i < cantidad_frames_a_liberar; i++)
        liberar_frame_de_tabla(tabla_paginas);

    log_reduccion_proceso(tabla_paginas->pid, tamanio_actual, tamanio_nuevo);

    return OPERATION_SUCCEED;
}

t_mem_response escribir_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, void *buffer, u_int32_t tamanio_buffer, int32_t fd)
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
            pthread_mutex_lock(&memoria_usuario_mutex);
            memcpy(memoria_usuario + (*direccion_fisica_a_recorrer), buffer, 1); // Va copiando byte por byte del buffer a la memoria
            pthread_mutex_unlock(&memoria_usuario_mutex);

            (*direccion_fisica_a_recorrer)++;
            buffer++;
            tamanio_guardado++;
        }
        i++;
    }

    if (tamanio_guardado == tamanio_buffer)
        log_acceso_espacio_usuario(pid, "ESCRIBIR", *(u_int32_t *)list_get(direcciones_fisicas, 0), tamanio_buffer);

    return tamanio_guardado == tamanio_buffer ? OPERATION_SUCCEED : OPERATION_FAILED;
}

void *leer_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, u_int32_t tamanio_buffer, int32_t fd)
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
            pthread_mutex_lock(&memoria_usuario_mutex);
            memcpy(buffer, memoria_usuario + (*direccion_fisica_a_recorrer), 1); // Va copiando byte por byte del buffer a la memoria
            pthread_mutex_unlock(&memoria_usuario_mutex);

            (*direccion_fisica_a_recorrer)++;
            buffer++;
            tamanio_leido++;
        }
    }
    if (ttamanio_leido == tamanio_buffer)
        log_acceso_espacio_usuario(pid, "LEER", *(u_int32_t *)list_get(direcciones_fisicas, 0), tamanio_buffer);

    return tamanio_leido == tamanio_buffer ? buffer : NULL;
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    destruir_bitmap_estados();
    destruir_lista_tablas();
    pthread_mutex_destroy(&memoria_usuario_mutex);
}

static u_int32_t get_numero_de_frame(u_int32_t direccion_fisica)
{
    return direccion_fisica / get_tamanio_pagina();
}
