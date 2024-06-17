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

typedef union
{
   u_int32_t *direccion_logica_32;
   u_int8_t *direccion_logica_8;
} direccion_logica;

typedef union
{
   u_int32_t *registro_datos_32;
   u_int8_t *registro_datos_8;
} registro_datos;

typedef struct
{
   direccion_logica direccion_logica;
   registro_datos registro_datos;
   char *direcciones_fisicas;
} elementos;

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
 * @brief Guarda el valor en el registro.
 *
 * @param instruccion [ Registro | Valor ].
 * @note Ejemplo: "AX 10". (AX = 10)
 */
void set(char **parametros);

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
void mov_in(char **parametros);
void mov_out(char **parametros);
void copy_string(char **parametros);
void io_gen_sleep(char **parametros);
void io_stdin_read(char **parametros);
void io_stdout_write(char **parametros);
void exit_instruction(char **parametros);

/**
 * @brief Setea el diccionario de registros con los
 *        registros de la PCB de turno.
 *
 * @note Se debe llamar antes de ejecutar cualquier instrucción.
 * @note No hace falta destruir o crear el diccionario de nuevo.
 */
void set_registros();

/**
 * @brief Obtiene todas las direcciones físicas necesarias para la instrucción.
 *        Es necesario para el caso donde el contrnido a leer o escribir no cabe
 *        en un sóla página, o que simplemente fue dividido en dos páginas.
 *
 * @param direccion_logica Dirección lógica donde se arranca a operar.
 * @param tamanio_registro Tamaño del en bytes del contenido a operar.
 *
 * @return `char*` en caso de ser varias, será del formato "14 34 56",
 *         siendo cada número una dirección física.
 */
char *obtener_direcciones_fisicas(u_int32_t direccion_logica, u_int32_t tamanio_registro);

parametros crearParametrosLeer(char *direccion_fisica, u_int32_t tamanio_valor);
parametros crearParametrosEscribir(char *direccion_fisica, void *buffer, u_int32_t tamanio_valor);
u_int32_t obtener_tamanio_registro(char *parametros_recibidos);
elementos obtenerElementos(char **parametros_recibidos, int num);
char *obtenerElem(char **parametros);

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