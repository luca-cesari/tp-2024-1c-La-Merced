#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <registers/registers.h>
#include <stdlib.h>
#include <string.h>
#include <pcb/pcb.h>
#include <io_request/io_request.h>
#include <mem_request/cpu.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>

#include "mmu/mmu.h"
#include "conexion/memoria.h"

/// ESTRUCTURAS DECODE ////////

// ver porque los registros pueden ser de 32 o 8 bits => de que tipo son
// void *set(char *registro, char *valor);
// void *sum(char *registro_destino, char *registro_origen);
// void *sub(char *registro_destino, char *registro_origen);
// void *jnz(char *registro, char *instruccion);
// void *io_gen_sleep(char *interfaz, char *unidades_trabajo);

void set(char **parametros);
void sum(char **parametros);
void sub(char **parametros);
void jnz(char **parametros);
void io_gen_sleep(char **parametros);
void io_stdin_read(char **parametros);
void io_stdout_write(char **parametros);
void exit_instruction(char **parametros);

///////FUNCINES OBTENER PARAMETROS //
void set_diccionario_registros(t_dictionary *);
void inicializar_diccionario_registros();

/////////AUX//////
char *array_a_string(char **array);
char **eliminar_primer_elemento(char **array);
char *obtener_direcciones_fisicas(u_int32_t direccion_logica, u_int32_t tamanio_registro, u_int32_t tamanio_pagina);
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