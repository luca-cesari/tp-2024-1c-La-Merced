#include "string.h"

char *array_a_string(char **array)
{
   char *string = string_new();
   for (int i = 0; array[i] != NULL; i++)
   {
      string_append(&string, array[i]);
      string_append(&string, " ");
   }
   string_trim_right(&string);

   return string;
}

char **eliminar_primer_elemento(char **array)
{
   char **nuevo_array = string_array_new();

   for (int i = 1; array[i] != NULL; i++)
      string_array_push(&nuevo_array, array[i]);

   return nuevo_array;
}

char *eliminar_salto_linea(char *linea)
{
   return string_replace(linea, "\n", "");
}