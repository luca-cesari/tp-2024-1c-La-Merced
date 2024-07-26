#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <commons/collections/list.h>
#include <pcb/pcb.h>
#include <commons/string.h>

#include "config/config.h"
#include "extended/string.h"

typedef struct
{
    int32_t pid;
    char *path;
    t_list *instrucciones;
} t_proceso_instrucciones;

/**
 * @brief Inicializa la memoria de instrucciones, es decir la lista de procesos
 */
void inicializar_memoria_instrucciones();

/**
 * @brief Carga un proceso a la memoria de instrucciones, es decir a una lista
 * de procesos(ver enunciado en la parte de memoria de instrucciones y ver issue #3627)
 *
 * @param pid
 * @param path
 */
void cargar_proceso_a_memoria(int32_t pid, char *path);

/**
 * @brief Lee las instrucciones de un archivo de pseudocodigo y las guarda en una lista
 *
 * @param path
 * @return t_list*
 */
t_list *leer_instrucciones(char *path);

/**
 * @brief
 *
 * @param pcb
 */
void eliminar_proceso(u_int32_t pid);

/**
 * @brief
 *
 * @param elemento
 */
void destruir_proceso_instrucciones(void *elemento);

/**
 * @brief
 *
 * @param pid
 * @param program_counter
 * @return `char*`
 */
char *proxima_instruccion(u_int32_t pid, int32_t program_counter);

#endif // INSTRUCCIONES_H