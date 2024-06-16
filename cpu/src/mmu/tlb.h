#ifndef TLB_H
#define TLB_H

#include <stdlib.h>

#include "logger/logger.h"
#include "config/config.h"

typedef struct
{
   u_int32_t pid;
   u_int32_t pagina;
   u_int32_t marco;
} entrada_tlb;

/**
 * @brief Inicializa las entradas según la cantidad indicado por configuración.
 *        Si la cantidad de entradas es 0, retorna 0 implicando que la TLB está desactivada.
 *
 * @return `u_int8_t` : 1 si se inicializó correctamente, 0 en caso contrario.
 */
u_int8_t inicializar_tlb(void);

/**
 * @brief
 *
 * @param pid
 * @param pagina
 * @param marco
 */
void cargar_nueva_entrada(u_int32_t pid, u_int32_t pagina, u_int32_t marco);

/**
 * @brief
 *
 * @param pid
 * @param numero_pagina
 * @return `int32_t`
 */
int32_t get_marco(u_int32_t pid, u_int32_t numero_pagina);

/**
 * @brief Destruye la TLB liberando el array alocado.
 */
void destruir_tlb(void);

#endif // TLB_H