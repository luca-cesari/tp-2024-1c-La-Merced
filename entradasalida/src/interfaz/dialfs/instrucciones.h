#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <sockets/sockets.h>
#include <config/config.h>

void inicializar_dicc_instrucciones(t_dictionary *dicc_instrucciones);
void io_fs_create(char *argumentos, u_int32_t pid);
// void io_fs_delete(char *argumentos, u_int32_t pid);
// void io_fs_truncate(char *argumentos, u_int32_t pid);
// void io_fs_write(char *argumentos, u_int32_t pid);
// void io_fs_read(char *argumentos, u_int32_t pid);

#endif // DIALFS_H