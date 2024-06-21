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

void io_fs_create(char *argumentos, u_int32_t pid)
{
    return NULL;
}