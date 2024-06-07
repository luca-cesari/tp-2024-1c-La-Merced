#ifndef MEM_USUARIO_H
#define MEM_USUARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>

#include "config/config.h"



/**
 * @brief Inicializa la memoria de usuario, es decir el espacio de memoria dado por el archivo de configuracion
 **/
void inicializar_memoria_usuario();
u_int32_t get_cantidad_frames();

#endif // MEM_USUARIO_H