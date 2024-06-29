#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <stdio.h>
#include <commons/collections/dictionary.h>
#include <sockets/sockets.h>
#include <extended/string.h>
#include <mem_request/entradasalida.h>

#include "config/config.h"
#include "metadata.h"
#include "bitmap.h"

void inicializar_dicc_instrucciones();
void (*get_funcion_instruccion(char *instruccion))(char *, u_int32_t);
void io_fs_create(char *argumentos, u_int32_t pid);
void io_fs_delete(char *argumentos, u_int32_t pid);
void io_fs_truncate(char *argumentos, u_int32_t pid);
// void io_fs_write(char *argumentos, u_int32_t pid);
// void io_fs_read(char *argumentos, u_int32_t pid);

#endif // DIALFS_H