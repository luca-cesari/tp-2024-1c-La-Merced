#include "instrucciones.h"

void inicializar_dicc_instrucciones(t_dictionary *dicc_instrucciones)
{
    dicc_instrucciones = dictionary_create();
    dictionary_put(dicc_instrucciones, "IO_FS_CREATE", &io_fs_create);
    // dictionary_put(dicc_instrucciones, "IO_FS_DELETE", &io_fs_delete);
    // dictionary_put(dicc_instrucciones, "IO_FS_TRUNCATE", &io_fs_truncate);
    // dictionary_put(dicc_instrucciones, "IO_FS_WRITE", &io_fs_write);
    // dictionary_put(dicc_instrucciones, "IO_FS_READ", &io_fs_read);
}

/*
    Creación de archivos
    Al momento de crearse un archivo, va a comenzar ocupando un bloque del FS aunque su tamaño sea 0 y luego el mismo se podrá extender o disminuir
    por medio de de la función IO_FS_TRUNCATE.
*/

void io_fs_create(char *argumentos, u_int32_t pid)
{
    char *path_archivo = string_from_format("%s/%s", get_path_base_dialfs(), argumentos);
    FILE *archivo = fopen(path_archivo, "r");
    if (archivo != NULL)
    {
        fclose(archivo);
        free(path_archivo);
        // enviar_respuesta(pid, FILE_ALREADY_EXISTS); VER PARA MANDAR AL KERNEL
        return;
    }

    archivo = fopen(path_archivo, "w");
    if (archivo == NULL)
    {
        perror("Error al crear el archivo");
        free(path_archivo);
        // enviar_respuesta(pid, FILE_CREATION_ERROR); VER PARA MANDAR AL KERNEL
        return;
    }

    fclose(archivo);
    free(path_archivo);
    // enviar_respuesta(pid, FILE_CREATED); VER PARA MANDAR AL KERNEL
    modificar_bitmap(get_siguiente_bloque_libre(), OCUPADO);
    // TODO: falta agregar al archivo de metadata el bloque inicial, y tamaño del archivo = 0
}