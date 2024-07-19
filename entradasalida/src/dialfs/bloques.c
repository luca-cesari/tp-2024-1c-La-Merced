#include "bloques.h"

void inicializar_archivo_bloques() // tener en cuenta que si la carpeta no se encuentra creada tira excepcion, esta bien igual
{

    char *path_bloques = string_from_format("%s/bloques.dat", get_path_base_dialfs());
    FILE *bloques = fopen(path_bloques, "r");
    if (bloques == NULL)
    {
        bloques = fopen(path_bloques, "w");
        ftruncate(fileno(bloques), get_block_size() * get_block_count());
        fclose(bloques);
    }
    else
    {
        fclose(bloques);
    }
    free(path_bloques);
}

void copiar_de_bloque_datos(char *buffer, u_int32_t bloque_inicial, u_int32_t tamanio_archivo)
{
    char *path_bloques = string_from_format("%s/bloques.dat", get_path_base_dialfs());
    FILE *bloques = fopen(path_bloques, "r+");
    fseek(bloques, bloque_inicial * get_block_size(), SEEK_SET);
    fwrite(buffer, tamanio_archivo, 1, bloques);
    fclose(bloques);
    free(path_bloques);
}

void copiar_de_bloque_datos_con_offset(char *buffer, u_int32_t bloque_inicial, u_int32_t offset ,u_int32_t tamanio_archivo)
{
    char *path_bloques = string_from_format("%s/bloques.dat", get_path_base_dialfs());
    FILE *bloques = fopen(path_bloques, "r+");
    fseek(bloques, bloque_inicial * get_block_size() + offset, SEEK_SET);
    fwrite(buffer, tamanio_archivo, 1, bloques);
    fclose(bloques);
    free(path_bloques);
}

void pegar_bloque_datos(char *buffer, u_int32_t bloque_inicial, u_int32_t tamanio_archivo)
{
    char *path_bloques = string_from_format("%s/bloques.dat", get_path_base_dialfs());
    FILE *bloques = fopen(path_bloques, "r+");
    fseek(bloques, bloque_inicial * get_block_size(), SEEK_SET);
    fwrite(buffer, tamanio_archivo, 1, bloques);
    fclose(bloques);
    free(path_bloques);
}

void desplazar_archivo_en_bloques(char *path_archivo_a_desplazar, u_int32_t bloque_inicial_a_desplazar)
{
    char *buffer = malloc(get_tamanio_archivo(path_archivo_a_desplazar));
    copiar_de_bloque_datos(buffer, get_bloque_inicial(path_archivo_a_desplazar), get_tamanio_archivo(path_archivo_a_desplazar));
    pegar_bloque_datos(buffer, bloque_inicial_a_desplazar, get_tamanio_archivo(path_archivo_a_desplazar));
}