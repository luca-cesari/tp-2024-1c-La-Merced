#include "recursos.h"

typedef struct
{
   char *nombre_recurso;
   u_int32_t instancias;
} t_recurso;

t_recurso *recursos;

void inicializar_recursos()
{
   t_dictionary *config_recursos = get_recursos();

   dictionary_destroy_and_destroy_elements(config_recursos, &free);
}