#ifndef MEM_USUARIO_H
#define MEM_USUARIO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <commons/collections/list.h>
#include <protocol/protocol.h>

#include "config/config.h"
#include "estado_frame/estado_frame.h"

typedef struct
{
    u_int32_t pid;

    // posición de lista: nro_pag
    // contenido de lista: nro_frame
    t_list *lista_frames;
} t_proceso_tabla;

/**
 * @brief Función constructor de la memoria de usuario.
 *        Inicializa la memoria de usuario con el espacio
 *        especificado por el archivo de configuración.
 */
void inicializar_memoria_usuario(void);

/**
 * @brief Crear una tabla de paginas vacías para un proceso.
 *        Función llamada ante una petición de `INICIAR_PROCESO`.
 *
 * @param pid
 */
void crear_tabla_de_paginas_para_proceso(u_int32_t pid);

/**
 * @brief Destruye la tabla de paginas de un proceso.
 *        Función llamada ante una petición de `FINALIZAR_PROCESO`.
 *
 * @param pid
 */
void destruir_tabla_de_paginas_para_proceso(u_int32_t pid);

/**
 * @brief Retorna el número de frame (marco) correspondiente
 *        a la página del proceso solicitado.
 * 
 * @param pid 
 * @param nro_pag 
 * @return `u_int32_t`
 */
u_int32_t obtener_marco(u_int32_t pid, u_int32_t nro_pag);

/**
 * @brief Función llamada ante una petición de `RESIZE`.
 *        Evalúa el tamaño solicitado respecto del actual y,
 *        amplía o reduce según resultado.
 *
 * @param pid
 * @param tamanio
 * @return `int8_t` : 0 si se pudo ajustar, -1 en caso de error.
 */
int8_t ajustar_memoria_para_proceso(u_int32_t pid, u_int32_t tamanio);

/**
 * @brief Amplia memoria para un proceso.
 *        Si la cantidad de memoria solicitada excede la cantidad
 *        de memoria disponible, retorna -1.
 *
 * @param tabla_de_paginas
 * @param tamanio_nuevo
 * @return `int8_t` : 0 si se pudo ampliar, -1 en caso de error.
 */
int8_t ampliar_memoria_para_proceso(t_proceso_tabla *tabla_de_paginas, u_int32_t tamanio_nuevo);

/**
 * @brief Reduce memoria para un proceso.
 *
 * @param tabla_de_paginas
 * @param tamanio_nuevo
 * @return `int8_t` : 0 si se pudo reducir, -1 en caso de error.
 */
int8_t reducir_memoria_para_proceso(t_proceso_tabla *tabla_de_paginas, u_int32_t tamanio_nuevo);

void escribir_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, void *buffer, u_int32_t tamanio, int32_t fd);

void leer_memoria_usuario(u_int32_t pid, t_list *direcciones_fisicas, u_int32_t tamanio, int32_t fd);

/**
 * @brief Función destructor de la memoria de usuario.
 */
void destruir_memoria_usuario();

#endif // MEM_USUARIO_H