#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H
#include <protocol/registers.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <commons/collections/dictionary.h>
#include <main.c>

///ESTRUCTURAS DECODE ////////
typedef struct
{
  enum
   {
      INT32,
      INT8,
      VALOR,
      INTERFAZ,
      NONE,
   } tipo_dato;
   union
   {
      u_int8_t *registro_u8;
      u_int32_t *registro_u32;
      int valor;
      char *interfaz;
   }dato;
} Parametro;

typedef struct
{
   Parametro parametro1;
   Parametro parametro2;
   // Parametro parametro3;
   // Parametro parametro4;
} Parametros;


char *fetch();
void (*decode(char *char_instruccion))(Parametros);
void execute(void (*instruccion)(Parametros), char *char_instruccion);
void check_interrupt();

// ver porque los registros pueden ser de 32 o 8 bits => de que tipo son
// void *set(char *registro, char *valor);
// void *sum(char *registro_destino, char *registro_origen);
// void *sub(char *registro_destino, char *registro_origen);
// void *jnz(char *registro, char *instruccion);
// void *io_gen_sleep(char *interfaz, char *unidades_trabajo);

// void *set(Parametros);
// void *sum(Parametros);
// void *sub(Parametros);
// void *jnz(Parametros);
// void *io_gen_sleep(Parametros);


/// FUNCIONES DECODE ////////////

char **instruccion_parametros(char *char_instruccion);
int es_numero(char *parametro);
int char_a_numero(char *parametro);
Parametros obtener_parametros(char **parametros);
Parametro buscar_operando(char *parametro);
void set_diccionario_instrucciones(t_dictionary *);
void set_diccionario_registros(t_dictionary *);

////////FUNCIONES EXECUTE////////
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