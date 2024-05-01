#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H
#include <registros.h>
#include <stdlib.h>

// void instruction_fetch ();
// void instruction_decode();
// void instruction_execute();
// void check_interrupt ();

// ver porque los registros pueden ser de 32 o 8 bits => de que tipo son
// void SET(Registro, int valor);
// void SUM(Registro Destino, Registro Origen);
// void SUB(Registro Destino, Registro Origen);
// void JNZ(Registro, Instrucción);
// void IO_GEN_SLEEP(Interfaz, int unidades_trabajo);


//Instrucciones
// SET (Registro, Valor)
// MOV_IN (Registro Datos, Registro Dirección)
// MOV_OUT (Registro Dirección, Registro Datos)
// SUM (Registro Destino, Registro Origen)
// SUB (Registro Destino, Registro Origen)
// JNZ (Registro, Instrucción)
// RESIZE (Tamaño)
// COPY_STRING (Tamaño)
// WAIT (Recurso)
// SIGNAL (Recurso)
// IO_GEN_SLEEP (Interfaz, Unidades de trabajo)
// IO_STDIN_READ (Interfaz, Registro Dirección, Registro Tamaño)
// STDIN (Teclado) 
// IO_STDOUT_WRITE (Interfaz, Registro Dirección, Registro Tamaño)
// IO_FS_CREATE (Interfaz, Nombre Archivo)
// IO_FS_DELETE (Interfaz, Nombre Archivo)
// IO_FS_TRUNCATE (Interfaz, Nombre Archivo, Registro Tamaño)
// IO_FS_WRITE (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
// IO_FS_READ (Interfaz, Nombre Archivo, Registro Dirección, Registro Tamaño, Registro Puntero Archivo)
// EXIT

#endif // INSTRUCCIONES_H