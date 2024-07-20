#ifndef METADATA_H
#define METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <commons/bitarray.h>
#include <sockets/sockets.h>
#include <dirent.h>

#include "config/config.h"

typedef struct
{
    char *path_archivo;
    u_int32_t bloque_inicial;
    u_int32_t tamanio_archivo;
} metadata;

void actualizar_lista_archivos_metadata();

void crear_archivo_metadata(char *path_archivo, u_int32_t bloque_inicial, u_int32_t tamanio_archivo);
void crear_archivo_metadata_vacio(char *path_archivo);
/**
 * @brief Obtiene el bloque inicial de un archivo a partir de su metadata
 */
u_int32_t get_bloque_inicial(char *path_archivo);
/**
 * @brief Obtiene la cantidad de bloques ocupados por un archivo a partir de su metadata
 */
u_int32_t get_tamanio_archivo(char *path_archivo);
u_int32_t get_cantidad_bloques_ocupados(char *path_archivo);

void set_tamanio_archivo(char *path_archivo, u_int32_t tamanio_archivo);

void cargar_lista_archivos_persistidos(const char *directoryPath, t_list *lista_archivos);

bool comparar_bloque_inicial(void *, void *);

void eliminar_archivo_metadata(char *path_archivo);

bool tiene_mismo_path(void *);

u_int32_t obtener_indice_archivo(char *path_archivo);
u_int32_t obtener_tamanio_lista_archivos();
char *get_path_archivo_por_indice(u_int32_t indice);
void actualizar_archivo_metadata(char *path_archivo, u_int32_t nuevo_bloque_inicial);
void ordenar_lista_archivos(u_int32_t indice);

#endif