#ifndef INTERRUPCION_H
#define INTERRUPCION_H

#include <stdlib.h>
#include <pthread.h>

void inicializar_interrupcion(void);

/**
 * @brief Setea la interrupción según el tipo.
 * 
 */
void set_interrupcion(int32_t);

/**
 * @brief Resetea la el estado de la interrupción.
 *        Debe ser usado previo a la ejecución de un nuevo proceso.
 * 
 */
void reset_interrupcion(void);

/**
 * @brief Obtiene la interrupción actual.
 * 
 * @return int8_t 
 * 
 * @note Si hay interrupción, devuelve el código de la interrupción,
 *       pudiendo ser 0 (QUANTUM) o 1 (PKILL).
 * @note Si no hay interrupción, devuelve -1.
 */
int8_t get_interrupcion(void);

#endif // INTERRUPCION_H