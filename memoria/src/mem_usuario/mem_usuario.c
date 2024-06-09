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

void destruir_tabla_de_paginas_para_proceso(u_int32_t pid)
{
    t_proceso_y_tabla *proceso_y_tabla = obtener_tabla_segun_proceso(pid);

    t_list_iterator *iterador = list_iterator_create(proceso_y_tabla->tabla_paginas);
    while (list_iterator_has_next(iterador))
    {
        u_int32_t *nro_frame = (u_int32_t *)list_iterator_next(iterador);
        modificar_bitmap(*nro_frame, LIBRE);
    }

    list_destroy_and_destroy_elements(proceso_y_tabla->tabla_paginas, free);

    list_iterator_destroy(iterador);
    free(proceso_y_tabla);
}

void ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio)
{

    t_proceso_y_tabla *proceso_y_tabla = obtener_tabla_segun_proceso(pid);
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
            u_int32_t *nro_frame = malloc(sizeof(u_int32_t));
            list_add(proceso_y_tabla->tabla_paginas, nro_frame);
        }
    }
}

void reducir_memoria_para_proceso(t_proceso_y_tabla *proceso_y_tabla, u_int32_t tamanio_nuevo)
{
    u_int32_t tamanio_actual = list_size(proceso_y_tabla->tabla_paginas) * get_tamanio_pagina();
    u_int32_t cantidad_frames_a_liberar = tamanio_actual / get_tamanio_pagina() - tamanio_nuevo / get_tamanio_pagina();

    for (u_int32_t i = 0; i < cantidad_frames_a_liberar; i++)
    {
        u_int32_t *nro_frame = list_get(proceso_y_tabla->tabla_paginas, list_size(proceso_y_tabla->tabla_paginas) - 1);
        modificar_bitmap(*nro_frame, LIBRE);
        list_remove(proceso_y_tabla->tabla_paginas, list_size(proceso_y_tabla->tabla_paginas) - 1);
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
/*
Acceso a espacio de usuario
Esta petición puede venir tanto de la CPU como de un Módulo de Interfaz de I/O, es importante tener en cuenta que las peticiones pueden ocupar más de una página.
El módulo Memoria deberá realizar lo siguiente:
Ante un pedido de lectura, devolver el valor que se encuentra a partir de la dirección física pedida.
Ante un pedido de escritura, escribir lo indicado a partir de la dirección física pedida. En caso satisfactorio se responderá un mensaje de ‘OK’.
*/

void escribir_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, void *buffer, u_int32_t tamanio_buffer, int32_t fd)
{
    u_int32_t frame = get_numero_de_frame(direccion_fisica);
    u_int32_t limite_de_frame = frame * get_tamanio_pagina() + get_tamanio_pagina();
    u_int32_t tamanio_guardado = 0;

    while ((direccion_fisica < limite_de_frame) && (tamanio_guardado < tamanio_buffer))
    {
        memcpy(memoria_usuario + direccion_fisica, buffer, 1); // Va copiando byte por byte del buffer a la memoria
        direccion_fisica++;
        buffer++;
    }
    if (tamanio_guardado == tamanio_buffer)
    {
        // RESPONDER OK A CPU O INTERFAZ DE I/O
        enviar_mensaje("OK", fd);
    }
    else
    {
        // LOGICA PARA SEGUIR ESCRIBIENDO
        // u_int32_t nro_pag_sig = obtener_numero_pagina_siguiente(pid, frame);
        // LOGICA PARA DEVOLVER A CPU EL NUMERO DE PAGINA SIGUIENTE
    }
}

void leer_memoria_usuario(u_int32_t pid, u_int32_t direccion_fisica, u_int32_t tamanio_buffer, int32_t fd)
{
    u_int32_t frame = get_numero_de_frame(direccion_fisica);
    u_int32_t limite_de_frame = frame * get_tamanio_pagina() + get_tamanio_pagina();
    u_int32_t tamanio_leido = 0;
    void *buffer = malloc(tamanio_buffer);

    while ((direccion_fisica < limite_de_frame) && (tamanio_leido < tamanio_buffer))
    {
        memcpy(buffer, memoria_usuario + direccion_fisica, 1); // Va copiando byte por byte del buffer a la memoria
        direccion_fisica++;
        buffer++;
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
        // LOGICA PARA SEGUIR LEYENDO
        // u_int32_t nro_pag_sig = obtener_numero_pagina_siguiente(pid, frame);
        // LOGICA PARA DEVOLVER A CPU EL NUMERO DE PAGINA SIGUIENTE
    }
}

t_proceso_y_tabla *obtener_tabla_segun_proceso(u_int32_t pid)
{
    int es_tabla_buscada(void *elemento)
    {
        t_proceso_y_tabla *tabla = (t_proceso_y_tabla *)elemento;
        return tabla->pid == pid;
    };
    return list_find(lista_tablas, (void *)es_tabla_buscada);
}

u_int32_t obtener_marco(u_int32_t pid, u_int32_t nro_pag)
{

    t_proceso_y_tabla *proceso_tabla = obtener_tabla_segun_proceso(pid);
    u_int32_t *valor = (u_int32_t *)list_get(proceso_tabla->tabla_paginas, nro_pag);
    if (valor == NULL)
    {
        // ES NECESARIO SIGNO DE PREGUNTA
        printf("No tiene marco asignado");
    }
    return *valor;
}

u_int32_t obtener_numero_pagina_siguiente(u_int32_t pid, u_int32_t frame)
{
    t_proceso_y_tabla *proceso_y_tabla = obtener_tabla_segun_proceso(pid);
    u_int32_t nro_pagina_actual;

    t_list_iterator *iterador = list_iterator_create(proceso_y_tabla->tabla_paginas);

    while (list_iterator_has_next(iterador))
    {
        u_int32_t *nro_frame = (u_int32_t *)list_iterator_next(iterador);
        if (*nro_frame == frame)
        {
            nro_pagina_actual = list_iterator_index(iterador);
            break;
        }
    }

    list_iterator_destroy(iterador);

    return nro_pagina_actual + 1;
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
    free(bitmap);
}