#include "bitmap.h"

void inicializar_archivo_bitmap()
{
    char *path_bitmap = string_from_format("%s/bitmap.dat", get_path_base_dialfs());
    int fd = open(path_bitmap, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            // El archivo ya existe, no hacer nada
            free(path_bitmap);
            return;
        }
        else
        {
            // Otro error ocurrió
            perror("Error al abrir el archivo bitmap.dat");
            free(path_bitmap);
            return;
        }
    }

    ftruncate(fd, get_block_count() / 8); // Ajustar el tamaño del archivo

    char *map = mmap(0, get_block_count() / 8, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        perror("Error al mapear el archivo bitmap.dat");
        return;
    }

    t_bitarray *bitmap = bitarray_create_with_mode(map, get_block_count(), MSB_FIRST);
    for (int i = 0; i < get_block_count(); i++)
    {
        bitarray_clean_bit(bitmap, i);
    }

    munmap(map, get_block_count() / 8);
    close(fd);
    free(path_bitmap);
}

void modificar_bitmap(int bloque, estado estado_bloque)
{
    char *path_bitmap = string_from_format("%s/bitmap.dat", get_path_base_dialfs());
    int fd = open(path_bitmap, O_RDWR);
    if (fd == -1)
    {
        // Handle error
        perror("Error al abrir el archivo bitmap.dat");
        return;
    }

    char *map = mmap(0, get_block_count() / 8, PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED)
    {
        close(fd);
        // Handle error
        perror("Error al mapear el archivo bitmap.dat");
        return;
    }

    t_bitarray *bitmap = bitarray_create_with_mode(map, get_block_count(), MSB_FIRST);
    if (estado_bloque == OCUPADO)
    {
        bitarray_set_bit(bitmap, bloque);
    }
    else
    {
        bitarray_clean_bit(bitmap, bloque);
    }

    munmap(map, get_block_count() / 8);
    close(fd);
    free(path_bitmap);
}
