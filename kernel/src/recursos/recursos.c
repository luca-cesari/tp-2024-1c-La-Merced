#include "recursos.h"

t_mutext_list *recursos;

static t_recurso *crear_recurso(char *nombre_recurso, u_int32_t instancias);
static void agregar_recurso(char *nombre_recurso, void *ptr_instancias);
static void destruir_recurso(t_recurso *recurso);

void inicializar_recursos()
{
   t_dictionary *config_recursos = get_recursos();
   dictionary_iterator(config_recursos, &agregar_recurso);
   dictionary_destroy_and_destroy_elements(config_recursos, &free);
}

int32_t consumir_recurso(t_pcb *pcb, char *nombre_recurso)
{
}

int32_t liberar_recurso(t_pcb *pcb, char *nombre_recurso)
{
}

void destruir_recursos()
{
   mlist_clean(recursos, &destruir_recurso);
   mlist_destroy(recursos);
}

static t_recurso *crear_recurso(char *nombre_recurso, u_int32_t instancias)
{
   t_recurso *recurso = malloc(sizeof(t_recurso));
   recurso->nombre_recurso = nombre_recurso;
   recurso->instancias = instancias;

   return recurso;
}

static void agregar_recurso(char *nombre_recurso, void *ptr_instancias)
{
   u_int32_t instancias = *(u_int32_t *)ptr_instancias;
   t_recurso *recurso = crear_recurso(nombre_recurso, instancias);
   mlist_add(recursos, recurso);
}

static void destruir_recurso(t_recurso *recurso)
{
   free(recurso->nombre_recurso);
   free(recurso);
}