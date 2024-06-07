#include "mem_usuario.h"

void * memoria_usuario;
t_list * lista_tablas;
t_estado_frame * bitmap;


void inicializar_memoria_usuario()
{

    u_int32_t tamanio_memoria = get_tamanio_memoria();
    memoria_usuario = malloc(tamanio_memoria);
    
}


u_int32_t get_cantidad_frames()
{
    return get_tamanio_memoria() / get_tamanio_pagina();
}

u_int32_t get_numero_de_frame(u_int32_t direccion_fisica) //No se si el tipo de dato de la direccion fisica será este
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
    t_proceso_y_tabla * proceso_y_tabla = malloc(sizeof(t_proceso_y_tabla));
    t_list * tabla_paginas = list_create(); 
    proceso_y_tabla->pid = pid;
    proceso_y_tabla->tabla_paginas = tabla_paginas;
    list_add(lista_tablas, proceso_y_tabla); //Agrego la tabla de paginas asociada a un PID a la lista de tablas
   
}



void ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio)
{
    t_proceso_y_tabla * proceso_y_tabla = list_get(lista_tablas, pid);
    t_list * tabla_paginas = proceso_y_tabla->tabla_paginas;
    u_int32_t cantidad_frames = tamanio / get_tamanio_pagina();
    u_int32_t frames_ocupados = 0;
    u_int32_t frame = 0;
    u_int32_t i = 0;
    while(frames_ocupados < cantidad_frames)
    {
        if(bitmap[frame] == LIBRE)
        {
            t_fila_tabla * fila_tabla = malloc(sizeof(t_fila_tabla));
            fila_tabla->nro_pagina = i;
            fila_tabla->nro_frame = frame;
            list_add(tabla_paginas, fila_tabla);
            modificar_bitmap(frame, OCUPADO);
            frames_ocupados++;
        }
        frame++;
    }
}


void destruir_memoria_usuario()
{
    free(memoria_usuario);
    free(bitmap);
}