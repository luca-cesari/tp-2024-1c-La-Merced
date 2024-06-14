#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <stdlib.h>
#include <string.h>
#include <commons/string.h>

char *array_a_string(char **array);
char **eliminar_primer_elemento(char **array);
char *eliminar_salto_linea(char *linea);

#endif // UTILS_STRING_H