#ifndef MEM_USUARIO_H
#define MEM_USUARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>

#include "config/config.h"

typedef struct{

    u_int32_t nro_pagina;
    u_int32_t nro_frame;
    //No pongo ni bit de presencia ni demás porque no hay memoria virtual
}t_fila_pagina;

typedef struct{

    u_int32_t pid;
    t_fila_pagina *tabla_paginas;

}t_lista_tablas;

/**
 * @brief Inicializa la memoria de usuario, es decir el espacio de memoria dado por el archivo de configuracion.
 **/
void inicializar_memoria_usuario();

/**
 * @brief Inicializa la lista de tablas de paginas de los procesos. Cada tabla de página va a estar asociada a un PID, es decir un proceso.
 **/
void inicializar_lista_tabla_paginas();

u_int32_t get_cantidad_frames();

#endif // MEM_USUARIO_H