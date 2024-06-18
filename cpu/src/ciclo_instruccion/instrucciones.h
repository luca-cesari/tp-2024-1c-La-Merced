#ifndef INSTRUCCIONES_H
#define INSTRUCCIONES_H

#include <stdlib.h>
#include <commons/collections/dictionary.h>

#include <pcb/pcb.h>
#include <registers/registers.h>
#include <io_request/io_request.h>
#include <mem_request/cpu.h>
#include <mem_request/mem_response.h>
#include <extended/string.h>

#include "conexion/memoria.h"
#include "logger/logger.h"
#include "mmu/mmu.h"
#include "registros/registros.h"

typedef enum
{
   REGISTER_SIZE,
   REGISTER_CONTENT,
} size_flag;

/**
 * @brief Guarda el valor en el registro.
 *
 * @param instruccion [ Registro | Valor ].
 * @note Ejemplo: "AX 10". (AX = 10)
 */
void set(char **parametros);

void mov_in(char **parametros);
void mov_out(char **parametros);

/**
 * @brief Suma el valor del registro origen al valor del registro destino.
 *        El resultado se guarda en el registro destino.
 *
 * @param parametros [ Registro Destino | Registro Origen ].
 * @note Ejemplo: "AX BX". (AX = AX + BX)
 */
void sum(char **parametros);

/**
 * @brief Resta el valor del registro origen al valor del registro destino.
 *        El resultado se guarda en el registro destino
 *
 * @param parametros [ Registro Destino | Registro Origen ].
 * @note Ejemplo: "AX BX". (AX = AX - BX)
 */
void sub(char **parametros);
void jnz(char **parametros);
void resize(char **parametros);
void copy_string(char **parametros);
void io_gen_sleep(char **parametros);
void io_stdin_read(char **parametros);
void io_stdout_write(char **parametros);
void exit_instruction(char **parametros);

void inicializar_instrucciones(void);

/**
 * @brief Devuelve una función instrucción según la instrucción recibida.
 *
 * @param instruccion Ejemplo:
 *        "SET", "SUM", "JNZ", "RESIZE",
 *        "COPY_STRING", "IO_GEN_SLEEP",
 *        "IO_STDIN_READ", "IO_STDOUT_WRITE", "EXIT".
 *
 * @return `void (*)(char **)` Función instrucción.
 *
 * @note Las funciones instrucción reciben un array de strings de la commons.
 *       Ejemplo: [ "AX", "10", NULL ].
 */
void (*get_instruccion(char *instruccion))(char **);

/**
 * @brief Obtiene todas las direcciones físicas necesarias para la instrucción.
 *        Es necesario para el caso donde el contrnido a leer o escribir no cabe
 *        en un sóla página, o que simplemente fue dividido en dos páginas.
 *
 * @param direccion_logica Dirección lógica donde se arranca a operar.
 * @param tamanio_dato Tamaño del en bytes del contenido a operar.
 *
 * @return `char*` en caso de ser varias, será del formato "14 34 56",
 *         siendo cada número una dirección física.
 */
char *obtener_direcciones_fisicas(u_int32_t direccion_logica, u_int32_t tamanio_dato);

char *get_direccion_tamanio(char **parametros);

// parametros es algo de mem_request, los constructores no tienen que ir aca
parametros crear_parametros_leer(char *direccion_fisica, u_int32_t tamanio_valor);
parametros crear_parametros_escribir(char *direccion_fisica, void *buffer, u_int32_t tamanio_valor);

// Instrucciones

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