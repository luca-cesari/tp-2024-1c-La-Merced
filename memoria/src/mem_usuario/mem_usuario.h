#ifndef MEM_USUARIO_H
#define MEM_USUARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>

#include "config/config.h"

typedef struct
{
    u_int32_t nro_pagina;
    u_int32_t nro_frame;
    // No pongo ni bit de presencia ni demás porque no hay memoria virtual
} t_fila_tabla;

typedef struct
{
    u_int32_t pid;
    t_list *tabla_paginas;
} t_proceso_y_tabla;

typedef enum
{
    LIBRE,
    OCUPADO
} t_estado_frame;

/**
 * @brief Inicializa la memoria de usuario, es decir el espacio de memoria dado por el archivo de configuracion.
 **/
void inicializar_memoria_usuario();

/**
 * @brief Inicializa la lista de tablas de paginas de los procesos. Cada tabla de página va a estar asociada a un PID, es decir un proceso.
 **/
void inicializar_lista_tablas_de_paginas();

/**
 * @brief Inicializa el bitmap para saber que frames están ocupados.
 **/
void inicializar_bitmap();

/**
 * @brief Modifica el estado de un frame en el bitmap(LIBRE / OCUPADO).
 **/
void modificar_bitmap(u_int32_t frame, t_estado_frame estado);

u_int32_t get_cantidad_frames();

u_int32_t get_numero_de_frame(u_int32_t direccion_fisica);

void crear_tabla_de_paginas_para_proceso(u_int32_t pid);

void ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio);
/**
 * @brief Amplia memoria para un proceso en caso que el RESIZE sea mayor al espacio que ya tiene.
 **/
void ampliar_memoria_para_proceso(t_proceso_y_tabla *proceso_y_tabla, u_int32_t tamanio_nuevo);

/**
 * @brief Reduce memoria para un proceso en caso que el RESIZE sea menor al espacio que ya tiene.
 **/
void reducir_memoria_para_proceso(t_proceso_y_tabla *proceso_y_tabla, u_int32_t tamanio_nuevo);

u_int32_t get_frame_libre();

u_int32_t get_cantidad_frames_disponibles();

void destruir_memoria_usuario();

#endif // MEM_USUARIO_H