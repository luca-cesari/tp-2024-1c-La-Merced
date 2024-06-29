#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

char *array_a_string(char **array);
char **eliminar_primer_elemento(char **array);
char *eliminar_salto_linea(char *linea);
u_int32_t string_full_length(char *string);
u_int8_t string_is_equal(char *string1, char *string2);

#endif // UTILS_STRING_H