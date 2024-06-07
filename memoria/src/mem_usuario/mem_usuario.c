#include "mem_usuario.h"

void *memoria_usuario;
t_list *lista_tablas;
t_estado_frame *bitmap;

void inicializar_memoria_usuario()
{
    u_int32_t tamanio_memoria = get_tamanio_memoria();
    memoria_usuario = malloc(tamanio_memoria);
}

u_int32_t get_cantidad_frames()
{
    return get_tamanio_memoria() / get_tamanio_pagina();
}

u_int32_t get_numero_de_frame(u_int32_t direccion_fisica) // No se si el tipo de dato de la direccion fisica será este
{
    return direccion_fisica / get_tamanio_pagina();
}

void inicializar_lista_tablas_de_paginas()
{
    lista_tablas = list_create();
}

void inicializar_bitmap()
{
    bitmap = malloc(get_cantidad_frames() * sizeof(t_estado_frame));
}

void modificar_bitmap(u_int32_t frame, t_estado_frame estado)
{
    bitmap[frame] = estado;
}

void crear_tabla_de_paginas_para_proceso(u_int32_t pid)
{
    t_proceso_y_tabla *proceso_y_tabla = malloc(sizeof(t_proceso_y_tabla));
    t_list *tabla_paginas = list_create();
    proceso_y_tabla->pid = pid;
    proceso_y_tabla->tabla_paginas = tabla_paginas;
    list_add(lista_tablas, proceso_y_tabla); // Agrego la tabla de paginas asociada a un PID a la lista de tablas
}

void ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio)
{
    t_proceso_y_tabla *proceso_y_tabla = list_get(lista_tablas, pid);
    u_int32_t tamanio_actual = list_size(proceso_y_tabla->tabla_paginas) * get_tamanio_pagina();
    u_int32_t tamanio_nuevo = tamanio;

    if (tamanio_nuevo > tamanio_actual)
    {
        ampliar_memoria_para_proceso(proceso_y_tabla, tamanio_nuevo);
    }
    else
    {
        reducir_memoria_para_proceso(proceso_y_tabla, tamanio_nuevo);
    }
}

void ampliar_memoria_para_proceso(t_proceso_y_tabla *proceso_y_tabla, u_int32_t tamanio_nuevo)
{
    u_int32_t tamanio_actual = list_size(proceso_y_tabla->tabla_paginas) * get_tamanio_pagina();
    u_int32_t cantidad_frames_necesarios = tamanio_nuevo / get_tamanio_pagina() - tamanio_actual / get_tamanio_pagina();
    u_int32_t cantidad_frames_disponibles = get_cantidad_frames_disponibles();

    if (cantidad_frames_necesarios > cantidad_frames_disponibles)
    {
        // ERROR OUT OF MEMORY
    }
    else
    {
        for (u_int32_t i = 0; i < cantidad_frames_necesarios; i++)
        {
            u_int32_t frame = get_frame_libre();
            modificar_bitmap(frame, OCUPADO);
            t_fila_tabla *fila_tabla = malloc(sizeof(t_fila_tabla));
            fila_tabla->nro_pagina = tamanio_actual / get_tamanio_pagina() + i;
            fila_tabla->nro_frame = frame;
            list_add(proceso_y_tabla->tabla_paginas, fila_tabla);
        }
    }
}

void reducir_memoria_para_proceso(t_proceso_y_tabla *proceso_y_tabla, u_int32_t tamanio_nuevo)
{
    u_int32_t tamanio_actual = list_size(proceso_y_tabla->tabla_paginas) * get_tamanio_pagina();
    u_int32_t cantidad_frames_a_liberar = tamanio_actual / get_tamanio_pagina() - tamanio_nuevo / get_tamanio_pagina();

    for (u_int32_t i = 0; i < cantidad_frames_a_liberar; i++)
    {
        t_fila_tabla *fila_tabla = list_get(proceso_y_tabla->tabla_paginas, list_size(proceso_y_tabla->tabla_paginas) - 1);
        modificar_bitmap(fila_tabla->nro_frame, LIBRE);
        list_remove(proceso_y_tabla->tabla_paginas, list_size(proceso_y_tabla->tabla_paginas) - 1);
        free(fila_tabla);
    }
}

u_int32_t get_frame_libre()
{
    for (u_int32_t i = 0; i < get_cantidad_frames(); i++)
    {
        if (bitmap[i] == LIBRE)
        {
            return i;
        }
    }
    return -1;
}

u_int32_t get_cantidad_frames_disponibles()
{
    u_int32_t cantidad_frames_disponibles = 0;
    for (u_int32_t i = 0; i < get_cantidad_frames(); i++)
    {
        if (bitmap[i] == LIBRE)
        {
            cantidad_frames_disponibles++;
        }
    }
    return cantidad_frames_disponibles;
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    free(bitmap);
}