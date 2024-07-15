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

t_list *lista_archivos;

char *path_archivo_comparar;

void actualizar_lista_archivos_metadata()
{
    /*Habria que leer el directorio y cargar los archivos en la lista*/
    lista_archivos = list_create();
    cargar_lista_archivos_persistidos(get_path_base_dialfs(), lista_archivos);
}

void crear_archivo_metadata(char *path_archivo, u_int32_t bloque_inicial, u_int32_t tamanio_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    config_set_value(archivo_metadata, "BLOQUE_INICIAL", string_itoa(bloque_inicial));
    config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_archivo));
    config_save(archivo_metadata);

    metadata *archivo = malloc(sizeof(metadata));
    archivo->path_archivo = path_archivo;
    archivo->bloque_inicial = bloque_inicial;
    archivo->tamanio_archivo = tamanio_archivo;

    list_add_sorted(lista_archivos, archivo, comparar_bloque_inicial);

    config_destroy(archivo_metadata);
}

u_int32_t get_bloque_inicial(char *path_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    u_int32_t bloque_inicial = config_get_int_value(archivo_metadata, "BLOQUE_INICIAL");
    config_destroy(archivo_metadata);
    return bloque_inicial;
}

u_int32_t get_tamanio_archivo(char *path_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    u_int32_t tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
    config_destroy(archivo_metadata);
    return tamanio_archivo;
}

u_int32_t get_cantidad_bloques_ocupados(char *path_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    u_int32_t tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");
    config_destroy(archivo_metadata);
    return tamanio_archivo / get_block_size() + (tamanio_archivo % get_block_size() != 0);
}

void set_tamanio_archivo(char *path_archivo, u_int32_t tamanio_archivo)
{
    t_config *archivo_metadata = config_create(path_archivo);
    config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", string_itoa(tamanio_archivo));
    config_save(archivo_metadata);
    config_destroy(archivo_metadata);
}

void cargar_lista_archivos_persistidos(const char *path_directorio, t_list *lista_archivos)
{
    DIR *dir = opendir(path_directorio); // Abre el directorio
    if (!dir)
    {
        perror("No se pudo abrir el directorio");
        exit(EXIT_FAILURE);
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    { // Lee cada entrada en el directorio
        // Ignorar "." y ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "bloques.dat") != 0 && strcmp(entry->d_name, "bitmap.dat") != 0)
        {
            //
            char *path_archivo_a_leer = string_from_format("%s/%s", path_directorio, entry->d_name);
            //

            FILE *archivo_metadata = fopen(path_archivo_a_leer, "r");
            if (archivo_metadata != NULL)
            {
                metadata *archivo = malloc(sizeof(metadata));
                archivo->path_archivo = path_archivo_a_leer;
                archivo->bloque_inicial = get_bloque_inicial(path_archivo_a_leer);
                archivo->tamanio_archivo = get_tamanio_archivo(path_archivo_a_leer);

                // Quiero agregarlo a la lista de archivos ordenadamente a partir del bloque incial, de menor a mayor
                list_add_sorted(lista_archivos, archivo, comparar_bloque_inicial);
            }

            fclose(archivo_metadata);
            // list_add(fileList, entry->d_name);
        }
    }

    closedir(dir); // Cierra el directorioS
}

bool comparar_bloque_inicial(void *archivo1, void *archivo2)
{
    metadata *archivo1_cast = (metadata *)archivo1;
    metadata *archivo2_cast = (metadata *)archivo2;

    return archivo1_cast->bloque_inicial < archivo2_cast->bloque_inicial;
}

void eliminar_archivo_metadata(char *path_archivo)
{
    if (remove(path_archivo) != 0)
    {
        perror("Error al borrar el archivo");
        free(path_archivo);
        // enviar_respuesta(pid, FILE_DELETION_ERROR); VER PARA MANDAR AL KERNEL
        return;
    }

    path_archivo_comparar = strdup(path_archivo);

    list_remove_by_condition(lista_archivos, tiene_mismo_path);
}

bool tiene_mismo_path(void *archivo)
{
    metadata *archivo_cast = (metadata *)archivo;
    return strcmp(archivo_cast->path_archivo, path_archivo_comparar) == 0;
}

u_int32_t obtener_indice_archivo(char *path_archivo)
{

    for (int i = 0; i < list_size(lista_archivos); i++)
    {
        metadata *archivo = list_get(lista_archivos, i);
        if (strcmp(archivo->path_archivo, path_archivo) == 0)
        {
            return i;
        }
    }
    perror("No se encontro el archivo");
    return -1; // Puede darse el caso de que esto este mal
}

u_int32_t obtener_tamanio_lista_archivos()
{
    return list_size(lista_archivos);
}

char *get_path_archivo_por_indice(u_int32_t indice)
{
    metadata *archivo = list_get(lista_archivos, indice);
    return strdup(archivo->path_archivo);
}

void actualizar_archivo_metadata(char *path_archivo, u_int32_t nuevo_bloque_inicial)
{
    t_config *archivo_metadata = config_create(path_archivo);
    config_set_value(archivo_metadata, "BLOQUE_INICIAL", string_itoa(nuevo_bloque_inicial));

    ((metadata *)list_get(lista_archivos, obtener_indice_archivo(path_archivo)))->bloque_inicial = nuevo_bloque_inicial;

    config_save(archivo_metadata);
    config_destroy(archivo_metadata);
}

void ordenar_lista_archivos(u_int32_t indice)
{
    list_add(lista_archivos, list_get(lista_archivos, indice));
    list_remove(lista_archivos, indice);
}