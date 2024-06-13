#include "consola.h"

char *iniciar_consola(u_int32_t tamanio_valor)
{
   char *comando = NULL;
   char *valor = NULL;

   while (1) // si el length es menor, que pida de nuevo hasta que sea mayor o igual al tamanio requerido
   {
      comando = readline("> ");

      if (string_length(comando) >= tamanio_valor)
      {
         valor = string_substring_until(comando, tamanio_valor);
         free(comando);
         return valor; // que onda el free
      }
      free(comando); // lo mismo, por las dudas
   }
}