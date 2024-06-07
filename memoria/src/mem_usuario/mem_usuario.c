#include "mem_usuario.h"

void * memoria_usuario;
t_list * lista_tablas;
t_bitarray * bitmap;

void inicializar_memoria_usuario()
{

    u_int32_t tamanio_memoria = get_tamanio_memoria();
    memoria_usuario = malloc(tamanio_memoria);
    
}


u_int32_t get_cantidad_frames()
{
    return get_tamanio_memoria() / get_tamanio_pagina();
}


void inicializar_lista_tablas_de_paginas() 
{ 
    lista_tablas = list_create();
}


void inicializar_bitmap()
{
    void * puntero_a_bits = malloc((get_cantidad_frames()/8) + 1);  //Esto del +1 es por si la cantidad de frames no es multiplo de 8 y necesito un byte mas aunque sobren bits
    bitmap = bitarray_create_with_mode(puntero_a_bits, (get_cantidad_frames()/8) + 1, MSB_FIRST);
}


void crear_tabla_de_paginas_para_proceso(u_int32_t pid)
{
    t_proceso_y_tabla * proceso_y_tabla = malloc(sizeof(t_proceso_y_tabla));
    t_list * tabla_paginas = list_create(); 
    proceso_y_tabla->pid = pid;
    proceso_y_tabla->tabla_paginas = tabla_paginas;
    list_add(lista_tablas, proceso_y_tabla); //Agrego la tabla de paginas asociada a un PID a la lista de tablas
   
}

void destruir_memoria_usuario()
{
    free(memoria_usuario);
}