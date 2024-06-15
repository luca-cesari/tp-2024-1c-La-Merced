#include "string.h"

char *array_a_string(char **array)
{
   char *string = string_new();
   int i = 0;
   while (array[i] != NULL)
   {
      string_append(&string, array[i]);
      i++;
   }
   return string;
}

char **eliminar_primer_elemento(char **array)
{
   // int tamano = string_array_size(array);

   char **nuevo_array = string_array_new();

   // Verificar que el array no esté vacío

   for (int i = 1; array[i] != NULL; i++)
   {
      string_array_push(&nuevo_array, array[i]);
   }

   return nuevo_array;
}

char *eliminar_salto_linea(char *linea)
{
   return string_replace(linea, "\n", "");
}