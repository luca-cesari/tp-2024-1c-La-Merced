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

u_int32_t string_full_lenght(char *string)
{
   return strlen(string) + 1;
}

u_int8_t string_is_equal(char *string1, char *string2)
{
   return strcmp(string1, string2) == 0 ? 1 : 0;
}