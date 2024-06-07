#include "mem_usuario.h"

void * memoria_usuario;
t_lista_tablas * lista_tablas;

void inicializar_memoria_usuario(){

    u_int32_t tamanio_memoria = get_tamanio_memoria();
    memoria_usuario = malloc(tamanio_memoria);
    
}


u_int32_t get_cantidad_frames(){
    return get_tamanio_memoria() / get_tamanio_pagina();
}


void inicializar_lista_tabla_paginas(){
    lista_tablas = list_create();
}