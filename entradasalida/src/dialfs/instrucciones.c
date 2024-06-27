#include "instrucciones.h"

t_dictionary *dicc_instrucciones;

void inicializar_dicc_instrucciones()
{
    dicc_instrucciones = dictionary_create();
    dictionary_put(dicc_instrucciones, "IO_FS_CREATE", &io_fs_create);
    dictionary_put(dicc_instrucciones, "IO_FS_DELETE", &io_fs_delete);
    dictionary_put(dicc_instrucciones, "IO_FS_TRUNCATE", &io_fs_truncate);
    // dictionary_put(dicc_instrucciones, "IO_FS_WRITE", &io_fs_write);
    // dictionary_put(dicc_instrucciones, "IO_FS_READ", &io_fs_read);
}

void (*get_funcion_instruccion(char *instruccion))(char *, u_int32_t)
{
    if (!dictionary_has_key(dicc_instrucciones, instruccion))
        return NULL;

    return dictionary_get(dicc_instrucciones, instruccion);
}

void io_fs_create(char *argumentos, u_int32_t pid)
{
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), argumentos); // Pensando que es el único argumento que viene
    modificar_bitmap(get_siguiente_bloque_libre(), OCUPADO);
    crear_archivo_metadata(path_archivo, get_siguiente_bloque_libre(), 0);
    free(path_archivo);
    // enviar_respuesta(pid, FILE_CREATED); VER PARA MANDAR AL KERNEL
}

void io_fs_delete(char *argumentos, u_int32_t pid)
{
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), argumentos); // Pensando que es el único argumento que viene
    FILE *archivo = fopen(path_archivo, "r");
    if (archivo == NULL)
    {
        free(path_archivo);
        // enviar_respuesta(pid, FILE_NOT_FOUND); VER PARA MANDAR AL KERNEL
        return;
    }

    fclose(archivo);
    if (remove(path_archivo) != 0)
    {
        perror("Error al borrar el archivo");
        free(path_archivo);
        // enviar_respuesta(pid, FILE_DELETION_ERROR); VER PARA MANDAR AL KERNEL
        return;
    }

    get_cantidad_bloques_ocupados(path_archivo);
    for (int i = 0; i < get_cantidad_bloques_ocupados(path_archivo); i++) // Va recorriendo los bloques ocupados y los va liberando ya que son contiguos
    {
        modificar_bitmap(get_bloque_inicial(path_archivo) + i, LIBRE);
    }
    // free(path_archivo);

    // Supongo que el archivo de bloques no hay que liberarlo sino que simplemente se pierde la referencia
    //  enviar_respuesta(pid, FILE_DELETED); VER PARA MANDAR AL KERNEL
}

void io_fs_truncate(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t nuevo_tamanio = atoi(parametros[1]);
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), parametros[0]);
    t_config *archivo_metadata = config_create(path_archivo);
    if (archivo_metadata == NULL)
    {
        // Manejo de error si no se puede abrir el archivo de metadatos
        perror("Error al abrir el archivo de metadatos");
        return;
    }
    u_int32_t tamanio_archivo = config_get_int_value(archivo_metadata, "TAMANIO_ARCHIVO");

    u_int32_t cantidad_bloques_necesarios = nuevo_tamanio / get_block_size() + (nuevo_tamanio % get_block_size() != 0);
    u_int32_t bloques_ocupados = get_cantidad_bloques_ocupados(path_archivo);

    if (nuevo_tamanio > tamanio_archivo)
    {
        u_int32_t bloques_faltantes = cantidad_bloques_necesarios - bloques_ocupados;
        // aca se podria hacer compactacion, y llevarlo al  final asi queda contiguo
        for (int i = 0; i < bloques_faltantes; i++)
        {
            if (get_siguiente_bloque_libre() != -1)
            {
                modificar_bitmap(get_bloque_inicial(path_archivo) + get_siguiente_bloque_libre(), OCUPADO);
            }
        }
    }
    else
    {
        for (int i = get_cantidad_bloques_ocupados(path_archivo); i > cantidad_bloques_necesarios; i--) // Va recorriendo los bloques ocupados y los va liberando ya que son contiguos
        {
            modificar_bitmap(get_bloque_inicial(path_archivo) + i, LIBRE);
        }
        // aca se podria hacer compactacion
    }

    config_set_value(archivo_metadata, "TAMANIO_ARCHIVO", parametros[1]);
    config_save(archivo_metadata);
    config_destroy(archivo_metadata);
}

/*
void io_fs_write(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t tamanio_valor = atoi(parametros[2]);
    char *direcciones_fisicas = array_a_string(parametros[1]);

    parametros_io parametros_leer;
    parametros_leer.param_leer.direcciones_fisicas = direcciones_fisicas;
    parametros_leer.param_leer.tamanio_buffer = tamanio_valor;

    t_io_mem_req *mem_request = crear_io_mem_request(LEER_IO, pid, parametros_leer);
    enviar_mem_request(mem_request);
    destruir_io_mem_request(mem_request);

    char *respuesta = (char *)recibir_mem_buffer();
    if (respuesta == NULL)
        return -1;

    printf("%s\n", respuesta);

    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), parametros[0]);
    FILE *archivo = fopen(path_archivo, "w");
    if (archivo == NULL)
    {
        free(path_archivo);
        // enviar_respuesta(pid, FILE_NOT_FOUND); VER PARA MANDAR AL KERNEL
        return;
    }
    //escribir respuesta en el archivo desde el bloque inicial + paramtros[3] (desplazamiento)
 }

void io_fs_read(char *argumentos, u_int32_t pid)
{
}
*/