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
   int tamano = string_array_size(array);

   char **nuevo_array = malloc(sizeof(char *) * (tamano - 1));

   // Verificar que el array no esté vacío

   for (int i = 0; i < tamano - 1; i++)
   {
      nuevo_array[i] = strdup(array[i + 1]);
   }

   return nuevo_array;
}