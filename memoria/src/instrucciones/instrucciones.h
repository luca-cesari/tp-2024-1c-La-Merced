#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "commons/collections/list.h"

typedef struct{
    int32_t num_instruccion;
    char * instruccion;
}t_instruccion;

typedef struct
{
    int32_t pid;
    char* path;
    t_list* instrucciones;
} t_proceso_instrucciones;


/**
 * @brief Inicializa la memoria de instrucciones, es decir la lista de procesos
 **/

void inicializar_memoria_instrucciones();

/**
 * @brief Carga un proceso a la memoria de instrucciones, es decir a una lista
 * de procesos(ver enunciado en la parte de memoria de instrucciones y ver issue #3627)
 * 
 * @param pid 
 * @param path 
 */

void cargar_proceso_a_memoria(int32_t pid, char* path);

/**
 * @brief Lee las instrucciones de un archivo de pseudocodigo y las guarda en una lista
 * 
 * @param path 
 * @return t_list* 
 */
t_list* leer_instrucciones(char* path);
void eliminar_proceso(t_pcb *pcb);
t_instruccion* proxima_instruccion(t_pcb *pcb);

#endif // INSTRUCCIONES_H