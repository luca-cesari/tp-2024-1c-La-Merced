#ifndef TABLA_PAGINAS_H
#define TABLA_PAGINAS_H

#include <stdlib.h>
#include <mlist/mlist.h>

#include "config/config.h"
#include "estado_frame/estado_frame.h"
#include "logger/logger.h"

typedef struct
{
   u_int32_t pid;

   // posición de lista: nro_pag
   // contenido de lista: nro_frame
   t_mutext_list *lista_frames;
} t_proceso_tabla;

/**
 * @brief Inicializa la lista de tablas de páginas.
 *
 */
void inicializar_tabla_paginas(void);

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
 * @brief Retorna la cantidad de páginas que tiene una tabla.
 *
 * @param tabla_paginas
 * @return `u_int32_t`
 */
u_int32_t get_cantidad_paginas(t_proceso_tabla *tabla_paginas);

/**
 * @brief Asigna un frame a la tabla de páginas.
 *
 * @param tabla_paginas
 */
void asignar_frame_a_tabla(t_proceso_tabla *tabla_paginas);

/**
 * @brief Libera un frame de la tabla de páginas.
 *
 * @param tabla_paginas
 * @param nro_frame
 */
void liberar_frame_de_tabla(t_proceso_tabla *tabla_paginas);

/**
 * @brief Devuelve la tabla de páginas correspondiente al proceso.
 *
 * @param pid
 * @return t_proceso_tabla*
 */
t_proceso_tabla *get_tabla_proceso(u_int32_t pid);

/**
 * @brief Retorna el número de frame (marco) correspondiente
 *        a la página del proceso solicitado.
 *
 * @param pid
 * @param nro_pag
 * @return `u_int32_t`
 *
 * @note Si no se encuentra el marco, retorna -1.
 */
u_int32_t obtener_marco(u_int32_t pid, u_int32_t nro_pag);

/**
 * @brief Función destructor de la lista de tablas de páginas.
 *
 */
void destruir_lista_tablas(void);

#endif // TABLA_PAGINAS_H