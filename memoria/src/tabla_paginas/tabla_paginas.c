#include "tabla_paginas.h"

t_mutext_list *lista_tablas;

static void destruir_proceso_tabla(void *ptr_tabla);
static void liberar_frame(void *ptr_nro_frame);

void inicializar_tabla_paginas()
{
   lista_tablas = mlist_create();
}

void crear_tabla_de_paginas_para_proceso(u_int32_t pid)
{
   t_proceso_tabla *tabla_paginas = malloc(sizeof(t_proceso_tabla));
   tabla_paginas->pid = pid;
   tabla_paginas->lista_frames = mlist_create();
   mlist_add(lista_tablas, tabla_paginas);
}

void destruir_tabla_de_paginas_para_proceso(u_int32_t pid)
{
   t_proceso_tabla *tabla_paginas = get_tabla_proceso(pid);
   mlist_iterate(tabla_paginas->lista_frames, &liberar_frame);

   int32_t es_tabla_buscada(void *elemento)
   {
      t_proceso_tabla *tabla = (t_proceso_tabla *)elemento;
      return tabla->pid == pid;
   };
   mlist_remove_and_destroy_by_condition(lista_tablas, &es_tabla_buscada, &destruir_proceso_tabla);
}

u_int32_t get_cantidad_paginas(t_proceso_tabla *tabla_paginas)
{
   return mlist_size(tabla_paginas->lista_frames);
}

void asignar_frame_a_tabla(t_proceso_tabla *tabla_paginas)
{
   u_int32_t frame = get_frame_libre();
   set_estado_frame(frame, OCUPADO);

   u_int32_t *nro_frame = malloc(sizeof(u_int32_t));
   *nro_frame = frame;
   mlist_add(tabla_paginas->lista_frames, nro_frame);
}

void liberar_frame_de_tabla(t_proceso_tabla *tabla_paginas)
{
   u_int32_t *nro_frame = mlist_remove_last(tabla_paginas->lista_frames);
   set_estado_frame(*nro_frame, LIBRE);
   free(nro_frame);
}

t_proceso_tabla *get_tabla_proceso(u_int32_t pid)
{
   int32_t es_tabla_buscada(void *elemento)
   {
      t_proceso_tabla *tabla = (t_proceso_tabla *)elemento;
      return tabla->pid == pid;
   };
   return (t_proceso_tabla *)mlist_find(lista_tablas, &es_tabla_buscada);
}

u_int32_t obtener_marco(u_int32_t pid, u_int32_t nro_pag)
{
   t_proceso_tabla *proceso_tabla = get_tabla_proceso(pid);
   u_int32_t *valor = (u_int32_t *)mlist_get(proceso_tabla->lista_frames, nro_pag);

   if (valor != NULL)
   {
      log_acceso_tabla_paginas(pid, nro_pag, *valor);
   }

   return valor == NULL ? -1 : *valor;
}

void destruir_lista_tablas()
{
   mlist_clean(lista_tablas, &destruir_proceso_tabla);
   mlist_destroy(lista_tablas);
}

static void destruir_proceso_tabla(void *ptr_tabla)
{
   t_proceso_tabla *tabla = (t_proceso_tabla *)ptr_tabla;
   mlist_clean(tabla->lista_frames, &free);
   mlist_destroy(tabla->lista_frames);
   free(tabla);
}

static void liberar_frame(void *ptr_nro_frame)
{
   u_int32_t nro_frame = *(u_int32_t *)ptr_nro_frame;
   set_estado_frame(nro_frame, LIBRE);
};