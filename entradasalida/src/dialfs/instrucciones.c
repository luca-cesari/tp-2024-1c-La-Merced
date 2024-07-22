#include "instrucciones.h"

t_dictionary *dicc_instrucciones;

void inicializar_dicc_instrucciones()
{
    dicc_instrucciones = dictionary_create();
    dictionary_put(dicc_instrucciones, "IO_FS_CREATE", &io_fs_create);
    dictionary_put(dicc_instrucciones, "IO_FS_DELETE", &io_fs_delete);
    dictionary_put(dicc_instrucciones, "IO_FS_TRUNCATE", &io_fs_truncate);
    dictionary_put(dicc_instrucciones, "IO_FS_WRITE", &io_fs_write);
    dictionary_put(dicc_instrucciones, "IO_FS_READ", &io_fs_read);
}

int8_t (*get_funcion_instruccion(char *instruccion))(char *, u_int32_t)
{
    if (!dictionary_has_key(dicc_instrucciones, instruccion))
        return NULL;

    return dictionary_get(dicc_instrucciones, instruccion);
}

int8_t io_fs_create(char *argumentos, u_int32_t pid)
{
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), argumentos); // Pensando que es el único argumento que viene
    crear_archivo_metadata(path_archivo, get_siguiente_bloque_libre(), 0);
    modificar_bitmap(get_siguiente_bloque_libre(), OCUPADO);
    free(path_archivo);
    // enviar_respuesta(pid, FILE_CREATED); VER PARA MANDAR AL KERNEL
    return 0;
}

int8_t io_fs_delete(char *argumentos, u_int32_t pid)
{
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), argumentos); // Pensando que es el único argumento que viene
    FILE *archivo = fopen(path_archivo, "r");
    if (archivo == NULL)
    {
        free(path_archivo);
        // enviar_respuesta(pid, FILE_NOT_FOUND); VER PARA MANDAR AL KERNEL
        return -1;
    }

    fclose(archivo);

    get_cantidad_bloques_ocupados(path_archivo);
    for (int i = 0; i < get_cantidad_bloques_ocupados(path_archivo); i++) // Va recorriendo los bloques ocupados y los va liberando ya que son contiguos
    {
        modificar_bitmap(get_bloque_inicial(path_archivo) + i, LIBRE);
    }
    eliminar_archivo_metadata(path_archivo);
    free(path_archivo);
    return 0;
}

int8_t io_fs_truncate(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    u_int32_t nuevo_tamanio = atoi(parametros[1]);
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), parametros[0]);
    u_int32_t tamanio_archivo = get_cantidad_bloques_ocupados(path_archivo) * get_block_size();

    u_int32_t cantidad_bloques_necesarios = nuevo_tamanio / get_block_size() + (nuevo_tamanio % get_block_size() != 0);
    u_int32_t bloques_ocupados = get_cantidad_bloques_ocupados(path_archivo);

    if (nuevo_tamanio > tamanio_archivo)
    {
        u_int32_t bloques_faltantes = cantidad_bloques_necesarios - bloques_ocupados;

        if (!hay_bloques_libres_contiguos(get_bloque_inicial(path_archivo) + bloques_ocupados, bloques_faltantes))
        {
            compactar(path_archivo, tamanio_archivo, bloques_ocupados);
        }

        for (int i = 0; i < bloques_faltantes; i++)
        {
            modificar_bitmap(get_bloque_inicial(path_archivo) + bloques_ocupados + i, OCUPADO);
        }
    }
    else
    {
        for (int i = bloques_ocupados; i > cantidad_bloques_necesarios; i--) // Va recorriendo los bloques ocupados y los va liberando ya que son contiguos
        {
            modificar_bitmap(get_bloque_inicial(path_archivo) + i, LIBRE);
        }
    }

    set_tamanio_archivo(path_archivo, nuevo_tamanio);
    free(path_archivo);
    string_array_destroy(parametros);
    return 0;
}

int8_t io_fs_write(char *argumentos, u_int32_t pid) // salida.txt tamaño puntero_Arc 0 16 32
{
    char **parametros = string_split(argumentos, " ");
    char *direcciones_fisicas = string_new();
    int i = 3;

    while (parametros[i] != NULL)
    {
        string_append(&direcciones_fisicas, parametros[i]);
        string_append(&direcciones_fisicas, " ");
        i++;
    }

    u_int32_t tamanio_valor = atoi(parametros[1]);
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), parametros[0]);
    u_int32_t offset = atoi(parametros[2]);

    t_io_mem_req *mem_request = crear_io_mem_request(LEER_IO, pid, direcciones_fisicas, tamanio_valor, NULL);
    enviar_mem_request(mem_request);

    t_mem_buffer_response *respuesta = recibir_mem_buffer();

    if (respuesta == NULL)
    {
        free(direcciones_fisicas);
        free(path_archivo);
        return -1; // Error al recibir respuesta de memoria
    }

    pegar_bloque_datos_con_offset((char *)respuesta->buffer, get_bloque_inicial(path_archivo), offset, tamanio_valor);

    free(direcciones_fisicas);
    free(path_archivo);
    destruir_io_mem_request(mem_request);
    destruir_buffer_response(respuesta);
    string_array_destroy(parametros);

    return 0;
}

int8_t io_fs_read(char *argumentos, u_int32_t pid)
{
    char **parametros = string_split(argumentos, " ");
    char *direccion_escribir = string_new();
    int i = 3;

    while (parametros[i] != NULL)
    {
        string_append(&direccion_escribir, parametros[i]);
        string_append(&direccion_escribir, " ");
        i++;
    }

    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), parametros[0]);
    u_int32_t offset = atoi(parametros[2]);
    u_int32_t bloque_inicial = get_bloque_inicial(path_archivo);
    u_int32_t tamanio = atoi(parametros[1]);

    char *buffer_archivo = malloc(tamanio);
    copiar_de_bloque_datos_con_offset(buffer_archivo, bloque_inicial, offset, tamanio);

    t_io_mem_req *mem_request = crear_io_mem_request(ESCRIBIR_IO, pid, direccion_escribir, tamanio, buffer_archivo);
    enviar_mem_request(mem_request);
    destruir_io_mem_request(mem_request);

    t_mem_response response = recibir_valor();

    free(path_archivo);
    free(buffer_archivo);
    free(direccion_escribir);
    string_array_destroy(parametros);
    // ver como liberar parametros**
    return response == OPERATION_SUCCEED ? 0 : -1;
}

/*Pasos a seguir para compactar
1) Copiar el archivo a truncar en un buffer
2) Mover todos los archivos siguientes a la posición del archivo a truncar a la primera posición donde se encontraba el archivo a truncar
3) Poner el archivo al final de los archivos que se movieron en el paso 2
*/

void compactar(char *path, u_int32_t tamanio_archivo, u_int32_t cantidad_bloques_ocupados)
{

    u_int32_t bloque_inicial = get_bloque_inicial(path);

    char *buffer = malloc(tamanio_archivo);
    copiar_de_bloque_datos(buffer, bloque_inicial, tamanio_archivo); // Se copia en un buffer el archivo a truncar

    u_int32_t bloque_inicial_con_desplazamiento = bloque_inicial;

    // Quiero obtener el indice de el archivo
    u_int32_t indice_primer_archivo_desplazar = obtener_indice_archivo(path) + 1;

    for (int i = indice_primer_archivo_desplazar; i < obtener_tamanio_lista_archivos(); i++)
    {
        desplazar_archivo_en_bloques(get_path_archivo_por_indice(i), bloque_inicial_con_desplazamiento);
        actualizar_archivo_metadata(get_path_archivo_por_indice(i), bloque_inicial_con_desplazamiento);
        bloque_inicial_con_desplazamiento += get_cantidad_bloques_ocupados(get_path_archivo_por_indice(i));
    }

    pegar_bloque_datos(buffer, bloque_inicial_con_desplazamiento, tamanio_archivo); // Se pega el archivo a truncar al final de los archivos desplazados
    actualizar_archivo_metadata(path, bloque_inicial_con_desplazamiento);
    bloque_inicial_con_desplazamiento += cantidad_bloques_ocupados;

    ordenar_lista_archivos(obtener_indice_archivo(path));
    // Ver si meter lo de abajo en una funcion

    for (int i = bloque_inicial; i < bloque_inicial_con_desplazamiento; i++)
    {
        modificar_bitmap(i, OCUPADO);
    }

    liberar_bitmap_a_partir_de(bloque_inicial_con_desplazamiento);

    // free(buffer); Lo comento porque tira exepción. Seguramente se libera en otra parte
    // free(path); No hay que liberarlo, se hace más adelante.

    sleep(get_retraso_compactacion() / 1000);
}