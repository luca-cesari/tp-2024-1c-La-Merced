void * memoria_usuario;



void inicializar_memoria_usuario(){

    u_int32_t tamanio_memoria = get_tamanio_memoria();
    memoria_usuario = malloc(tamanio_memoria);
    
}


u_int32_t get_cantidad_frames(){
    return get_tamanio_memoria() / get_tamanio_pagina();
}