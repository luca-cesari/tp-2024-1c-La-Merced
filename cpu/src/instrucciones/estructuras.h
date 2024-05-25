#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/// ESTRUCTURAS DECODE ////////
typedef struct
{
   enum
   {
      INT32,
      INT8,
      VALOR,
      INTERFAZ,
      NINGUNO
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

#endif