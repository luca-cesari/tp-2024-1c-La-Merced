#ifndef CICLO_H
#define CICLO_H
#include <protocol/registers.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <commons/collections/dictionary.h>

/// ESTRUCTURAS DECODE ////////
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
      u_int8_t valor;
      char *interfaz;
   } dato;
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

/// FUNCIONES DECODE ////////////

char **instruccion_parametros(char *char_instruccion);
int es_numero(char *parametro);
int char_a_numero(char *parametro);
Parametros obtener_parametros(char **parametros);
Parametro buscar_operando(char *parametro);
void set_diccionario_instrucciones(t_dictionary *);
void set_diccionario_registros(t_dictionary *);

#endif // CICLO_H