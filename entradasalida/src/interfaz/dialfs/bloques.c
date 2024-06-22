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