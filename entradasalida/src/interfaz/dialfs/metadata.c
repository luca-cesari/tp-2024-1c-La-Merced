#include "metadata.h"

/*
Creación de archivos
Al momento de crearse un archivo, va a comenzar ocupando un bloque del FS aunque su tamaño sea 0 y luego el mismo se podrá extender o disminuir por medio de de la función IO_FS_TRUNCATE.

El tercer tipo de archivo va a ser un archivo de metadata, del cual vamos a tener varios en nuestro FS y va a ser un archivo cuyo nombre va a ser el nombre del archivo en el FS,
por ej notas.txt y su contenido va a tener el bloque en el cual empieza el archivo y el tamaño del archivo en bytes:
BLOQUE_INICIAL=25
TAMANIO_ARCHIVO=1024
El contenido del archivo quiero tratarlo con las commons de config

*/

void crear_archivo_metadata(char *path_archivo, u_int32_t bloque_inicial, u_int32_t tamanio_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    config_set_value(archivo_metadata, "BLOQUE_INICIAL", string_itoa(bloque_inicial));
    config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_archivo));
    config_save(archivo_metadata);
    config_destroy(archivo_metadata);
}

u_int32_t get_bloque_inicial(char *path_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    u_int32_t bloque_inicial = config_get_int_value(archivo_metadata, "BLOQUE_INICIAL");
    config_destroy(archivo_metadata);
    return bloque_inicial;
}

u_int32_t get_cantidad_bloques_ocupados(char *path_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    u_int32_t tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
    config_destroy(archivo_metadata);
    return tamanio_archivo / get_block_size() + (tamanio_archivo % get_block_size() != 0);
}
