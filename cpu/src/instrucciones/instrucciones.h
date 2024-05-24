#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H
#include <protocol/registers.h>
#include <stdlib.h>
#include <string.h>
#include <protocol/pcb.h>
#include <protocol/io_request.h>
#include <ciclo.h>
#include <commons/string.h>

/// ESTRUCTURAS DECODE ////////

// ver porque los registros pueden ser de 32 o 8 bits => de que tipo son
// void *set(char *registro, char *valor);
// void *sum(char *registro_destino, char *registro_origen);
// void *sub(char *registro_destino, char *registro_origen);
// void *jnz(char *registro, char *instruccion);
// void *io_gen_sleep(char *interfaz, char *unidades_trabajo);

void set(Parametros);
void sum(Parametros);
void sub(Parametros);
void jnz(Parametros);
void io_gen_sleep(Parametros);

// Instrucciones////////////////////////////////////////
//  SET (Registro, Valor)
//  MOV_IN (Registro Datos, Registro Dirección)
//  MOV_OUT (Registro Dirección, Registro Datos)
//  SUM (Registro Destino, Registro Origen)
//  SUB (Registro Destino, Registro Origen)
//  JNZ (Registro, Instrucción)
//  RESIZE (Tamaño)
//  COPY_STRING (Tamaño)
//  WAIT (Recurso)
//  SIGNAL (Recurso)
//  IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
//  IO_STDIN_READ (Interfaz, Registro Dirección, Registro Tamaño)
//  STDIN (Teclado)
//  IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño)
//  IO_FS_CREATE (Interfaz, Nombre Archivo)
//  IO_FS_DELETE (Interfaz, Nombre Archivo)
//  IO_FS_TRUNCATE (Interfaz, Nombre Archivo, Registro Tamaño)
//  IO_FS_WRITE (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
//  IO_FS_READ (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
//  EXIT

#endif // INSTRUCCIONES_H